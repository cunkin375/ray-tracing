#pragma once

#include <numbers>
#include <random>

#include "Util/Aliases.hpp"

namespace Rand {

inline f64 DegreesToRadians(f64 degrees) { return degrees * std::numbers::pi; }

inline f64 RandomF64() {
    static auto distribution = std::uniform_real_distribution<f64>{0.0, 1.0};
    static auto generator = std::mt19937{};
    return distribution(generator);
}

inline f64 RandomF64(f64 min, f64 max) { return min + (max - min) * RandomF64(); }

}

