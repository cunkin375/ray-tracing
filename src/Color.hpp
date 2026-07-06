#pragma once

#include "Math/Vector.hpp"

#include "Util/Aliases.hpp"

#include <iostream>

constexpr void WriteColor(std::ostream &out, const dColor &pixel_color) {
    auto r = pixel_color.r;
    auto g = pixel_color.g;
    auto b = pixel_color.b;

    i32 rbyte = i32(255.999 * r);
    i32 gbyte = i32(255.999 * g);
    i32 bbyte = i32(255.999 * b);

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
