#pragma once

#include <concepts>

namespace Math {

template <typename T>
concept Number = std::floating_point<T> || std::integral<T>;

}

