#pragma once

#include "Hittable.hpp"

#include "Math/Vector.hpp"
#include "Util/Aliases.hpp"

class Sphere {
private:
    dPoint3 center_;
    f64 radius_;

public:
    Sphere(const dPoint3 &center, f64 radius) : center_{center}, radius_{std::fmax(0, radius)} {}

    /* Hit function that solves quadratic of dot_product(vector, vector)->double */
    constexpr std::optional<HitRecord> Hit(const dRay &ray, f64 ray_tmin, f64 ray_tmax) const {
        dVector3 origin_center = center_ - ray.origin;
        f64 a = ray.direction.MagnitudeSquared();
        f64 h = dVector3::DotProduct(ray.direction, origin_center);
        f64 c = origin_center.MagnitudeSquared() - radius_ * radius_;
        f64 discriminant = h * h - a * c;
        if (discriminant < 0) return std::nullopt;

        f64 square_root_discriminant = std::sqrt(discriminant);

        // find nearest root
        f64 root = (h - square_root_discriminant) / a;
        if (root <= ray_tmin || ray_tmax <= root) {
            root = (h + square_root_discriminant) / a;
            if (root <= ray_tmin || ray_tmax <= root) return std::nullopt;
        }

        auto temp_record = HitRecord{};

        temp_record.scalar = root;
        temp_record.point = ray.At(temp_record.scalar);

        dVector3 outward_normal = (temp_record.point - center_) / radius_;
        temp_record.SetFrontfaceNormal(ray, outward_normal);

        return temp_record;
    }
};
