#pragma once

#include <optional>
#include <variant>

#include "Ray.hpp"

#include "Math/Vector.hpp"
#include "Util/Aliases.hpp"

struct ScatterRecord {
    dColor attenuation;
    dRay scattered_ray;
};

struct Lambertian {
private:
    dColor albedo_;

public:
    Lambertian(const dColor &albedo) : albedo_{albedo} {}

    std::optional<ScatterRecord> Scatter(const dRay &ray_in, const dVector3 &surface_hit_normal,
                                         const dPoint3 &end_point) const;
};

struct Metal {
private:
    dColor albedo_;

public:
    Metal(const dColor &albedo) : albedo_{albedo} {}

    std::optional<ScatterRecord> Scatter(const dRay &ray_in, const dVector3 &surface_hit_normal,
                                         const dPoint3 &end_point) const;
};

using Material = std::variant<Lambertian, Metal>;
