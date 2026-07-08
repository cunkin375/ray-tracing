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
    dColor albedo_{0.5};

public:
    Lambertian(const dColor &albedo) : albedo_{albedo} {}

    std::optional<ScatterRecord> Scatter(const dRay &ray_in, const dVector3 &surface_hit_normal,
                                         const dPoint3 &end_point) const;
};

struct Metal {
private:
    dColor albedo_{0.5};
    f64 fuzz_{1.0};

public:
    Metal(const dColor &albedo) : albedo_{albedo} {}

    Metal(const dColor &albedo, f64 fuzz) : albedo_{albedo}, fuzz_{fuzz} {}

    std::optional<ScatterRecord> Scatter(const dRay &ray_in, const dVector3 &surface_hit_normal,
                                         const dPoint3 &end_point) const;
};

using Material = std::variant<Lambertian, Metal>;
