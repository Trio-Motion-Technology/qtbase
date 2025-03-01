// Copyright (C) 2020 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Giuseppe D'Angelo <giuseppe.dangelo@kdab.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtCore/QtCompare>
#include <QtTest/QTest>

#ifdef __cpp_lib_three_way_comparison
#include <compare>
#endif // __cpp_lib_three_way_comparison

class tst_QCompare: public QObject
{
    Q_OBJECT
private slots:
    void legacyPartialOrdering();
    void stdQtBinaryCompatibility();
    void partialOrdering();
    void weakOrdering();
    void strongOrdering();
    void conversions();
    void is_eq_overloads();
};

void tst_QCompare::legacyPartialOrdering()
{
    static_assert(QPartialOrdering::Unordered == QPartialOrdering::Unordered);
    static_assert(QPartialOrdering::Unordered != QPartialOrdering::Less);
    static_assert(QPartialOrdering::Unordered != QPartialOrdering::Equivalent);
    static_assert(QPartialOrdering::Unordered != QPartialOrdering::Greater);

    static_assert(QPartialOrdering::Less != QPartialOrdering::Unordered);
    static_assert(QPartialOrdering::Less == QPartialOrdering::Less);
    static_assert(QPartialOrdering::Less != QPartialOrdering::Equivalent);
    static_assert(QPartialOrdering::Less != QPartialOrdering::Greater);

    static_assert(QPartialOrdering::Equivalent != QPartialOrdering::Unordered);
    static_assert(QPartialOrdering::Equivalent != QPartialOrdering::Less);
    static_assert(QPartialOrdering::Equivalent == QPartialOrdering::Equivalent);
    static_assert(QPartialOrdering::Equivalent != QPartialOrdering::Greater);

    static_assert(QPartialOrdering::Greater != QPartialOrdering::Unordered);
    static_assert(QPartialOrdering::Greater != QPartialOrdering::Less);
    static_assert(QPartialOrdering::Greater != QPartialOrdering::Equivalent);
    static_assert(QPartialOrdering::Greater == QPartialOrdering::Greater);

    static_assert(!is_eq  (QPartialOrdering::Unordered));
    static_assert(!is_neq (QPartialOrdering::Unordered));
    static_assert(!is_lt  (QPartialOrdering::Unordered));
    static_assert(!is_lteq(QPartialOrdering::Unordered));
    static_assert(!is_gt  (QPartialOrdering::Unordered));
    static_assert(!is_gteq(QPartialOrdering::Unordered));

    static_assert(!(QPartialOrdering::Unordered == 0));
    static_assert(!(QPartialOrdering::Unordered != 0));
    static_assert(!(QPartialOrdering::Unordered <  0));
    static_assert(!(QPartialOrdering::Unordered <= 0));
    static_assert(!(QPartialOrdering::Unordered >  0));
    static_assert(!(QPartialOrdering::Unordered >= 0));

    static_assert(!(0 == QPartialOrdering::Unordered));
    static_assert(!(0 != QPartialOrdering::Unordered));
    static_assert(!(0 <  QPartialOrdering::Unordered));
    static_assert(!(0 <= QPartialOrdering::Unordered));
    static_assert(!(0 >  QPartialOrdering::Unordered));
    static_assert(!(0 >= QPartialOrdering::Unordered));


    static_assert(!is_eq  (QPartialOrdering::Less));
    static_assert( is_neq (QPartialOrdering::Less));
    static_assert( is_lt  (QPartialOrdering::Less));
    static_assert( is_lteq(QPartialOrdering::Less));
    static_assert(!is_gt  (QPartialOrdering::Less));
    static_assert(!is_gteq(QPartialOrdering::Less));

    static_assert(!(QPartialOrdering::Less == 0));
    static_assert( (QPartialOrdering::Less != 0));
    static_assert( (QPartialOrdering::Less <  0));
    static_assert( (QPartialOrdering::Less <= 0));
    static_assert(!(QPartialOrdering::Less >  0));
    static_assert(!(QPartialOrdering::Less >= 0));

    static_assert(!(0 == QPartialOrdering::Less));
    static_assert( (0 != QPartialOrdering::Less));
    static_assert(!(0 <  QPartialOrdering::Less));
    static_assert(!(0 <= QPartialOrdering::Less));
    static_assert( (0 >  QPartialOrdering::Less));
    static_assert( (0 >= QPartialOrdering::Less));


    static_assert( is_eq  (QPartialOrdering::Equivalent));
    static_assert(!is_neq (QPartialOrdering::Equivalent));
    static_assert(!is_lt  (QPartialOrdering::Equivalent));
    static_assert( is_lteq(QPartialOrdering::Equivalent));
    static_assert(!is_gt  (QPartialOrdering::Equivalent));
    static_assert( is_gteq(QPartialOrdering::Equivalent));

    static_assert( (QPartialOrdering::Equivalent == 0));
    static_assert(!(QPartialOrdering::Equivalent != 0));
    static_assert(!(QPartialOrdering::Equivalent <  0));
    static_assert( (QPartialOrdering::Equivalent <= 0));
    static_assert(!(QPartialOrdering::Equivalent >  0));
    static_assert( (QPartialOrdering::Equivalent >= 0));

    static_assert( (0 == QPartialOrdering::Equivalent));
    static_assert(!(0 != QPartialOrdering::Equivalent));
    static_assert(!(0 <  QPartialOrdering::Equivalent));
    static_assert( (0 <= QPartialOrdering::Equivalent));
    static_assert(!(0 >  QPartialOrdering::Equivalent));
    static_assert( (0 >= QPartialOrdering::Equivalent));


    static_assert(!is_eq  (QPartialOrdering::Greater));
    static_assert( is_neq (QPartialOrdering::Greater));
    static_assert(!is_lt  (QPartialOrdering::Greater));
    static_assert(!is_lteq(QPartialOrdering::Greater));
    static_assert( is_gt  (QPartialOrdering::Greater));
    static_assert( is_gteq(QPartialOrdering::Greater));

    static_assert(!(QPartialOrdering::Greater == 0));
    static_assert( (QPartialOrdering::Greater != 0));
    static_assert(!(QPartialOrdering::Greater <  0));
    static_assert(!(QPartialOrdering::Greater <= 0));
    static_assert( (QPartialOrdering::Greater >  0));
    static_assert( (QPartialOrdering::Greater >= 0));

    static_assert(!(0 == QPartialOrdering::Greater));
    static_assert( (0 != QPartialOrdering::Greater));
    static_assert( (0 <  QPartialOrdering::Greater));
    static_assert( (0 <= QPartialOrdering::Greater));
    static_assert(!(0 >  QPartialOrdering::Greater));
    static_assert(!(0 >= QPartialOrdering::Greater));
}

void tst_QCompare::stdQtBinaryCompatibility()
{
#ifndef __cpp_lib_three_way_comparison
    QSKIP("This test requires C++20 three-way-comparison support enabled in the stdlib.");
#else
    QCOMPARE_EQ(sizeof(std::partial_ordering), 1U);
    QCOMPARE_EQ(sizeof( Qt::partial_ordering), 1U);
    QCOMPARE_EQ(sizeof(std::   weak_ordering), 1U);
    QCOMPARE_EQ(sizeof( Qt::   weak_ordering), 1U);
    QCOMPARE_EQ(sizeof(std:: strong_ordering), 1U);
    QCOMPARE_EQ(sizeof( Qt:: strong_ordering), 1U);

    auto valueOf = [](auto obj) {
        typename QIntegerForSizeof<decltype(obj)>::Unsigned value;
        memcpy(&value, &obj, sizeof(obj));
        return value;
    };
#define CHECK(type, flag) \
        QCOMPARE_EQ(valueOf( Qt:: type ## _ordering :: flag), \
                    valueOf(std:: type ## _ordering :: flag)) \
        /* end */
    CHECK(partial, unordered);
    CHECK(partial, less);
    CHECK(partial, greater);
    CHECK(partial, equivalent);

    CHECK(weak, less);
    CHECK(weak, greater);
    CHECK(weak, equivalent);

    CHECK(strong, less);
    CHECK(strong, greater);
    CHECK(strong, equivalent);
    CHECK(strong, equal);
#undef CHECK
#endif //__cpp_lib_three_way_comparison
}

void tst_QCompare::partialOrdering()
{
    static_assert(Qt::partial_ordering::unordered == Qt::partial_ordering::unordered);
    static_assert(Qt::partial_ordering::unordered != Qt::partial_ordering::less);
    static_assert(Qt::partial_ordering::unordered != Qt::partial_ordering::equivalent);
    static_assert(Qt::partial_ordering::unordered != Qt::partial_ordering::greater);

    static_assert(Qt::partial_ordering::less != Qt::partial_ordering::unordered);
    static_assert(Qt::partial_ordering::less == Qt::partial_ordering::less);
    static_assert(Qt::partial_ordering::less != Qt::partial_ordering::equivalent);
    static_assert(Qt::partial_ordering::less != Qt::partial_ordering::greater);

    static_assert(Qt::partial_ordering::equivalent != Qt::partial_ordering::unordered);
    static_assert(Qt::partial_ordering::equivalent != Qt::partial_ordering::less);
    static_assert(Qt::partial_ordering::equivalent == Qt::partial_ordering::equivalent);
    static_assert(Qt::partial_ordering::equivalent != Qt::partial_ordering::greater);

    static_assert(Qt::partial_ordering::greater != Qt::partial_ordering::unordered);
    static_assert(Qt::partial_ordering::greater != Qt::partial_ordering::less);
    static_assert(Qt::partial_ordering::greater != Qt::partial_ordering::equivalent);
    static_assert(Qt::partial_ordering::greater == Qt::partial_ordering::greater);

    static_assert(!is_eq  (Qt::partial_ordering::unordered));
    static_assert(!is_neq (Qt::partial_ordering::unordered));
    static_assert(!is_lt  (Qt::partial_ordering::unordered));
    static_assert(!is_lteq(Qt::partial_ordering::unordered));
    static_assert(!is_gt  (Qt::partial_ordering::unordered));
    static_assert(!is_gteq(Qt::partial_ordering::unordered));

    static_assert(!(Qt::partial_ordering::unordered == 0));
    static_assert(!(Qt::partial_ordering::unordered != 0));
    static_assert(!(Qt::partial_ordering::unordered <  0));
    static_assert(!(Qt::partial_ordering::unordered <= 0));
    static_assert(!(Qt::partial_ordering::unordered >  0));
    static_assert(!(Qt::partial_ordering::unordered >= 0));

    static_assert(!(0 == Qt::partial_ordering::unordered));
    static_assert(!(0 != Qt::partial_ordering::unordered));
    static_assert(!(0 <  Qt::partial_ordering::unordered));
    static_assert(!(0 <= Qt::partial_ordering::unordered));
    static_assert(!(0 >  Qt::partial_ordering::unordered));
    static_assert(!(0 >= Qt::partial_ordering::unordered));


    static_assert(!is_eq  (Qt::partial_ordering::less));
    static_assert( is_neq (Qt::partial_ordering::less));
    static_assert( is_lt  (Qt::partial_ordering::less));
    static_assert( is_lteq(Qt::partial_ordering::less));
    static_assert(!is_gt  (Qt::partial_ordering::less));
    static_assert(!is_gteq(Qt::partial_ordering::less));

    static_assert(!(Qt::partial_ordering::less == 0));
    static_assert( (Qt::partial_ordering::less != 0));
    static_assert( (Qt::partial_ordering::less <  0));
    static_assert( (Qt::partial_ordering::less <= 0));
    static_assert(!(Qt::partial_ordering::less >  0));
    static_assert(!(Qt::partial_ordering::less >= 0));

    static_assert(!(0 == Qt::partial_ordering::less));
    static_assert( (0 != Qt::partial_ordering::less));
    static_assert(!(0 <  Qt::partial_ordering::less));
    static_assert(!(0 <= Qt::partial_ordering::less));
    static_assert( (0 >  Qt::partial_ordering::less));
    static_assert( (0 >= Qt::partial_ordering::less));


    static_assert( is_eq  (Qt::partial_ordering::equivalent));
    static_assert(!is_neq (Qt::partial_ordering::equivalent));
    static_assert(!is_lt  (Qt::partial_ordering::equivalent));
    static_assert( is_lteq(Qt::partial_ordering::equivalent));
    static_assert(!is_gt  (Qt::partial_ordering::equivalent));
    static_assert( is_gteq(Qt::partial_ordering::equivalent));

    static_assert( (Qt::partial_ordering::equivalent == 0));
    static_assert(!(Qt::partial_ordering::equivalent != 0));
    static_assert(!(Qt::partial_ordering::equivalent <  0));
    static_assert( (Qt::partial_ordering::equivalent <= 0));
    static_assert(!(Qt::partial_ordering::equivalent >  0));
    static_assert( (Qt::partial_ordering::equivalent >= 0));

    static_assert( (0 == Qt::partial_ordering::equivalent));
    static_assert(!(0 != Qt::partial_ordering::equivalent));
    static_assert(!(0 <  Qt::partial_ordering::equivalent));
    static_assert( (0 <= Qt::partial_ordering::equivalent));
    static_assert(!(0 >  Qt::partial_ordering::equivalent));
    static_assert( (0 >= Qt::partial_ordering::equivalent));


    static_assert(!is_eq  (Qt::partial_ordering::greater));
    static_assert( is_neq (Qt::partial_ordering::greater));
    static_assert(!is_lt  (Qt::partial_ordering::greater));
    static_assert(!is_lteq(Qt::partial_ordering::greater));
    static_assert( is_gt  (Qt::partial_ordering::greater));
    static_assert( is_gteq(Qt::partial_ordering::greater));

    static_assert(!(Qt::partial_ordering::greater == 0));
    static_assert( (Qt::partial_ordering::greater != 0));
    static_assert(!(Qt::partial_ordering::greater <  0));
    static_assert(!(Qt::partial_ordering::greater <= 0));
    static_assert( (Qt::partial_ordering::greater >  0));
    static_assert( (Qt::partial_ordering::greater >= 0));

    static_assert(!(0 == Qt::partial_ordering::greater));
    static_assert( (0 != Qt::partial_ordering::greater));
    static_assert( (0 <  Qt::partial_ordering::greater));
    static_assert( (0 <= Qt::partial_ordering::greater));
    static_assert(!(0 >  Qt::partial_ordering::greater));
    static_assert(!(0 >= Qt::partial_ordering::greater));
}

void tst_QCompare::weakOrdering()
{
    static_assert(Qt::weak_ordering::less == Qt::weak_ordering::less);
    static_assert(Qt::weak_ordering::less != Qt::weak_ordering::equivalent);
    static_assert(Qt::weak_ordering::less != Qt::weak_ordering::greater);

    static_assert(Qt::weak_ordering::equivalent != Qt::weak_ordering::less);
    static_assert(Qt::weak_ordering::equivalent == Qt::weak_ordering::equivalent);
    static_assert(Qt::weak_ordering::equivalent != Qt::weak_ordering::greater);

    static_assert(Qt::weak_ordering::greater != Qt::weak_ordering::less);
    static_assert(Qt::weak_ordering::greater != Qt::weak_ordering::equivalent);
    static_assert(Qt::weak_ordering::greater == Qt::weak_ordering::greater);

    static_assert(!is_eq  (Qt::weak_ordering::less));
    static_assert( is_neq (Qt::weak_ordering::less));
    static_assert( is_lt  (Qt::weak_ordering::less));
    static_assert( is_lteq(Qt::weak_ordering::less));
    static_assert(!is_gt  (Qt::weak_ordering::less));
    static_assert(!is_gteq(Qt::weak_ordering::less));

    static_assert(!(Qt::weak_ordering::less == 0));
    static_assert( (Qt::weak_ordering::less != 0));
    static_assert( (Qt::weak_ordering::less <  0));
    static_assert( (Qt::weak_ordering::less <= 0));
    static_assert(!(Qt::weak_ordering::less >  0));
    static_assert(!(Qt::weak_ordering::less >= 0));

    static_assert(!(0 == Qt::weak_ordering::less));
    static_assert( (0 != Qt::weak_ordering::less));
    static_assert(!(0 <  Qt::weak_ordering::less));
    static_assert(!(0 <= Qt::weak_ordering::less));
    static_assert( (0 >  Qt::weak_ordering::less));
    static_assert( (0 >= Qt::weak_ordering::less));


    static_assert( is_eq  (Qt::weak_ordering::equivalent));
    static_assert(!is_neq (Qt::weak_ordering::equivalent));
    static_assert(!is_lt  (Qt::weak_ordering::equivalent));
    static_assert( is_lteq(Qt::weak_ordering::equivalent));
    static_assert(!is_gt  (Qt::weak_ordering::equivalent));
    static_assert( is_gteq(Qt::weak_ordering::equivalent));

    static_assert( (Qt::weak_ordering::equivalent == 0));
    static_assert(!(Qt::weak_ordering::equivalent != 0));
    static_assert(!(Qt::weak_ordering::equivalent <  0));
    static_assert( (Qt::weak_ordering::equivalent <= 0));
    static_assert(!(Qt::weak_ordering::equivalent >  0));
    static_assert( (Qt::weak_ordering::equivalent >= 0));

    static_assert( (0 == Qt::weak_ordering::equivalent));
    static_assert(!(0 != Qt::weak_ordering::equivalent));
    static_assert(!(0 <  Qt::weak_ordering::equivalent));
    static_assert( (0 <= Qt::weak_ordering::equivalent));
    static_assert(!(0 >  Qt::weak_ordering::equivalent));
    static_assert( (0 >= Qt::weak_ordering::equivalent));


    static_assert(!is_eq  (Qt::weak_ordering::greater));
    static_assert( is_neq (Qt::weak_ordering::greater));
    static_assert(!is_lt  (Qt::weak_ordering::greater));
    static_assert(!is_lteq(Qt::weak_ordering::greater));
    static_assert( is_gt  (Qt::weak_ordering::greater));
    static_assert( is_gteq(Qt::weak_ordering::greater));

    static_assert(!(Qt::weak_ordering::greater == 0));
    static_assert( (Qt::weak_ordering::greater != 0));
    static_assert(!(Qt::weak_ordering::greater <  0));
    static_assert(!(Qt::weak_ordering::greater <= 0));
    static_assert( (Qt::weak_ordering::greater >  0));
    static_assert( (Qt::weak_ordering::greater >= 0));

    static_assert(!(0 == Qt::weak_ordering::greater));
    static_assert( (0 != Qt::weak_ordering::greater));
    static_assert( (0 <  Qt::weak_ordering::greater));
    static_assert( (0 <= Qt::weak_ordering::greater));
    static_assert(!(0 >  Qt::weak_ordering::greater));
    static_assert(!(0 >= Qt::weak_ordering::greater));
}

void tst_QCompare::strongOrdering()
{
    static_assert(Qt::strong_ordering::less == Qt::strong_ordering::less);
    static_assert(Qt::strong_ordering::less != Qt::strong_ordering::equal);
    static_assert(Qt::strong_ordering::less != Qt::strong_ordering::equivalent);
    static_assert(Qt::strong_ordering::less != Qt::strong_ordering::greater);

    static_assert(Qt::strong_ordering::equal != Qt::strong_ordering::less);
    static_assert(Qt::strong_ordering::equal == Qt::strong_ordering::equal);
    static_assert(Qt::strong_ordering::equal == Qt::strong_ordering::equivalent);
    static_assert(Qt::strong_ordering::equal != Qt::strong_ordering::greater);

    static_assert(Qt::strong_ordering::equivalent != Qt::strong_ordering::less);
    static_assert(Qt::strong_ordering::equivalent == Qt::strong_ordering::equal);
    static_assert(Qt::strong_ordering::equivalent == Qt::strong_ordering::equivalent);
    static_assert(Qt::strong_ordering::equivalent != Qt::strong_ordering::greater);

    static_assert(Qt::strong_ordering::greater != Qt::strong_ordering::less);
    static_assert(Qt::strong_ordering::greater != Qt::strong_ordering::equal);
    static_assert(Qt::strong_ordering::greater != Qt::strong_ordering::equivalent);
    static_assert(Qt::strong_ordering::greater == Qt::strong_ordering::greater);

    static_assert(!is_eq  (Qt::strong_ordering::less));
    static_assert( is_neq (Qt::strong_ordering::less));
    static_assert( is_lt  (Qt::strong_ordering::less));
    static_assert( is_lteq(Qt::strong_ordering::less));
    static_assert(!is_gt  (Qt::strong_ordering::less));
    static_assert(!is_gteq(Qt::strong_ordering::less));

    static_assert(!(Qt::strong_ordering::less == 0));
    static_assert( (Qt::strong_ordering::less != 0));
    static_assert( (Qt::strong_ordering::less <  0));
    static_assert( (Qt::strong_ordering::less <= 0));
    static_assert(!(Qt::strong_ordering::less >  0));
    static_assert(!(Qt::strong_ordering::less >= 0));

    static_assert(!(0 == Qt::strong_ordering::less));
    static_assert( (0 != Qt::strong_ordering::less));
    static_assert(!(0 <  Qt::strong_ordering::less));
    static_assert(!(0 <= Qt::strong_ordering::less));
    static_assert( (0 >  Qt::strong_ordering::less));
    static_assert( (0 >= Qt::strong_ordering::less));


    static_assert( is_eq  (Qt::strong_ordering::equal));
    static_assert(!is_neq (Qt::strong_ordering::equal));
    static_assert(!is_lt  (Qt::strong_ordering::equal));
    static_assert( is_lteq(Qt::strong_ordering::equal));
    static_assert(!is_gt  (Qt::strong_ordering::equal));
    static_assert( is_gteq(Qt::strong_ordering::equal));

    static_assert( (Qt::strong_ordering::equal == 0));
    static_assert(!(Qt::strong_ordering::equal != 0));
    static_assert(!(Qt::strong_ordering::equal <  0));
    static_assert( (Qt::strong_ordering::equal <= 0));
    static_assert(!(Qt::strong_ordering::equal >  0));
    static_assert( (Qt::strong_ordering::equal >= 0));

    static_assert( (0 == Qt::strong_ordering::equal));
    static_assert(!(0 != Qt::strong_ordering::equal));
    static_assert(!(0 <  Qt::strong_ordering::equal));
    static_assert( (0 <= Qt::strong_ordering::equal));
    static_assert(!(0 >  Qt::strong_ordering::equal));
    static_assert( (0 >= Qt::strong_ordering::equal));


    static_assert( is_eq  (Qt::strong_ordering::equivalent));
    static_assert(!is_neq (Qt::strong_ordering::equivalent));
    static_assert(!is_lt  (Qt::strong_ordering::equivalent));
    static_assert( is_lteq(Qt::strong_ordering::equivalent));
    static_assert(!is_gt  (Qt::strong_ordering::equivalent));
    static_assert( is_gteq(Qt::strong_ordering::equivalent));

    static_assert( (Qt::strong_ordering::equivalent == 0));
    static_assert(!(Qt::strong_ordering::equivalent != 0));
    static_assert(!(Qt::strong_ordering::equivalent <  0));
    static_assert( (Qt::strong_ordering::equivalent <= 0));
    static_assert(!(Qt::strong_ordering::equivalent >  0));
    static_assert( (Qt::strong_ordering::equivalent >= 0));

    static_assert( (0 == Qt::strong_ordering::equivalent));
    static_assert(!(0 != Qt::strong_ordering::equivalent));
    static_assert(!(0 <  Qt::strong_ordering::equivalent));
    static_assert( (0 <= Qt::strong_ordering::equivalent));
    static_assert(!(0 >  Qt::strong_ordering::equivalent));
    static_assert( (0 >= Qt::strong_ordering::equivalent));


    static_assert(!is_eq  (Qt::strong_ordering::greater));
    static_assert( is_neq (Qt::strong_ordering::greater));
    static_assert(!is_lt  (Qt::strong_ordering::greater));
    static_assert(!is_lteq(Qt::strong_ordering::greater));
    static_assert( is_gt  (Qt::strong_ordering::greater));
    static_assert( is_gteq(Qt::strong_ordering::greater));

    static_assert(!(Qt::strong_ordering::greater == 0));
    static_assert( (Qt::strong_ordering::greater != 0));
    static_assert(!(Qt::strong_ordering::greater <  0));
    static_assert(!(Qt::strong_ordering::greater <= 0));
    static_assert( (Qt::strong_ordering::greater >  0));
    static_assert( (Qt::strong_ordering::greater >= 0));

    static_assert(!(0 == Qt::strong_ordering::greater));
    static_assert( (0 != Qt::strong_ordering::greater));
    static_assert( (0 <  Qt::strong_ordering::greater));
    static_assert( (0 <= Qt::strong_ordering::greater));
    static_assert(!(0 >  Qt::strong_ordering::greater));
    static_assert(!(0 >= Qt::strong_ordering::greater));
}

void tst_QCompare::conversions()
{
    // Qt::weak_ordering -> Qt::partial_ordering
    {
        constexpr Qt::partial_ordering less = Qt::weak_ordering::less;
        static_assert(less == Qt::partial_ordering::less);
        constexpr Qt::partial_ordering equivalent = Qt::weak_ordering::equivalent;
        static_assert(equivalent == Qt::partial_ordering::equivalent);
        constexpr Qt::partial_ordering greater = Qt::weak_ordering::greater;
        static_assert(greater == Qt::partial_ordering::greater);
    }
    // Qt::strong_ordering -> Qt::partial_ordering
    {
        constexpr Qt::partial_ordering less = Qt::strong_ordering::less;
        static_assert(less == Qt::partial_ordering::less);
        constexpr Qt::partial_ordering equal = Qt::strong_ordering::equal;
        static_assert(equal == Qt::partial_ordering::equivalent);
        constexpr Qt::partial_ordering equivalent = Qt::strong_ordering::equivalent;
        static_assert(equivalent == Qt::partial_ordering::equivalent);
        constexpr Qt::partial_ordering greater = Qt::strong_ordering::greater;
        static_assert(greater == Qt::partial_ordering::greater);
    }
    // Qt::strong_ordering -> Qt::weak_ordering
    {
        constexpr Qt::weak_ordering less = Qt::strong_ordering::less;
        static_assert(less == Qt::weak_ordering::less);
        constexpr Qt::weak_ordering equal = Qt::strong_ordering::equal;
        static_assert(equal == Qt::weak_ordering::equivalent);
        constexpr Qt::weak_ordering equivalent = Qt::strong_ordering::equivalent;
        static_assert(equivalent == Qt::weak_ordering::equivalent);
        constexpr Qt::weak_ordering greater = Qt::strong_ordering::greater;
        static_assert(greater == Qt::weak_ordering::greater);
    }
    // Mixed types
    {
        static_assert(Qt::partial_ordering::less == Qt::strong_ordering::less);
        static_assert(Qt::partial_ordering::equivalent != Qt::strong_ordering::less);
        static_assert(Qt::partial_ordering::equivalent == Qt::strong_ordering::equal);
        static_assert(Qt::partial_ordering::greater == Qt::strong_ordering::greater);

        static_assert(Qt::partial_ordering::less == Qt::weak_ordering::less);
        static_assert(Qt::partial_ordering::equivalent == Qt::weak_ordering::equivalent);
        static_assert(Qt::partial_ordering::greater == Qt::weak_ordering::greater);

        static_assert(Qt::weak_ordering::less == Qt::strong_ordering::less);
        static_assert(Qt::weak_ordering::equivalent != Qt::strong_ordering::greater);
        static_assert(Qt::weak_ordering::equivalent == Qt::strong_ordering::equal);
        static_assert(Qt::weak_ordering::greater == Qt::strong_ordering::greater);

        static_assert(Qt::weak_ordering::less == Qt::partial_ordering::less);
        static_assert(Qt::weak_ordering::equivalent == Qt::partial_ordering::equivalent);
        static_assert(Qt::weak_ordering::greater == Qt::partial_ordering::greater);

        static_assert(Qt::strong_ordering::less == Qt::partial_ordering::less);
        static_assert(Qt::strong_ordering::equivalent == Qt::partial_ordering::equivalent);
        static_assert(Qt::strong_ordering::equal == Qt::partial_ordering::equivalent);
        static_assert(Qt::strong_ordering::greater == Qt::partial_ordering::greater);

        static_assert(Qt::strong_ordering::less == Qt::weak_ordering::less);
        static_assert(Qt::strong_ordering::equivalent == Qt::weak_ordering::equivalent);
        static_assert(Qt::strong_ordering::equal == Qt::weak_ordering::equivalent);
        static_assert(Qt::strong_ordering::greater == Qt::weak_ordering::greater);
    }
#ifdef __cpp_lib_three_way_comparison
    // Qt::partial_ordering <-> std::partial_ordering
    {
        static_assert(Qt::partial_ordering::less == std::partial_ordering::less);
        static_assert(Qt::partial_ordering::less != std::partial_ordering::greater);
        static_assert(std::partial_ordering::unordered != Qt::partial_ordering::equivalent);
        static_assert(std::partial_ordering::unordered == Qt::partial_ordering::unordered);

        static_assert((Qt::partial_ordering(std::partial_ordering::less) ==
                       std::partial_ordering::less));
        static_assert((Qt::partial_ordering(std::partial_ordering::equivalent) ==
                       std::partial_ordering::equivalent));
        static_assert((Qt::partial_ordering(std::partial_ordering::greater) ==
                       std::partial_ordering::greater));
        static_assert((Qt::partial_ordering(std::partial_ordering::unordered) ==
                       std::partial_ordering::unordered));
    }
    // Qt::weak_ordering <-> std::weak_ordering
    {
        static_assert(Qt::weak_ordering::less == std::weak_ordering::less);
        static_assert(Qt::weak_ordering::less != std::weak_ordering::equivalent);
        static_assert(std::weak_ordering::greater != Qt::weak_ordering::less);
        static_assert(std::weak_ordering::equivalent == Qt::weak_ordering::equivalent);

        static_assert((Qt::weak_ordering(std::weak_ordering::less) ==
                       std::weak_ordering::less));
        static_assert((Qt::weak_ordering(std::weak_ordering::equivalent) ==
                       std::weak_ordering::equivalent));
        static_assert((Qt::weak_ordering(std::weak_ordering::greater) ==
                       std::weak_ordering::greater));
    }
    // Qt::strong_ordering <-> std::strong_ordering
    {
        static_assert(Qt::strong_ordering::less == std::strong_ordering::less);
        static_assert(Qt::strong_ordering::less != std::strong_ordering::equivalent);
        static_assert(std::strong_ordering::greater != Qt::strong_ordering::less);
        static_assert(std::strong_ordering::equivalent == Qt::strong_ordering::equivalent);

        static_assert((Qt::strong_ordering(std::strong_ordering::less) ==
                       std::strong_ordering::less));
        static_assert((Qt::strong_ordering(std::strong_ordering::equivalent) ==
                       std::strong_ordering::equivalent));
        static_assert((Qt::strong_ordering(std::strong_ordering::greater) ==
                       std::strong_ordering::greater));
    }
    // Mixed Qt::*_ordering <> std::*_ordering types
    {
        static_assert(Qt::strong_ordering::less == std::partial_ordering::less);
        static_assert(Qt::strong_ordering::less != std::partial_ordering::greater);
        static_assert(Qt::strong_ordering::equal == std::weak_ordering::equivalent);
        static_assert(Qt::strong_ordering::equivalent != std::weak_ordering::less);

        static_assert(Qt::weak_ordering::less != std::partial_ordering::greater);
        static_assert(Qt::weak_ordering::less == std::partial_ordering::less);
        static_assert(Qt::weak_ordering::equivalent == std::strong_ordering::equivalent);
        static_assert(Qt::weak_ordering::equivalent != std::strong_ordering::less);

        static_assert(Qt::partial_ordering::less != std::weak_ordering::greater);
        static_assert(Qt::partial_ordering::less == std::weak_ordering::less);
        static_assert(Qt::partial_ordering::equivalent == std::strong_ordering::equivalent);
        static_assert(Qt::partial_ordering::equivalent != std::strong_ordering::less);
    }
#endif

}

void tst_QCompare::is_eq_overloads()
{
#ifndef __cpp_lib_three_way_comparison
    QSKIP("This test requires C++20 three-way-comparison support enabled in the stdlib.");
#else
    constexpr auto u = std::partial_ordering::unordered;
    constexpr auto l = std::weak_ordering::less;
    constexpr auto g = std::strong_ordering::greater;
    constexpr auto e = std::weak_ordering::equivalent;
    constexpr auto s = std::strong_ordering::equal;

    // This is a compile-time check that unqualified name lookup of
    // std::is_eq-like functions isn't ambiguous, so we can recommend it to our
    // users for minimizing porting on the way to C++20.

    // The goal is to check each std::ordering and each is_eq function at least
    // once, not to test all combinations (we're not the stdlib test suite here).

    QVERIFY(is_eq(s));
    QVERIFY(is_neq(u));
    QVERIFY(is_lt(l));
    QVERIFY(is_gt(g));
    QVERIFY(is_lteq(e));
    QVERIFY(is_gteq(s));
#endif // __cpp_lib_three_way_comparison
}

QTEST_MAIN(tst_QCompare)
#include "tst_qcompare.moc"
