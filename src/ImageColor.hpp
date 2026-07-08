#pragma once

#include "Math/Interval.hpp"
#include "Math/Vector.hpp"

#include "Util/Aliases.hpp"

#include <iostream>

namespace ImageColor {

constexpr void WriteColor(std::ostream &out, const dColor &pixel_color) {
    auto r = pixel_color.r;
    auto g = pixel_color.g;
    auto b = pixel_color.b;

    static const auto intensity = dInterval{0.000, 0.999};
    i32 rbyte = i32(256 * intensity.clamp(r));
    i32 gbyte = i32(256 * intensity.clamp(g));
    i32 bbyte = i32(256 * intensity.clamp(b));

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

} // namespace ImageColor
