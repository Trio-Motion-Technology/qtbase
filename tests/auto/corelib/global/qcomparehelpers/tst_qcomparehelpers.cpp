// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include <QtCore/qcompare.h>
#include <QtTest/qtest.h>
#include <QtTest/private/qcomparisontesthelper_p.h>

#if defined(__STDCPP_FLOAT16_T__) && __has_include(<stdfloat>)
#include <stdfloat>
#endif

class IntWrapper
{
public:
    // implicit constructor and operator int() to simulate the case that
    // triggers a bug on MSVC < 19.36.
    IntWrapper(int val) : m_val(val) {}
    operator int() const noexcept { return m_val; }

    int value() const { return m_val; }

private:
    friend bool comparesEqual(const IntWrapper &lhs, const IntWrapper &rhs) noexcept
    { return lhs.m_val == rhs.m_val; }
    friend Qt::strong_ordering
    compareThreeWay(const IntWrapper &lhs, const IntWrapper &rhs) noexcept
    {
        return Qt::compareThreeWay(lhs.m_val, rhs.m_val);
    }
    friend bool comparesEqual(const IntWrapper &lhs, int rhs) noexcept
    { return lhs.m_val == rhs; }
    friend Qt::strong_ordering compareThreeWay(const IntWrapper &lhs, int rhs) noexcept
    { return compareThreeWay(lhs, IntWrapper(rhs)); }

    Q_DECLARE_STRONGLY_ORDERED(IntWrapper)
    Q_DECLARE_STRONGLY_ORDERED(IntWrapper, int)

    int m_val = 0;
};

class DoubleWrapper
{
public:
    explicit DoubleWrapper(double val) : m_val(val) {}
    double value() const { return m_val; }

private:
    friend bool comparesEqual(const DoubleWrapper &lhs, const DoubleWrapper &rhs) noexcept
    { return lhs.m_val == rhs.m_val; }
    friend Qt::partial_ordering
    compareThreeWay(const DoubleWrapper &lhs, const DoubleWrapper &rhs) noexcept
    {
        return Qt::compareThreeWay(lhs.m_val, rhs.m_val);
    }
    friend bool comparesEqual(const DoubleWrapper &lhs, const IntWrapper &rhs) noexcept
    { return comparesEqual(lhs, DoubleWrapper(rhs.value())); }
    friend Qt::partial_ordering
    compareThreeWay(const DoubleWrapper &lhs, const IntWrapper &rhs) noexcept
    { return compareThreeWay(lhs, DoubleWrapper(rhs.value())); }
    friend bool comparesEqual(const DoubleWrapper &lhs, double rhs) noexcept
    { return lhs.m_val == rhs; }
    friend Qt::partial_ordering compareThreeWay(const DoubleWrapper &lhs, double rhs) noexcept
    {
        return Qt::compareThreeWay(lhs.m_val, rhs);
    }

    Q_DECLARE_PARTIALLY_ORDERED(DoubleWrapper)
    Q_DECLARE_PARTIALLY_ORDERED(DoubleWrapper, IntWrapper)
    Q_DECLARE_PARTIALLY_ORDERED(DoubleWrapper, double)

    double m_val = 0.0;
};

template <typename String>
class StringWrapper
{
public:
    explicit StringWrapper(String val) : m_val(val) {}
    String value() const { return m_val; }

private:
    // Some of the helper functions are intentionally NOT marked as noexcept
    // to test the conditional noexcept in the macros.
    template <typename T>
    friend bool comparesEqual(const StringWrapper<T> &, const StringWrapper<T> &) noexcept;
    template <typename T>
    friend Qt::weak_ordering
    compareThreeWay(const StringWrapper<T> &, const StringWrapper<T> &) noexcept;
    template <typename T>
    friend bool comparesEqual(const StringWrapper<T> &, QAnyStringView);
    template <typename T>
    friend Qt::weak_ordering compareThreeWay(const StringWrapper<T> &, QAnyStringView);

    Q_DECLARE_WEAKLY_ORDERED(StringWrapper)
    Q_DECLARE_WEAKLY_ORDERED(StringWrapper, QAnyStringView)

    String m_val;
};

// StringWrapper comparison helper functions

bool equalsHelper(QAnyStringView lhs, QAnyStringView rhs) noexcept
{
    return QAnyStringView::compare(lhs, rhs, Qt::CaseInsensitive) == 0;
}

template <typename T>
bool comparesEqual(const StringWrapper<T> &lhs, const StringWrapper<T> &rhs) noexcept
{
    return equalsHelper(lhs.m_val, rhs.m_val);
}

Qt::weak_ordering compareHelper(QAnyStringView lhs, QAnyStringView rhs) noexcept
{
    const int res = QAnyStringView::compare(lhs, rhs, Qt::CaseInsensitive);
    if (res < 0)
        return Qt::weak_ordering::less;
    else if (res > 0)
        return Qt::weak_ordering::greater;
    else
        return Qt::weak_ordering::equivalent;
}

template <typename T>
Qt::weak_ordering compareThreeWay(const StringWrapper<T> &lhs, const StringWrapper<T> &rhs) noexcept
{
    return compareHelper(lhs.m_val, rhs.m_val);
}

template <typename T>
bool comparesEqual(const StringWrapper<T> &lhs, QAnyStringView rhs)
{
    return equalsHelper(lhs.m_val, rhs);
}

template <typename T>
Qt::weak_ordering compareThreeWay(const StringWrapper<T> &lhs, QAnyStringView rhs)
{
    return compareHelper(lhs.m_val, rhs);
}

// Test class

class tst_QCompareHelpers : public QObject
{
    Q_OBJECT

private:
    template <typename LeftType, typename RightType, typename OrderingType>
    void compareImpl();

    template <typename LeftType, typename RightType>
    void compareIntData();

    template <typename LeftType, typename RightType>
    void compareFloatData();

    template <typename LeftType, typename RightType>
    void compareStringData();

private slots:
    void comparisonCompiles();

    void compare_IntWrapper_data() { compareIntData<IntWrapper, IntWrapper>(); }
    void compare_IntWrapper() { compareImpl<IntWrapper, IntWrapper, Qt::strong_ordering>(); }

    void compare_IntWrapper_int_data() { compareIntData<IntWrapper, int>(); }
    void compare_IntWrapper_int() { compareImpl<IntWrapper, int, Qt::strong_ordering>(); }

    void compare_DoubleWrapper_data() { compareFloatData<DoubleWrapper, DoubleWrapper>(); }
    void compare_DoubleWrapper()
    { compareImpl<DoubleWrapper, DoubleWrapper, Qt::partial_ordering>(); }

    void compare_DoubleWrapper_double_data() { compareFloatData<DoubleWrapper, double>(); }
    void compare_DoubleWrapper_double()
    { compareImpl<DoubleWrapper, double, Qt::partial_ordering>(); }

    void compare_IntWrapper_DoubleWrapper_data();
    void compare_IntWrapper_DoubleWrapper()
    { compareImpl<IntWrapper, DoubleWrapper, Qt::partial_ordering>(); }

    void compare_StringWrapper_data()
    { compareStringData<StringWrapper<QString>, StringWrapper<QString>>(); }
    void compare_StringWrapper()
    { compareImpl<StringWrapper<QString>, StringWrapper<QString>, Qt::weak_ordering>(); }

    void compare_StringWrapper_AnyStringView_data()
    { compareStringData<StringWrapper<QString>, QAnyStringView>(); }
    void compare_StringWrapper_AnyStringView()
    { compareImpl<StringWrapper<QString>, QAnyStringView, Qt::weak_ordering>(); }

    void generatedClasses();

    void builtinOrder();
};

template<typename LeftType, typename RightType, typename OrderingType>
void tst_QCompareHelpers::compareImpl()
{
    QFETCH(LeftType, lhs);
    QFETCH(RightType, rhs);
    QFETCH(OrderingType, expectedOrdering);

    QTestPrivate::testAllComparisonOperators(lhs, rhs, expectedOrdering);
    if (QTest::currentTestFailed())
        return;
#ifdef __cpp_lib_three_way_comparison
    // Also check std types.

    // if Ordering == Qt::strong_ordering -> std::strong_ordering
    // else if Ordering == Qt::weak_ordering -> std::weak_ordering
    // else std::partial_ordering
    using StdType = std::conditional_t<
                        std::is_same_v<OrderingType, Qt::strong_ordering>,
                            std::strong_ordering,
                            std::conditional_t<std::is_same_v<OrderingType, Qt::weak_ordering>,
                                std::weak_ordering,
                                std::partial_ordering>>;

    QTestPrivate::testAllComparisonOperators(lhs, rhs, static_cast<StdType>(expectedOrdering));
    if (QTest::currentTestFailed())
        return;
#endif // __cpp_lib_three_way_comparison
}

template<typename LeftType, typename RightType>
void tst_QCompareHelpers::compareIntData()
{
    QTest::addColumn<LeftType>("lhs");
    QTest::addColumn<RightType>("rhs");
    QTest::addColumn<Qt::strong_ordering>("expectedOrdering");

    auto createRow = [](auto lhs, auto rhs, Qt::strong_ordering ordering) {
        QTest::addRow("%d vs %d", lhs, rhs) << LeftType(lhs) << RightType(rhs) << ordering;
    };

    createRow(0, 0, Qt::strong_ordering::equivalent);
    createRow(-1, 0, Qt::strong_ordering::less);
    createRow(1, 0, Qt::strong_ordering::greater);
    constexpr int max = std::numeric_limits<int>::max();
    constexpr int min = std::numeric_limits<int>::min();
    createRow(max, max, Qt::strong_ordering::equivalent);
    createRow(min, min, Qt::strong_ordering::equivalent);
    createRow(max, min, Qt::strong_ordering::greater);
    createRow(min, max, Qt::strong_ordering::less);
}

template<typename LeftType, typename RightType>
void tst_QCompareHelpers::compareFloatData()
{
    QTest::addColumn<LeftType>("lhs");
    QTest::addColumn<RightType>("rhs");
    QTest::addColumn<Qt::partial_ordering>("expectedOrdering");

    auto createRow = [](auto lhs, auto rhs, Qt::partial_ordering ordering) {
        QTest::addRow("%f vs %f", lhs, rhs) << LeftType(lhs) << RightType(rhs) << ordering;
    };

    createRow(0.0, 0.0, Qt::partial_ordering::equivalent);
    createRow(-0.000001, 0.0, Qt::partial_ordering::less);
    createRow(0.000001, 0.0, Qt::partial_ordering::greater);

    const double nan = qQNaN();
    createRow(nan, 0.0, Qt::partial_ordering::unordered);
    createRow(0.0, nan, Qt::partial_ordering::unordered);
    createRow(nan, nan, Qt::partial_ordering::unordered);

    const double inf = qInf();
    createRow(inf, 0.0, Qt::partial_ordering::greater);
    createRow(0.0, inf, Qt::partial_ordering::less);
    createRow(-inf, 0.0, Qt::partial_ordering::less);
    createRow(0.0, -inf, Qt::partial_ordering::greater);
    createRow(inf, inf, Qt::partial_ordering::equivalent);
    createRow(-inf, -inf, Qt::partial_ordering::equivalent);
    createRow(-inf, inf, Qt::partial_ordering::less);
    createRow(inf, -inf, Qt::partial_ordering::greater);

    createRow(nan, inf, Qt::partial_ordering::unordered);
    createRow(inf, nan, Qt::partial_ordering::unordered);
    createRow(nan, -inf, Qt::partial_ordering::unordered);
    createRow(-inf, nan, Qt::partial_ordering::unordered);
}

template<typename LeftType, typename RightType>
void tst_QCompareHelpers::compareStringData()
{
    QTest::addColumn<LeftType>("lhs");
    QTest::addColumn<RightType>("rhs");
    QTest::addColumn<Qt::weak_ordering>("expectedOrdering");

    auto createRow = [](auto lhs, auto rhs, Qt::weak_ordering ordering) {
        QTest::addRow("'%s' vs '%s'", lhs, rhs) << LeftType(lhs) << RightType(rhs) << ordering;
    };

    createRow("", "", Qt::weak_ordering::equivalent);
    createRow("Ab", "abc", Qt::weak_ordering::less);
    createRow("aBc", "AB", Qt::weak_ordering::greater);
    createRow("ab", "AB", Qt::weak_ordering::equivalent);
    createRow("ABC", "abc", Qt::weak_ordering::equivalent);
}

void tst_QCompareHelpers::comparisonCompiles()
{
    QTestPrivate::testAllComparisonOperatorsCompile<IntWrapper>();
    if (QTest::currentTestFailed())
        return;

    QTestPrivate::testAllComparisonOperatorsCompile<IntWrapper, int>();
    if (QTest::currentTestFailed())
        return;

    QTestPrivate::testAllComparisonOperatorsCompile<DoubleWrapper>();
    if (QTest::currentTestFailed())
        return;

    QTestPrivate::testAllComparisonOperatorsCompile<DoubleWrapper, double>();
    if (QTest::currentTestFailed())
        return;

    QTestPrivate::testAllComparisonOperatorsCompile<DoubleWrapper, IntWrapper>();
    if (QTest::currentTestFailed())
        return;

    QTestPrivate::testAllComparisonOperatorsCompile<StringWrapper<QString>>();
    if (QTest::currentTestFailed())
        return;

    QTestPrivate::testAllComparisonOperatorsCompile<StringWrapper<QString>, QAnyStringView>();
    if (QTest::currentTestFailed())
        return;
}

void tst_QCompareHelpers::compare_IntWrapper_DoubleWrapper_data()
{
    QTest::addColumn<IntWrapper>("lhs");
    QTest::addColumn<DoubleWrapper>("rhs");
    QTest::addColumn<Qt::partial_ordering>("expectedOrdering");

    auto createRow = [](auto lhs, auto rhs, Qt::partial_ordering ordering) {
        QTest::addRow("%d vs %f", lhs, rhs) << IntWrapper(lhs) << DoubleWrapper(rhs) << ordering;
    };

    createRow(0, 0.0, Qt::partial_ordering::equivalent);
    createRow(-1, 0.0, Qt::partial_ordering::less);
    createRow(1, 0.0, Qt::partial_ordering::greater);
    createRow(0, -0.000001, Qt::partial_ordering::greater);
    createRow(0, 0.000001, Qt::partial_ordering::less);

    constexpr int max = std::numeric_limits<int>::max();
    constexpr int min = std::numeric_limits<int>::min();
    const double nan = qQNaN();
    createRow(0, nan, Qt::partial_ordering::unordered);
    createRow(max, nan, Qt::partial_ordering::unordered);
    createRow(min, nan, Qt::partial_ordering::unordered);

    const double inf = qInf();
    createRow(0, inf, Qt::partial_ordering::less);
    createRow(0, -inf, Qt::partial_ordering::greater);
    createRow(max, inf, Qt::partial_ordering::less);
    createRow(max, -inf, Qt::partial_ordering::greater);
    createRow(min, inf, Qt::partial_ordering::less);
    createRow(min, -inf, Qt::partial_ordering::greater);
}

#define DECLARE_TYPE(Name, Type, Attrs, RetType, Constexpr, Suffix) \
class Dummy ## Name \
{ \
public: \
    Constexpr Dummy ## Name () {} \
\
private: \
    friend Attrs Constexpr bool \
    comparesEqual(const Dummy ## Name &lhs, const Dummy ## Name &rhs) noexcept; \
    friend Attrs Constexpr RetType \
    compareThreeWay(const Dummy ## Name &lhs, const Dummy ## Name &rhs) noexcept; \
    friend Attrs Constexpr bool \
    comparesEqual(const Dummy ## Name &lhs, int rhs) noexcept; \
    friend Attrs Constexpr RetType \
    compareThreeWay(const Dummy ## Name &lhs, int rhs) noexcept; \
    Q_DECLARE_ ## Type ##_ORDERED ## Suffix (Dummy ## Name) \
    Q_DECLARE_ ## Type ##_ORDERED ## Suffix (Dummy ## Name, int) \
}; \
\
Attrs Constexpr bool comparesEqual(const Dummy ## Name &lhs, const Dummy ## Name &rhs) noexcept \
{ Q_UNUSED(lhs); Q_UNUSED(rhs); return true; } \
Attrs Constexpr RetType \
compareThreeWay(const Dummy ## Name &lhs, const Dummy ## Name &rhs) noexcept \
{ Q_UNUSED(lhs); Q_UNUSED(rhs); return RetType::equivalent; } \
Attrs Constexpr bool comparesEqual(const Dummy ## Name &lhs, int rhs) noexcept \
{ Q_UNUSED(lhs); Q_UNUSED(rhs); return true; } \
Attrs Constexpr RetType compareThreeWay(const Dummy ## Name &lhs, int rhs) noexcept \
{ Q_UNUSED(lhs); Q_UNUSED(rhs); return RetType::equivalent; }

DECLARE_TYPE(PartialConstAttr, PARTIALLY, Q_DECL_PURE_FUNCTION, Qt::partial_ordering, constexpr,
             _LITERAL_TYPE)
DECLARE_TYPE(PartialConst, PARTIALLY, /* no attrs */, Qt::partial_ordering, constexpr, _LITERAL_TYPE)
DECLARE_TYPE(PartialAttr, PARTIALLY, Q_DECL_CONST_FUNCTION, Qt::partial_ordering, , )
DECLARE_TYPE(Partial, PARTIALLY, /* no attrs */, Qt::partial_ordering, , )

DECLARE_TYPE(WeakConstAttr, WEAKLY, Q_DECL_PURE_FUNCTION, Qt::weak_ordering, constexpr, _LITERAL_TYPE)
DECLARE_TYPE(WeakConst, WEAKLY, /* no attrs */, Qt::weak_ordering, constexpr, _LITERAL_TYPE)
DECLARE_TYPE(WeakAttr, WEAKLY, Q_DECL_CONST_FUNCTION, Qt::weak_ordering, , )
DECLARE_TYPE(Weak, WEAKLY, /* no attrs */, Qt::weak_ordering, , )

DECLARE_TYPE(StrongConstAttr, STRONGLY, Q_DECL_PURE_FUNCTION, Qt::strong_ordering, constexpr,
             _LITERAL_TYPE)
DECLARE_TYPE(StrongConst, STRONGLY, /* no attrs */, Qt::strong_ordering, constexpr, _LITERAL_TYPE)
DECLARE_TYPE(StrongAttr, STRONGLY, Q_DECL_CONST_FUNCTION, Qt::strong_ordering, , )
DECLARE_TYPE(Strong, STRONGLY, /* no attrs */, Qt::strong_ordering, , )

#define DECLARE_EQUALITY_COMPARABLE(Name, Attrs, Constexpr, Suffix) \
class Dummy ## Name \
{ \
public: \
    Constexpr Dummy ## Name (int) {} \
\
private: \
    friend Attrs Constexpr bool \
    comparesEqual(const Dummy ## Name &lhs, const Dummy ## Name &rhs) noexcept; \
    friend Attrs Constexpr bool comparesEqual(const Dummy ## Name &lhs, int rhs) noexcept; \
    Q_DECLARE_EQUALITY_COMPARABLE ## Suffix (Dummy ## Name) \
    Q_DECLARE_EQUALITY_COMPARABLE ## Suffix (Dummy ## Name, int) \
}; \
\
Attrs Constexpr bool comparesEqual(const Dummy ## Name &lhs, const Dummy ## Name &rhs) noexcept \
{ Q_UNUSED(lhs); Q_UNUSED(rhs); return true; } \
Attrs Constexpr bool comparesEqual(const Dummy ## Name &lhs, int rhs) noexcept \
{ Q_UNUSED(lhs); Q_UNUSED(rhs); return true; } \

DECLARE_EQUALITY_COMPARABLE(ConstAttr, Q_DECL_PURE_FUNCTION, constexpr, _LITERAL_TYPE)
DECLARE_EQUALITY_COMPARABLE(Const, /* no attrs */, constexpr, _LITERAL_TYPE)
DECLARE_EQUALITY_COMPARABLE(Attr, Q_DECL_CONST_FUNCTION, , )
DECLARE_EQUALITY_COMPARABLE(None, /* no attrs */, , )

void tst_QCompareHelpers::generatedClasses()
{
#define COMPARE(ClassName) \
    do { \
        QTestPrivate::testAllComparisonOperatorsCompile<ClassName>(); \
        QTestPrivate::testAllComparisonOperatorsCompile<ClassName, int>(); \
    } while (0)

    COMPARE(DummyPartialConstAttr);
    COMPARE(DummyPartialConst);
    COMPARE(DummyPartialAttr);
    COMPARE(DummyPartial);

    COMPARE(DummyWeakConstAttr);
    COMPARE(DummyWeakConst);
    COMPARE(DummyWeakAttr);
    COMPARE(DummyWeak);

    COMPARE(DummyStrongConstAttr);
    COMPARE(DummyStrongConst);
    COMPARE(DummyStrongAttr);
    COMPARE(DummyStrong);
#undef COMPARE

    QTestPrivate::testEqualityOperatorsCompile<DummyConstAttr>();
    QTestPrivate::testEqualityOperatorsCompile<DummyConstAttr, int>();

    QTestPrivate::testEqualityOperatorsCompile<DummyConst>();
    QTestPrivate::testEqualityOperatorsCompile<DummyConst, int>();

    QTestPrivate::testEqualityOperatorsCompile<DummyAttr>();
    QTestPrivate::testEqualityOperatorsCompile<DummyAttr, int>();

    QTestPrivate::testEqualityOperatorsCompile<DummyNone>();
    QTestPrivate::testEqualityOperatorsCompile<DummyNone, int>();
}

template <typename LeftType, typename RightType,
          Qt::if_integral<LeftType, RightType> = true>
void testOrderForTypes()
{
    LeftType l0{0};
    LeftType l1{1};
    RightType r0{0};
    RightType r1{1};
    QCOMPARE_EQ(Qt::compareThreeWay(l0, r1), Qt::strong_ordering::less);
    QCOMPARE_EQ(Qt::compareThreeWay(l1, r0), Qt::strong_ordering::greater);
    QCOMPARE_EQ(Qt::compareThreeWay(l1, r1), Qt::strong_ordering::equivalent);
    // also swap types
    QCOMPARE_EQ(Qt::compareThreeWay(r1, l0), Qt::strong_ordering::greater);
    QCOMPARE_EQ(Qt::compareThreeWay(r0, l1), Qt::strong_ordering::less);
    QCOMPARE_EQ(Qt::compareThreeWay(r1, l1), Qt::strong_ordering::equivalent);

#ifdef __cpp_lib_three_way_comparison
    QCOMPARE_EQ(Qt::compareThreeWay(l0, r1), std::strong_ordering::less);
    QCOMPARE_EQ(Qt::compareThreeWay(l1, r0), std::strong_ordering::greater);
    QCOMPARE_EQ(Qt::compareThreeWay(l1, r1), std::strong_ordering::equivalent);

    QCOMPARE_EQ(Qt::compareThreeWay(r1, l0), std::strong_ordering::greater);
    QCOMPARE_EQ(Qt::compareThreeWay(r0, l1), std::strong_ordering::less);
    QCOMPARE_EQ(Qt::compareThreeWay(r1, l1), std::strong_ordering::equivalent);
#endif // __cpp_lib_three_way_comparison

    if constexpr (std::is_signed_v<LeftType>) {
        LeftType lm1{-1};
        QCOMPARE_EQ(Qt::compareThreeWay(lm1, r1), Qt::strong_ordering::less);
        QCOMPARE_EQ(Qt::compareThreeWay(r1, lm1), Qt::strong_ordering::greater);
#ifdef __cpp_lib_three_way_comparison
        QCOMPARE_EQ(Qt::compareThreeWay(lm1, r1), std::strong_ordering::less);
        QCOMPARE_EQ(Qt::compareThreeWay(r1, lm1), std::strong_ordering::greater);
#endif // __cpp_lib_three_way_comparison
    }
    if constexpr (std::is_signed_v<RightType>) {
        RightType rm1{-1};
        QCOMPARE_EQ(Qt::compareThreeWay(rm1, l1), Qt::strong_ordering::less);
        QCOMPARE_EQ(Qt::compareThreeWay(l1, rm1), Qt::strong_ordering::greater);
#ifdef __cpp_lib_three_way_comparison
        QCOMPARE_EQ(Qt::compareThreeWay(rm1, l1), std::strong_ordering::less);
        QCOMPARE_EQ(Qt::compareThreeWay(l1, rm1), std::strong_ordering::greater);
#endif // __cpp_lib_three_way_comparison
    }
}

template <typename LeftType, typename RightType,
          Qt::if_floating_point<LeftType, RightType> = true>
void testOrderForTypes()
{
    LeftType lNeg{-1.0};
    LeftType lPos{1.0};

    RightType rNeg{-1.0};
    RightType rPos{1.0};

    QCOMPARE_EQ(Qt::compareThreeWay(lNeg, rPos), Qt::partial_ordering::less);
    QCOMPARE_EQ(Qt::compareThreeWay(lPos, rNeg), Qt::partial_ordering::greater);
    QCOMPARE_EQ(Qt::compareThreeWay(rNeg, lPos), Qt::partial_ordering::less);
    QCOMPARE_EQ(Qt::compareThreeWay(rPos, lNeg), Qt::partial_ordering::greater);
    QCOMPARE_EQ(Qt::compareThreeWay(lNeg, rNeg), Qt::partial_ordering::equivalent);
    QCOMPARE_EQ(Qt::compareThreeWay(rNeg, lNeg), Qt::partial_ordering::equivalent);

    LeftType lNaN{std::numeric_limits<LeftType>::quiet_NaN()};
    LeftType lInf{std::numeric_limits<LeftType>::infinity()};

    RightType rNaN{std::numeric_limits<RightType>::quiet_NaN()};
    RightType rInf{std::numeric_limits<RightType>::infinity()};

    QCOMPARE_EQ(Qt::compareThreeWay(lNaN, rPos), Qt::partial_ordering::unordered);
    QCOMPARE_EQ(Qt::compareThreeWay(rNeg, lNaN), Qt::partial_ordering::unordered);
    QCOMPARE_EQ(Qt::compareThreeWay(lNeg, rNaN), Qt::partial_ordering::unordered);
    QCOMPARE_EQ(Qt::compareThreeWay(rNaN, lPos), Qt::partial_ordering::unordered);
    QCOMPARE_EQ(Qt::compareThreeWay(rNaN, lNaN), Qt::partial_ordering::unordered);
    QCOMPARE_EQ(Qt::compareThreeWay(lNaN, rNaN), Qt::partial_ordering::unordered);
    QCOMPARE_EQ(Qt::compareThreeWay(lNaN, rInf), Qt::partial_ordering::unordered);
    QCOMPARE_EQ(Qt::compareThreeWay(rNaN, -lInf), Qt::partial_ordering::unordered);

    QCOMPARE_EQ(Qt::compareThreeWay(lInf, rPos), Qt::partial_ordering::greater);
    QCOMPARE_EQ(Qt::compareThreeWay(rPos, lInf), Qt::partial_ordering::less);
    QCOMPARE_EQ(Qt::compareThreeWay(rInf, lNeg), Qt::partial_ordering::greater);
    QCOMPARE_EQ(Qt::compareThreeWay(lNeg, rInf), Qt::partial_ordering::less);
    QCOMPARE_EQ(Qt::compareThreeWay(lInf, -rInf), Qt::partial_ordering::greater);
    QCOMPARE_EQ(Qt::compareThreeWay(-lInf, rInf), Qt::partial_ordering::less);
    QCOMPARE_EQ(Qt::compareThreeWay(-rInf, lInf), Qt::partial_ordering::less);
    QCOMPARE_EQ(Qt::compareThreeWay(rInf, -lInf), Qt::partial_ordering::greater);

#ifdef __cpp_lib_three_way_comparison
    QCOMPARE_EQ(Qt::compareThreeWay(lNeg, rPos), std::partial_ordering::less);
    QCOMPARE_EQ(Qt::compareThreeWay(lPos, rNeg), std::partial_ordering::greater);
    QCOMPARE_EQ(Qt::compareThreeWay(rNeg, lPos), std::partial_ordering::less);
    QCOMPARE_EQ(Qt::compareThreeWay(rPos, lNeg), std::partial_ordering::greater);
    QCOMPARE_EQ(Qt::compareThreeWay(lNeg, rNeg), std::partial_ordering::equivalent);
    QCOMPARE_EQ(Qt::compareThreeWay(rNeg, lNeg), std::partial_ordering::equivalent);

    QCOMPARE_EQ(Qt::compareThreeWay(lNaN, rPos), std::partial_ordering::unordered);
    QCOMPARE_EQ(Qt::compareThreeWay(rNeg, lNaN), std::partial_ordering::unordered);
    QCOMPARE_EQ(Qt::compareThreeWay(lNeg, rNaN), std::partial_ordering::unordered);
    QCOMPARE_EQ(Qt::compareThreeWay(rNaN, lPos), std::partial_ordering::unordered);
    QCOMPARE_EQ(Qt::compareThreeWay(rNaN, lNaN), std::partial_ordering::unordered);
    QCOMPARE_EQ(Qt::compareThreeWay(lNaN, rNaN), std::partial_ordering::unordered);
    QCOMPARE_EQ(Qt::compareThreeWay(lNaN, rInf), std::partial_ordering::unordered);
    QCOMPARE_EQ(Qt::compareThreeWay(rNaN, -lInf), std::partial_ordering::unordered);

    QCOMPARE_EQ(Qt::compareThreeWay(lInf, rPos), std::partial_ordering::greater);
    QCOMPARE_EQ(Qt::compareThreeWay(rPos, lInf), std::partial_ordering::less);
    QCOMPARE_EQ(Qt::compareThreeWay(rInf, lNeg), std::partial_ordering::greater);
    QCOMPARE_EQ(Qt::compareThreeWay(lNeg, rInf), std::partial_ordering::less);
    QCOMPARE_EQ(Qt::compareThreeWay(lInf, -rInf), std::partial_ordering::greater);
    QCOMPARE_EQ(Qt::compareThreeWay(-lInf, rInf), std::partial_ordering::less);
    QCOMPARE_EQ(Qt::compareThreeWay(-rInf, lInf), std::partial_ordering::less);
    QCOMPARE_EQ(Qt::compareThreeWay(rInf, -lInf), std::partial_ordering::greater);
#endif // __cpp_lib_three_way_comparison
}

template <typename IntType, typename FloatType,
          Qt::if_integral_and_floating_point<IntType, FloatType> = true>
void testOrderForTypes()
{
    IntType l0{0};
    IntType l1{1};

    FloatType r0{0.0};
    FloatType r1{1.0};
    FloatType rNaN{std::numeric_limits<FloatType>::quiet_NaN()};

    QCOMPARE_EQ(Qt::compareThreeWay(l0, r1), Qt::partial_ordering::less);
    QCOMPARE_EQ(Qt::compareThreeWay(l1, r0), Qt::partial_ordering::greater);
    QCOMPARE_EQ(Qt::compareThreeWay(r1, l0), Qt::partial_ordering::greater);
    QCOMPARE_EQ(Qt::compareThreeWay(r0, l1), Qt::partial_ordering::less);
    QCOMPARE_EQ(Qt::compareThreeWay(l0, r0), Qt::partial_ordering::equivalent);
    QCOMPARE_EQ(Qt::compareThreeWay(r0, l0), Qt::partial_ordering::equivalent);
    QCOMPARE_EQ(Qt::compareThreeWay(l0, rNaN), Qt::partial_ordering::unordered);
    QCOMPARE_EQ(Qt::compareThreeWay(rNaN, l1), Qt::partial_ordering::unordered);
#ifdef __cpp_lib_three_way_comparison
    QCOMPARE_EQ(Qt::compareThreeWay(l0, r1), std::partial_ordering::less);
    QCOMPARE_EQ(Qt::compareThreeWay(l1, r0), std::partial_ordering::greater);
    QCOMPARE_EQ(Qt::compareThreeWay(r1, l0), std::partial_ordering::greater);
    QCOMPARE_EQ(Qt::compareThreeWay(r0, l1), std::partial_ordering::less);
    QCOMPARE_EQ(Qt::compareThreeWay(l0, r0), std::partial_ordering::equivalent);
    QCOMPARE_EQ(Qt::compareThreeWay(r0, l0), std::partial_ordering::equivalent);
    QCOMPARE_EQ(Qt::compareThreeWay(l0, rNaN), std::partial_ordering::unordered);
    QCOMPARE_EQ(Qt::compareThreeWay(rNaN, l1), std::partial_ordering::unordered);
#endif // __cpp_lib_three_way_comparison
}

enum class TestEnum : quint8 {
    Smaller,
    Bigger
};

void tst_QCompareHelpers::builtinOrder()
{
#define TEST_BUILTIN(Left, Right) \
    testOrderForTypes<Left, Right>(); \
    if (QTest::currentTestFailed()) { \
        qDebug("Failed Qt::compareThreeWay() test for builtin types " #Left " and " #Right); \
        return; \
    }

    // some combinations
    TEST_BUILTIN(char, char)
#if CHAR_MIN < 0
    TEST_BUILTIN(char, short)
    TEST_BUILTIN(qint8, char)
#else
    TEST_BUILTIN(char, ushort)
    TEST_BUILTIN(quint8, char)
#endif
    TEST_BUILTIN(qint8, qint8)
    TEST_BUILTIN(qint8, int)
    TEST_BUILTIN(ulong, quint8)
    TEST_BUILTIN(ushort, uchar)
    TEST_BUILTIN(int, int)
    TEST_BUILTIN(uint, ulong)
    TEST_BUILTIN(long, int)
    TEST_BUILTIN(uint, quint64)
    TEST_BUILTIN(qint64, short)
    TEST_BUILTIN(wchar_t, wchar_t)
    TEST_BUILTIN(uint, char16_t)
    TEST_BUILTIN(char32_t, char32_t)
    TEST_BUILTIN(char32_t, ushort)
#ifdef __cpp_char8_t
    TEST_BUILTIN(char8_t, char8_t)
    TEST_BUILTIN(char8_t, ushort)
    TEST_BUILTIN(char8_t, uint)
    TEST_BUILTIN(char8_t, quint64)
#endif // __cpp_char8_t
#ifdef QT_SUPPORTS_INT128
    TEST_BUILTIN(qint128, qint128)
    TEST_BUILTIN(quint128, quint128)
    TEST_BUILTIN(qint128, int)
    TEST_BUILTIN(ushort, quint128)
#endif
    TEST_BUILTIN(float, double)
    TEST_BUILTIN(double, float)
    TEST_BUILTIN(quint64, float)
    TEST_BUILTIN(qint64, double)
#ifdef __STDCPP_FLOAT16_T__
    TEST_BUILTIN(std::float16_t, std::float16_t)
    TEST_BUILTIN(std::float16_t, double)
    TEST_BUILTIN(qint64, std::float16_t)
    TEST_BUILTIN(uint, std::float16_t)
#endif

    TEST_BUILTIN(long double, long double)
    TEST_BUILTIN(float, long double)
    TEST_BUILTIN(double, long double)
    TEST_BUILTIN(quint64, long double)
    TEST_BUILTIN(ushort, long double)

    QCOMPARE_EQ(Qt::compareThreeWay(TestEnum::Smaller, TestEnum::Bigger),
                Qt::strong_ordering::less);
    QCOMPARE_EQ(Qt::compareThreeWay(TestEnum::Bigger, TestEnum::Smaller),
                Qt::strong_ordering::greater);
    QCOMPARE_EQ(Qt::compareThreeWay(TestEnum::Smaller, TestEnum::Smaller),
                Qt::strong_ordering::equivalent);

    std::array<int, 2> arr{1, 0};
    QCOMPARE_EQ(Qt::compareThreeWay(&arr[0], &arr[1]), Qt::strong_ordering::less);
    QCOMPARE_EQ(Qt::compareThreeWay(arr.data(), &arr[0]), Qt::strong_ordering::equivalent);

    class Base {};
    class Derived : public Base {};

    auto b = std::make_unique<Base>();
    auto d = std::make_unique<Derived>();
    QCOMPARE_NE(Qt::compareThreeWay(b.get(), d.get()), Qt::strong_ordering::equivalent);
    QCOMPARE_EQ(Qt::compareThreeWay(b.get(), nullptr), Qt::strong_ordering::greater);
    QCOMPARE_EQ(Qt::compareThreeWay(nullptr, d.get()), Qt::strong_ordering::less);

#undef TEST_BUILTIN
}

QTEST_MAIN(tst_QCompareHelpers)
#include "tst_qcomparehelpers.moc"
