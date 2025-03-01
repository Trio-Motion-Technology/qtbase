// Copyright (C) 2020 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Giuseppe D'Angelo <giuseppe.dangelo@kdab.com>
// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QCOMPARE_H
#define QCOMPARE_H

#if 0
#pragma qt_class(QtCompare)
#endif

#include <QtCore/qglobal.h>
#include <QtCore/qcompare_impl.h>

#ifdef __cpp_lib_bit_cast
#include <bit>
#endif
#ifdef __cpp_lib_three_way_comparison
#include <compare>
#endif

QT_BEGIN_NAMESPACE

namespace QtPrivate {
using CompareUnderlyingType = qint8;

// [cmp.categories.pre] / 1
enum class Ordering : CompareUnderlyingType
{
    Equal = 0,
    Equivalent = Equal,
    Less = -1,
    Greater = 1
};

enum class LegacyUncomparable : CompareUnderlyingType
{
    Unordered = -127
};

enum class Uncomparable : CompareUnderlyingType
{
    Unordered =
        #if defined(_LIBCPP_VERSION) // libc++
                -127
        #elif defined(__GLIBCXX__)   // libstd++
                   2
        #else                        // assume MSSTL
                -128
        #endif
};

} // namespace QtPrivate

// [cmp.partialord]
class QPartialOrdering
{
public:
    static const QPartialOrdering Less;
    static const QPartialOrdering Equivalent;
    static const QPartialOrdering Greater;
    static const QPartialOrdering Unordered;

    friend constexpr bool operator==(QPartialOrdering lhs,
                                     QtPrivate::CompareAgainstLiteralZero) noexcept
    { return lhs.isOrdered() && lhs.m_order == 0; }

    friend constexpr bool operator!=(QPartialOrdering lhs,
                                     QtPrivate::CompareAgainstLiteralZero) noexcept
    { return lhs.isOrdered() && lhs.m_order != 0; }

    friend constexpr bool operator< (QPartialOrdering lhs,
                                     QtPrivate::CompareAgainstLiteralZero) noexcept
    { return lhs.isOrdered() && lhs.m_order <  0; }

    friend constexpr bool operator<=(QPartialOrdering lhs,
                                     QtPrivate::CompareAgainstLiteralZero) noexcept
    { return lhs.isOrdered() && lhs.m_order <= 0; }

    friend constexpr bool operator> (QPartialOrdering lhs,
                                     QtPrivate::CompareAgainstLiteralZero) noexcept
    { return lhs.isOrdered() && lhs.m_order >  0; }

    friend constexpr bool operator>=(QPartialOrdering lhs,
                                     QtPrivate::CompareAgainstLiteralZero) noexcept
    { return lhs.isOrdered() && lhs.m_order >= 0; }


    friend constexpr bool operator==(QtPrivate::CompareAgainstLiteralZero,
                                     QPartialOrdering rhs) noexcept
    { return rhs.isOrdered() && 0 == rhs.m_order; }

    friend constexpr bool operator!=(QtPrivate::CompareAgainstLiteralZero,
                                     QPartialOrdering rhs) noexcept
    { return rhs.isOrdered() && 0 != rhs.m_order; }

    friend constexpr bool operator< (QtPrivate::CompareAgainstLiteralZero,
                                     QPartialOrdering rhs) noexcept
    { return rhs.isOrdered() && 0 <  rhs.m_order; }

    friend constexpr bool operator<=(QtPrivate::CompareAgainstLiteralZero,
                                     QPartialOrdering rhs) noexcept
    { return rhs.isOrdered() && 0 <= rhs.m_order; }

    friend constexpr bool operator> (QtPrivate::CompareAgainstLiteralZero,
                                     QPartialOrdering rhs) noexcept
    { return rhs.isOrdered() && 0 >  rhs.m_order; }

    friend constexpr bool operator>=(QtPrivate::CompareAgainstLiteralZero,
                                     QPartialOrdering rhs) noexcept
    { return rhs.isOrdered() && 0 >= rhs.m_order; }


    friend constexpr bool operator==(QPartialOrdering lhs, QPartialOrdering rhs) noexcept
    { return lhs.m_order == rhs.m_order; }

    friend constexpr bool operator!=(QPartialOrdering lhs, QPartialOrdering rhs) noexcept
    { return lhs.m_order != rhs.m_order; }

#ifdef __cpp_lib_three_way_comparison
    constexpr Q_IMPLICIT QPartialOrdering(std::partial_ordering stdorder) noexcept
    {
        if (stdorder == std::partial_ordering::less)
            m_order = static_cast<QtPrivate::CompareUnderlyingType>(QtPrivate::Ordering::Less);
        else if (stdorder == std::partial_ordering::equivalent)
            m_order = static_cast<QtPrivate::CompareUnderlyingType>(QtPrivate::Ordering::Equivalent);
        else if (stdorder == std::partial_ordering::greater)
            m_order = static_cast<QtPrivate::CompareUnderlyingType>(QtPrivate::Ordering::Greater);
        else if (stdorder == std::partial_ordering::unordered)
            m_order = static_cast<QtPrivate::CompareUnderlyingType>(QtPrivate::LegacyUncomparable::Unordered);
    }

    constexpr Q_IMPLICIT operator std::partial_ordering() const noexcept
    {
        if (static_cast<QtPrivate::Ordering>(m_order) == QtPrivate::Ordering::Less)
            return std::partial_ordering::less;
        else if (static_cast<QtPrivate::Ordering>(m_order) == QtPrivate::Ordering::Equivalent)
            return std::partial_ordering::equivalent;
        else if (static_cast<QtPrivate::Ordering>(m_order) == QtPrivate::Ordering::Greater)
            return std::partial_ordering::greater;
        else if (static_cast<QtPrivate::LegacyUncomparable>(m_order) == QtPrivate::LegacyUncomparable::Unordered)
            return std::partial_ordering::unordered;
        return std::partial_ordering::unordered;
    }

    friend constexpr bool operator==(QPartialOrdering lhs, std::partial_ordering rhs) noexcept
    { return static_cast<std::partial_ordering>(lhs) == rhs; }

    friend constexpr bool operator!=(QPartialOrdering lhs, std::partial_ordering rhs) noexcept
    { return static_cast<std::partial_ordering>(lhs) != rhs; }

    friend constexpr bool operator==(std::partial_ordering lhs, QPartialOrdering rhs) noexcept
    { return lhs == static_cast<std::partial_ordering>(rhs); }

    friend constexpr bool operator!=(std::partial_ordering lhs, QPartialOrdering rhs) noexcept
    { return lhs != static_cast<std::partial_ordering>(rhs); }
#endif // __cpp_lib_three_way_comparison

private:
    constexpr explicit QPartialOrdering(QtPrivate::Ordering order) noexcept
        : m_order(static_cast<QtPrivate::CompareUnderlyingType>(order))
    {}
    constexpr explicit QPartialOrdering(QtPrivate::LegacyUncomparable order) noexcept
        : m_order(static_cast<QtPrivate::CompareUnderlyingType>(order))
    {}

    QT_WARNING_PUSH
    // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=100903
    QT_WARNING_DISABLE_GCC("-Wzero-as-null-pointer-constant")
    friend constexpr bool is_eq  (QPartialOrdering o) noexcept { return o == 0; }
    friend constexpr bool is_neq (QPartialOrdering o) noexcept { return o != 0; }
    friend constexpr bool is_lt  (QPartialOrdering o) noexcept { return o <  0; }
    friend constexpr bool is_lteq(QPartialOrdering o) noexcept { return o <= 0; }
    friend constexpr bool is_gt  (QPartialOrdering o) noexcept { return o >  0; }
    friend constexpr bool is_gteq(QPartialOrdering o) noexcept { return o >= 0; }
    QT_WARNING_POP

    // instead of the exposition only is_ordered member in [cmp.partialord],
    // use a private function
    constexpr bool isOrdered() const noexcept
    { return m_order != static_cast<QtPrivate::CompareUnderlyingType>(QtPrivate::LegacyUncomparable::Unordered); }

    QtPrivate::CompareUnderlyingType m_order;
};

inline constexpr QPartialOrdering QPartialOrdering::Less(QtPrivate::Ordering::Less);
inline constexpr QPartialOrdering QPartialOrdering::Equivalent(QtPrivate::Ordering::Equivalent);
inline constexpr QPartialOrdering QPartialOrdering::Greater(QtPrivate::Ordering::Greater);
inline constexpr QPartialOrdering QPartialOrdering::Unordered(QtPrivate::LegacyUncomparable::Unordered);

namespace Qt {

class partial_ordering
{
public:
    static const partial_ordering less;
    static const partial_ordering equivalent;
    static const partial_ordering greater;
    static const partial_ordering unordered;

    friend constexpr bool operator==(partial_ordering lhs,
                                     QtPrivate::CompareAgainstLiteralZero) noexcept
    { return lhs.isOrdered() && lhs.m_order == 0; }

    friend constexpr bool operator!=(partial_ordering lhs,
                                     QtPrivate::CompareAgainstLiteralZero) noexcept
    { return lhs.isOrdered() && lhs.m_order != 0; }

    friend constexpr bool operator< (partial_ordering lhs,
                                     QtPrivate::CompareAgainstLiteralZero) noexcept
    { return lhs.isOrdered() && lhs.m_order <  0; }

    friend constexpr bool operator<=(partial_ordering lhs,
                                     QtPrivate::CompareAgainstLiteralZero) noexcept
    { return lhs.isOrdered() && lhs.m_order <= 0; }

    friend constexpr bool operator> (partial_ordering lhs,
                                     QtPrivate::CompareAgainstLiteralZero) noexcept
    { return lhs.isOrdered() && lhs.m_order >  0; }

    friend constexpr bool operator>=(partial_ordering lhs,
                                     QtPrivate::CompareAgainstLiteralZero) noexcept
    { return lhs.isOrdered() && lhs.m_order >= 0; }


    friend constexpr bool operator==(QtPrivate::CompareAgainstLiteralZero,
                                     partial_ordering rhs) noexcept
    { return rhs.isOrdered() && 0 == rhs.m_order; }

    friend constexpr bool operator!=(QtPrivate::CompareAgainstLiteralZero,
                                     partial_ordering rhs) noexcept
    { return rhs.isOrdered() && 0 != rhs.m_order; }

    friend constexpr bool operator< (QtPrivate::CompareAgainstLiteralZero,
                                     partial_ordering rhs) noexcept
    { return rhs.isOrdered() && 0 <  rhs.m_order; }

    friend constexpr bool operator<=(QtPrivate::CompareAgainstLiteralZero,
                                     partial_ordering rhs) noexcept
    { return rhs.isOrdered() && 0 <= rhs.m_order; }

    friend constexpr bool operator> (QtPrivate::CompareAgainstLiteralZero,
                                     partial_ordering rhs) noexcept
    { return rhs.isOrdered() && 0 >  rhs.m_order; }

    friend constexpr bool operator>=(QtPrivate::CompareAgainstLiteralZero,
                                     partial_ordering rhs) noexcept
    { return rhs.isOrdered() && 0 >= rhs.m_order; }


    friend constexpr bool operator==(partial_ordering lhs, partial_ordering rhs) noexcept
    { return lhs.m_order == rhs.m_order; }

    friend constexpr bool operator!=(partial_ordering lhs, partial_ordering rhs) noexcept
    { return lhs.m_order != rhs.m_order; }

#ifdef __cpp_lib_three_way_comparison
    constexpr Q_IMPLICIT partial_ordering(std::partial_ordering stdorder) noexcept
    {
        if (stdorder == std::partial_ordering::less)
            m_order = static_cast<QtPrivate::CompareUnderlyingType>(QtPrivate::Ordering::Less);
        else if (stdorder == std::partial_ordering::equivalent)
            m_order = static_cast<QtPrivate::CompareUnderlyingType>(QtPrivate::Ordering::Equivalent);
        else if (stdorder == std::partial_ordering::greater)
            m_order = static_cast<QtPrivate::CompareUnderlyingType>(QtPrivate::Ordering::Greater);
        else if (stdorder == std::partial_ordering::unordered)
            m_order = static_cast<QtPrivate::CompareUnderlyingType>(QtPrivate::Uncomparable::Unordered);
    }

    constexpr Q_IMPLICIT operator std::partial_ordering() const noexcept
    {
        static_assert(sizeof(*this) == sizeof(std::partial_ordering));
#ifdef __cpp_lib_bit_cast
        return std::bit_cast<std::partial_ordering>(*this);
#else
        switch (m_order) {
        case qToUnderlying(QtPrivate::Ordering::Less):          return std::partial_ordering::less;
        case qToUnderlying(QtPrivate::Ordering::Greater):       return std::partial_ordering::greater;
        case qToUnderlying(QtPrivate::Ordering::Equivalent):    return std::partial_ordering::equivalent;
        case qToUnderlying(QtPrivate::Uncomparable::Unordered): return std::partial_ordering::unordered;
        }
        Q_UNREACHABLE_RETURN(std::partial_ordering::unordered);
#endif // __cpp_lib_bit_cast
    }

    friend constexpr bool operator==(partial_ordering lhs, std::partial_ordering rhs) noexcept
    { return static_cast<std::partial_ordering>(lhs) == rhs; }

    friend constexpr bool operator!=(partial_ordering lhs, std::partial_ordering rhs) noexcept
    { return static_cast<std::partial_ordering>(lhs) != rhs; }

    friend constexpr bool operator==(std::partial_ordering lhs, partial_ordering rhs) noexcept
    { return lhs == static_cast<std::partial_ordering>(rhs); }

    friend constexpr bool operator!=(std::partial_ordering lhs, partial_ordering rhs) noexcept
    { return lhs != static_cast<std::partial_ordering>(rhs); }
#endif // __cpp_lib_three_way_comparison

private:
    friend class weak_ordering;
    friend class strong_ordering;

    constexpr explicit partial_ordering(QtPrivate::Ordering order) noexcept
        : m_order(static_cast<QtPrivate::CompareUnderlyingType>(order))
    {}
    constexpr explicit partial_ordering(QtPrivate::Uncomparable order) noexcept
        : m_order(static_cast<QtPrivate::CompareUnderlyingType>(order))
    {}

    QT_WARNING_PUSH
    // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=100903
    QT_WARNING_DISABLE_GCC("-Wzero-as-null-pointer-constant")
    friend constexpr bool is_eq  (partial_ordering o) noexcept { return o == 0; }
    friend constexpr bool is_neq (partial_ordering o) noexcept { return o != 0; }
    friend constexpr bool is_lt  (partial_ordering o) noexcept { return o <  0; }
    friend constexpr bool is_lteq(partial_ordering o) noexcept { return o <= 0; }
    friend constexpr bool is_gt  (partial_ordering o) noexcept { return o >  0; }
    friend constexpr bool is_gteq(partial_ordering o) noexcept { return o >= 0; }
    QT_WARNING_POP

    // instead of the exposition only is_ordered member in [cmp.partialord],
    // use a private function
    constexpr bool isOrdered() const noexcept
    { return m_order != static_cast<QtPrivate::CompareUnderlyingType>(QtPrivate::Uncomparable::Unordered); }

    QtPrivate::CompareUnderlyingType m_order;
};

inline constexpr partial_ordering partial_ordering::less(QtPrivate::Ordering::Less);
inline constexpr partial_ordering partial_ordering::equivalent(QtPrivate::Ordering::Equivalent);
inline constexpr partial_ordering partial_ordering::greater(QtPrivate::Ordering::Greater);
inline constexpr partial_ordering partial_ordering::unordered(QtPrivate::Uncomparable::Unordered);

class weak_ordering
{
public:
    static const weak_ordering less;
    static const weak_ordering equivalent;
    static const weak_ordering greater;

    constexpr Q_IMPLICIT operator partial_ordering() const noexcept
    { return partial_ordering(static_cast<QtPrivate::Ordering>(m_order)); }

    friend constexpr bool operator==(weak_ordering lhs,
                                     QtPrivate::CompareAgainstLiteralZero) noexcept
    { return lhs.m_order == 0; }

    friend constexpr bool operator!=(weak_ordering lhs,
                                     QtPrivate::CompareAgainstLiteralZero) noexcept
    { return lhs.m_order != 0; }

    friend constexpr bool operator< (weak_ordering lhs,
                                     QtPrivate::CompareAgainstLiteralZero) noexcept
    { return lhs.m_order <  0; }

    friend constexpr bool operator<=(weak_ordering lhs,
                                     QtPrivate::CompareAgainstLiteralZero) noexcept
    { return lhs.m_order <= 0; }

    friend constexpr bool operator> (weak_ordering lhs,
                                     QtPrivate::CompareAgainstLiteralZero) noexcept
    { return lhs.m_order >  0; }

    friend constexpr bool operator>=(weak_ordering lhs,
                                     QtPrivate::CompareAgainstLiteralZero) noexcept
    { return lhs.m_order >= 0; }


    friend constexpr bool operator==(QtPrivate::CompareAgainstLiteralZero,
                                     weak_ordering rhs) noexcept
    { return 0 == rhs.m_order; }

    friend constexpr bool operator!=(QtPrivate::CompareAgainstLiteralZero,
                                     weak_ordering rhs) noexcept
    { return 0 != rhs.m_order; }

    friend constexpr bool operator< (QtPrivate::CompareAgainstLiteralZero,
                                     weak_ordering rhs) noexcept
    { return 0 <  rhs.m_order; }

    friend constexpr bool operator<=(QtPrivate::CompareAgainstLiteralZero,
                                     weak_ordering rhs) noexcept
    { return 0 <= rhs.m_order; }

    friend constexpr bool operator> (QtPrivate::CompareAgainstLiteralZero,
                                     weak_ordering rhs) noexcept
    { return 0 > rhs.m_order; }

    friend constexpr bool operator>=(QtPrivate::CompareAgainstLiteralZero,
                                     weak_ordering rhs) noexcept
    { return 0 >= rhs.m_order; }


    friend constexpr bool operator==(weak_ordering lhs, weak_ordering rhs) noexcept
    { return lhs.m_order == rhs.m_order; }

    friend constexpr bool operator!=(weak_ordering lhs, weak_ordering rhs) noexcept
    { return lhs.m_order != rhs.m_order; }

    friend constexpr bool operator==(weak_ordering lhs, partial_ordering rhs) noexcept
    { return static_cast<partial_ordering>(lhs) == rhs; }

    friend constexpr bool operator!=(weak_ordering lhs, partial_ordering rhs) noexcept
    { return static_cast<partial_ordering>(lhs) != rhs; }

    friend constexpr bool operator==(partial_ordering lhs, weak_ordering rhs) noexcept
    { return lhs == static_cast<partial_ordering>(rhs); }

    friend constexpr bool operator!=(partial_ordering lhs, weak_ordering rhs) noexcept
    { return lhs != static_cast<partial_ordering>(rhs); }

#ifdef __cpp_lib_three_way_comparison
    constexpr Q_IMPLICIT weak_ordering(std::weak_ordering stdorder) noexcept
    {
        if (stdorder == std::weak_ordering::less)
            m_order = static_cast<QtPrivate::CompareUnderlyingType>(QtPrivate::Ordering::Less);
        else if (stdorder == std::weak_ordering::equivalent)
            m_order = static_cast<QtPrivate::CompareUnderlyingType>(QtPrivate::Ordering::Equivalent);
        else if (stdorder == std::weak_ordering::greater)
            m_order = static_cast<QtPrivate::CompareUnderlyingType>(QtPrivate::Ordering::Greater);
    }

    constexpr Q_IMPLICIT operator std::weak_ordering() const noexcept
    {
        static_assert(sizeof(*this) == sizeof(std::weak_ordering));
#ifdef __cpp_lib_bit_cast
        return std::bit_cast<std::weak_ordering>(*this);
#else
        switch (m_order) {
        case qToUnderlying(QtPrivate::Ordering::Less):          return std::weak_ordering::less;
        case qToUnderlying(QtPrivate::Ordering::Greater):       return std::weak_ordering::greater;
        case qToUnderlying(QtPrivate::Ordering::Equivalent):    return std::weak_ordering::equivalent;
        }
        Q_UNREACHABLE_RETURN(std::weak_ordering::equivalent);
#endif // __cpp_lib_bit_cast
    }

    friend constexpr bool operator==(weak_ordering lhs, std::weak_ordering rhs) noexcept
    { return static_cast<std::weak_ordering>(lhs) == rhs; }

    friend constexpr bool operator!=(weak_ordering lhs, std::weak_ordering rhs) noexcept
    { return static_cast<std::weak_ordering>(lhs) != rhs; }

    friend constexpr bool operator==(weak_ordering lhs, std::partial_ordering rhs) noexcept
    { return static_cast<std::weak_ordering>(lhs) == rhs; }

    friend constexpr bool operator!=(weak_ordering lhs, std::partial_ordering rhs) noexcept
    { return static_cast<std::weak_ordering>(lhs) != rhs; }

    friend constexpr bool operator==(weak_ordering lhs, std::strong_ordering rhs) noexcept
    { return static_cast<std::weak_ordering>(lhs) == rhs; }

    friend constexpr bool operator!=(weak_ordering lhs, std::strong_ordering rhs) noexcept
    { return static_cast<std::weak_ordering>(lhs) != rhs; }

    friend constexpr bool operator==(std::weak_ordering lhs, weak_ordering rhs) noexcept
    { return lhs == static_cast<std::weak_ordering>(rhs); }

    friend constexpr bool operator!=(std::weak_ordering lhs, weak_ordering rhs) noexcept
    { return lhs != static_cast<std::weak_ordering>(rhs); }

    friend constexpr bool operator==(std::partial_ordering lhs, weak_ordering rhs) noexcept
    { return lhs == static_cast<std::weak_ordering>(rhs); }

    friend constexpr bool operator!=(std::partial_ordering lhs, weak_ordering rhs) noexcept
    { return lhs != static_cast<std::weak_ordering>(rhs); }

    friend constexpr bool operator==(std::strong_ordering lhs, weak_ordering rhs) noexcept
    { return lhs == static_cast<std::weak_ordering>(rhs); }

    friend constexpr bool operator!=(std::strong_ordering lhs, weak_ordering rhs) noexcept
    { return lhs != static_cast<std::weak_ordering>(rhs); }
#endif // __cpp_lib_three_way_comparison

private:
    friend class strong_ordering;

    constexpr explicit weak_ordering(QtPrivate::Ordering order) noexcept
        : m_order(static_cast<QtPrivate::CompareUnderlyingType>(order))
    {}

    QT_WARNING_PUSH
    // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=100903
    QT_WARNING_DISABLE_GCC("-Wzero-as-null-pointer-constant")
    friend constexpr bool is_eq  (weak_ordering o) noexcept { return o == 0; }
    friend constexpr bool is_neq (weak_ordering o) noexcept { return o != 0; }
    friend constexpr bool is_lt  (weak_ordering o) noexcept { return o <  0; }
    friend constexpr bool is_lteq(weak_ordering o) noexcept { return o <= 0; }
    friend constexpr bool is_gt  (weak_ordering o) noexcept { return o >  0; }
    friend constexpr bool is_gteq(weak_ordering o) noexcept { return o >= 0; }
    QT_WARNING_POP

    QtPrivate::CompareUnderlyingType m_order;
};

inline constexpr weak_ordering weak_ordering::less(QtPrivate::Ordering::Less);
inline constexpr weak_ordering weak_ordering::equivalent(QtPrivate::Ordering::Equivalent);
inline constexpr weak_ordering weak_ordering::greater(QtPrivate::Ordering::Greater);

class strong_ordering
{
public:
    static const strong_ordering less;
    static const strong_ordering equivalent;
    static const strong_ordering equal;
    static const strong_ordering greater;

    constexpr Q_IMPLICIT operator partial_ordering() const noexcept
    { return partial_ordering(static_cast<QtPrivate::Ordering>(m_order)); }

    constexpr Q_IMPLICIT operator weak_ordering() const noexcept
    { return weak_ordering(static_cast<QtPrivate::Ordering>(m_order)); }

    friend constexpr bool operator==(strong_ordering lhs,
                                     QtPrivate::CompareAgainstLiteralZero) noexcept
    { return lhs.m_order == 0; }

    friend constexpr bool operator!=(strong_ordering lhs,
                                     QtPrivate::CompareAgainstLiteralZero) noexcept
    { return lhs.m_order != 0; }

    friend constexpr bool operator< (strong_ordering lhs,
                                     QtPrivate::CompareAgainstLiteralZero) noexcept
    { return lhs.m_order <  0; }

    friend constexpr bool operator<=(strong_ordering lhs,
                                     QtPrivate::CompareAgainstLiteralZero) noexcept
    { return lhs.m_order <= 0; }

    friend constexpr bool operator> (strong_ordering lhs,
                                     QtPrivate::CompareAgainstLiteralZero) noexcept
    { return lhs.m_order >  0; }

    friend constexpr bool operator>=(strong_ordering lhs,
                                     QtPrivate::CompareAgainstLiteralZero) noexcept
    { return lhs.m_order >= 0; }


    friend constexpr bool operator==(QtPrivate::CompareAgainstLiteralZero,
                                     strong_ordering rhs) noexcept
    { return 0 == rhs.m_order; }

    friend constexpr bool operator!=(QtPrivate::CompareAgainstLiteralZero,
                                     strong_ordering rhs) noexcept
    { return 0 != rhs.m_order; }

    friend constexpr bool operator< (QtPrivate::CompareAgainstLiteralZero,
                                    strong_ordering rhs) noexcept
    { return 0 <  rhs.m_order; }

    friend constexpr bool operator<=(QtPrivate::CompareAgainstLiteralZero,
                                     strong_ordering rhs) noexcept
    { return 0 <= rhs.m_order; }

    friend constexpr bool operator> (QtPrivate::CompareAgainstLiteralZero,
                                    strong_ordering rhs) noexcept
    { return 0 >  rhs.m_order; }

    friend constexpr bool operator>=(QtPrivate::CompareAgainstLiteralZero,
                                     strong_ordering rhs) noexcept
    { return 0 >= rhs.m_order; }


    friend constexpr bool operator==(strong_ordering lhs, strong_ordering rhs) noexcept
    { return lhs.m_order == rhs.m_order; }

    friend constexpr bool operator!=(strong_ordering lhs, strong_ordering rhs) noexcept
    { return lhs.m_order != rhs.m_order; }

    friend constexpr bool operator==(strong_ordering lhs, partial_ordering rhs) noexcept
    { return static_cast<partial_ordering>(lhs) == rhs; }

    friend constexpr bool operator!=(strong_ordering lhs, partial_ordering rhs) noexcept
    { return static_cast<partial_ordering>(lhs) == rhs; }

    friend constexpr bool operator==(partial_ordering lhs, strong_ordering rhs) noexcept
    { return lhs == static_cast<partial_ordering>(rhs); }

    friend constexpr bool operator!=(partial_ordering lhs, strong_ordering rhs) noexcept
    { return lhs != static_cast<partial_ordering>(rhs); }

    friend constexpr bool operator==(strong_ordering lhs, weak_ordering rhs) noexcept
    { return static_cast<weak_ordering>(lhs) == rhs; }

    friend constexpr bool operator!=(strong_ordering lhs, weak_ordering rhs) noexcept
    { return static_cast<weak_ordering>(lhs) == rhs; }

    friend constexpr bool operator==(weak_ordering lhs, strong_ordering rhs) noexcept
    { return lhs == static_cast<weak_ordering>(rhs); }

    friend constexpr bool operator!=(weak_ordering lhs, strong_ordering rhs) noexcept
    { return lhs != static_cast<weak_ordering>(rhs); }

#ifdef __cpp_lib_three_way_comparison
    constexpr Q_IMPLICIT strong_ordering(std::strong_ordering stdorder) noexcept
    {
        if (stdorder == std::strong_ordering::less)
            m_order = static_cast<QtPrivate::CompareUnderlyingType>(QtPrivate::Ordering::Less);
        else if (stdorder == std::strong_ordering::equivalent)
            m_order = static_cast<QtPrivate::CompareUnderlyingType>(QtPrivate::Ordering::Equivalent);
        else if (stdorder == std::strong_ordering::equal)
            m_order = static_cast<QtPrivate::CompareUnderlyingType>(QtPrivate::Ordering::Equal);
        else if (stdorder == std::strong_ordering::greater)
            m_order = static_cast<QtPrivate::CompareUnderlyingType>(QtPrivate::Ordering::Greater);
    }

    constexpr Q_IMPLICIT operator std::strong_ordering() const noexcept
    {
        static_assert(sizeof(*this) == sizeof(std::strong_ordering));
#ifdef __cpp_lib_bit_cast
        return std::bit_cast<std::strong_ordering>(*this);
#else
        switch (m_order) {
        case qToUnderlying(QtPrivate::Ordering::Less):    return std::strong_ordering::less;
        case qToUnderlying(QtPrivate::Ordering::Greater): return std::strong_ordering::greater;
        case qToUnderlying(QtPrivate::Ordering::Equal):   return std::strong_ordering::equal;
        }
        Q_UNREACHABLE_RETURN(std::strong_ordering::equal);
#endif // __cpp_lib_bit_cast
    }

    friend constexpr bool operator==(strong_ordering lhs, std::strong_ordering rhs) noexcept
    { return static_cast<std::strong_ordering>(lhs) == rhs; }

    friend constexpr bool operator!=(strong_ordering lhs, std::strong_ordering rhs) noexcept
    { return static_cast<std::strong_ordering>(lhs) != rhs; }

    friend constexpr bool operator==(strong_ordering lhs, std::partial_ordering rhs) noexcept
    { return static_cast<std::strong_ordering>(lhs) == rhs; }

    friend constexpr bool operator!=(strong_ordering lhs, std::partial_ordering rhs) noexcept
    { return static_cast<std::strong_ordering>(lhs) != rhs; }

    friend constexpr bool operator==(strong_ordering lhs, std::weak_ordering rhs) noexcept
    { return static_cast<std::strong_ordering>(lhs) == rhs; }

    friend constexpr bool operator!=(strong_ordering lhs, std::weak_ordering rhs) noexcept
    { return static_cast<std::strong_ordering>(lhs) != rhs; }

    friend constexpr bool operator==(std::strong_ordering lhs, strong_ordering rhs) noexcept
    { return lhs == static_cast<std::strong_ordering>(rhs); }

    friend constexpr bool operator!=(std::strong_ordering lhs, strong_ordering rhs) noexcept
    { return lhs != static_cast<std::strong_ordering>(rhs); }

    friend constexpr bool operator==(std::partial_ordering lhs, strong_ordering rhs) noexcept
    { return lhs == static_cast<std::strong_ordering>(rhs); }

    friend constexpr bool operator!=(std::partial_ordering lhs, strong_ordering rhs) noexcept
    { return lhs != static_cast<std::strong_ordering>(rhs); }

    friend constexpr bool operator==(std::weak_ordering lhs, strong_ordering rhs) noexcept
    { return lhs == static_cast<std::strong_ordering>(rhs); }

    friend constexpr bool operator!=(std::weak_ordering lhs, strong_ordering rhs) noexcept
    { return lhs != static_cast<std::strong_ordering>(rhs); }
#endif // __cpp_lib_three_way_comparison

    private:
    constexpr explicit strong_ordering(QtPrivate::Ordering order) noexcept
        : m_order(static_cast<QtPrivate::CompareUnderlyingType>(order))
    {}

    QT_WARNING_PUSH
    // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=100903
    QT_WARNING_DISABLE_GCC("-Wzero-as-null-pointer-constant")
    friend constexpr bool is_eq  (strong_ordering o) noexcept { return o == 0; }
    friend constexpr bool is_neq (strong_ordering o) noexcept { return o != 0; }
    friend constexpr bool is_lt  (strong_ordering o) noexcept { return o <  0; }
    friend constexpr bool is_lteq(strong_ordering o) noexcept { return o <= 0; }
    friend constexpr bool is_gt  (strong_ordering o) noexcept { return o >  0; }
    friend constexpr bool is_gteq(strong_ordering o) noexcept { return o >= 0; }
    QT_WARNING_POP

    QtPrivate::CompareUnderlyingType m_order;
};

inline constexpr strong_ordering strong_ordering::less(QtPrivate::Ordering::Less);
inline constexpr strong_ordering strong_ordering::equivalent(QtPrivate::Ordering::Equivalent);
inline constexpr strong_ordering strong_ordering::equal(QtPrivate::Ordering::Equal);
inline constexpr strong_ordering strong_ordering::greater(QtPrivate::Ordering::Greater);

} // namespace Qt

QT_END_NAMESPACE

// This is intentionally included in the end of qcompare.h
#include <QtCore/qcomparehelpers.h>

#endif // QCOMPARE_H
