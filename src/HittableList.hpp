#pragma once

#include <optional>
#include <tuple>
#include <utility>
#include <vector>

#include "Ray.hpp"
#include "Material.hpp"

#include "Math/Interval.hpp"
#include "Math/Vector.hpp"
#include "Util/Aliases.hpp"

struct HitRecord {
public:
    dPoint3 end_point;
    dVector3 normal;
    f64 distance;
    bool front_face;
    const Material *material_view;

public:
    /* Here, the side of a hittable surface from the persepective of the ray is decided at geometry time */
    void SetFrontfaceNormal(const dRay &ray, const dVector3 &outward_normal) {
        front_face = (dVector3::DotProduct(ray.direction, outward_normal) < 0);
        normal = front_face ? outward_normal : -outward_normal;
    }
};

template <typename... ShapeArgs>
struct HittableList {
public:
    std::tuple<std::vector<ShapeArgs>...> object_pools{};

public:
    HittableList() = default;

    template <typename Shape>
    void Add(Shape object) {
        std::get<std::vector<Shape>>(object_pools).push_back(std::move(object));
    }

    constexpr std::optional<HitRecord> Hit(const dRay &ray, dInterval ray_interval) const {
        auto return_record = HitRecord{};
        auto hit_anything = bool{false};
        auto closest_so_far = ray_interval.upper;

        // fold over each <Shape> type's vector for all <... ShapeArgs>
        std::apply(
            [&](const auto &...type_pools) {
                (([&](const auto &pool) {
                     for (const auto &object : pool) {
                         if (auto temp = object.Hit(ray, dInterval{ray_interval.lower, closest_so_far})) {
                             hit_anything = true;
                             closest_so_far = temp->distance;
                             return_record = *temp;
                         }
                     }
                 }(type_pools)),
                 ...); // go to the next ShapeArgument
            },
            object_pools); // apply this fold to object_pools

        // This is the original loop:
        //     for (const auto &object : objects) {
        //         if (auto temp_record = object.Hit(ray, dInterval{ray_interval.lower, closest_so_far});
        //             temp_record != std::nullopt) {
        //             hit_anything = true;
        //             closest_so_far = temp_record->distance;
        //             return_record = temp_record.value();
        //         }
        //     }

        return hit_anything ? std::optional{return_record} : std::nullopt;
    }
};
