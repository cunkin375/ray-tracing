#pragma once

#include "Numbers.hpp"

#include <format>
#include <limits>
#include <stdint.h>

namespace Math {

template <Number T>
struct Interval {
    T lower{}, upper{};

    static constexpr T NegativeInfinity() noexcept {
        if constexpr (std::numeric_limits<T>::has_infinity) {
            return -std::numeric_limits<T>::infinity();
        } else {
            return std::numeric_limits<T>::lowest();
        }
    }

    static constexpr T PositiveInfinity() noexcept {
        if constexpr (std::numeric_limits<T>::has_infinity) {
            return std::numeric_limits<T>::infinity();
        } else {
            return std::numeric_limits<T>::max();
        }
    }

    // this should represent empty constant
    constexpr Interval() : lower{PositiveInfinity()}, upper{NegativeInfinity()} {}

    constexpr Interval(T lower, T upper) : lower{lower}, upper{upper} {}

    constexpr double Size() const { return upper - lower; }

    constexpr bool Contains(T x) const { return lower <= x && x <= upper; }

    constexpr bool Surrounds(T x) const { return lower < x && x < upper; }

    constexpr T clamp(T number) const {
        if (number < lower) return lower;
        if (number > upper) return upper;
        return number;
    }

    static constexpr Interval Empty() { return {PositiveInfinity(), NegativeInfinity()}; }
    static constexpr Interval Universe() { return {NegativeInfinity(), PositiveInfinity()}; }
};

} // namespace Math

template <Math::Number T>
struct std::formatter<Math::Interval<T>> {
    constexpr auto parse(std::format_parse_context &context) const { return std::begin(context); }
    constexpr auto format(const Math::Interval<T> &interval, std::format_context &context) const {
        auto out = std::format_to(context.out(), "(");
        out = std::format_to(out, "{}, {}", interval.lower, interval.upper);
        return std::format_to(out, ")");
    }
};

/* Aliases */
using Interval = Math::Interval<float>;
using dInterval = Math::Interval<double>;
using uInterval = Math::Interval<std::uint32_t>;
using iInterval = Math::Interval<std::int32_t>;
