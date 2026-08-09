#pragma once

#include <variant>

#include "Math/Vector.hpp"
#include "Util/Aliases.hpp"

class Texture;

namespace Textures {

class SolidColor {
private:
    dColor albedo_;

public:
    SolidColor() = default;

    SolidColor(const dColor &albedo) : albedo_{albedo} {}

    SolidColor(f64 red, f64 green, f64 blue) : SolidColor{dColor{red, green, blue}} {}

    dColor Value(f64 u, f64 v, const dPoint3 &p) const noexcept { return albedo_; }
};

template <typename T, typename U = T>
class Checker {
private:
    f64 inverse_scale_;
    T even_texture_;
    U odd_texture_;

public:
    Checker() = default;

    Checker(f64 scale, const T &even_texture, const U &odd_texture)
        : inverse_scale_{1.0 / scale}, even_texture_{even_texture}, odd_texture_{odd_texture} {}

    dColor Value(f64 u, f64 v, const dPoint3 &point) const noexcept {
        auto x_integer = i32(std::floor(inverse_scale_ * point.x));
        auto y_integer = i32(std::floor(inverse_scale_ * point.y));
        auto z_integer = i32(std::floor(inverse_scale_ * point.z));

        bool is_even = (x_integer + y_integer + z_integer) % 2 == 0;

        return is_even ? even_texture_.Value(u, v, point) : odd_texture_.Value(u, v, point);
    }
};

using TextureVariant = std::variant<SolidColor, Checker<SolidColor>>;

} // namespace Textures

class Texture {
private:
    Textures::TextureVariant data_;

public:
    Texture() = default;

    template <typename T>
    Texture(T &&data) : data_{std::forward<T>(data)} {}

    // NOTE: this might not be needed
    Texture(const dColor &data) : data_{Textures::SolidColor{data}} {}

    dColor Value(f64 u, f64 v, dPoint3 point) const {
        return std::visit([&](const auto &texture) { return texture.Value(u, v, point); }, data_);
    }
};
