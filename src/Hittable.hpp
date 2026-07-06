#pragma once

#include <optional>
#include <utility>
#include <vector>

#include "Math/Vector.hpp"
#include "Ray.hpp"
#include "Util/Aliases.hpp"
#include "Util/Log.hpp"

struct HitRecord {
public:
    dPoint3 point;
    dVector3 normal;
    f64 scalar;
    bool front_face;

public:
    /* Here, the side of a hittable surface from the persepective of the ray is decided at geometry time */
    void SetFrontfaceNormal(const dRay &ray, const dVector3 &outward_normal) {
        front_face = (dVector3::DotProduct(ray.direction, outward_normal) < 0);
        normal = front_face ? outward_normal : -outward_normal;
    }
};

struct Hittable {
private:
    void *data_pointer = nullptr;

    std::optional<HitRecord> (*hit_function)(const void *, const dRay &ray, f64 ray_tmin,
                                             f64 ray_tmax) = nullptr;
    void *(*copy_function)(const void *) = nullptr;
    void (*destroy_function)(void *) = nullptr;

public:
    template <typename Derived>
    Hittable(Derived object) {
        data_pointer = new Derived(std::move(object));

        hit_function = [](const void *pointer, const dRay &ray, f64 ray_tmin, f64 ray_tmax) {
            static_cast<const Derived *>(pointer)->Hit(ray, ray_tmin, ray_tmax);
        };

        copy_function = [](const void *pointer) {
            return new Derived(static_cast<const Derived *>(pointer));
        };

        destroy_function = [](void *pointer) { delete static_cast<Derived *>(pointer); };
    }

    Hittable(const Hittable &other) {
        if (other.data_pointer) {
            data_pointer = other.copy_function(other.data_pointer);
            hit_function = other.hit_function;
            copy_function = other.copy_function;
            destroy_function = other.destroy_function;
        }
    }

    Hittable &operator=(const Hittable &other) { std::swap(data_pointer, other.data_pointer); }

    Hittable(Hittable &&other) noexcept
        : data_pointer{other.data_pointer}, hit_function{other.hit_function},
          copy_function{other.copy_function}, destroy_function{other.destroy_function} {
        other.data_pointer = nullptr;
    }

    ~Hittable() {
        if (data_pointer && destroy_function) { destroy_function(data_pointer); }
    }

    std::optional<HitRecord> Hit(const dRay &ray, f64 ray_tmin, f64 ray_tmax) const {
        if (data_pointer && hit_function) return hit_function(data_pointer, ray, ray_tmin, ray_tmax);
        else return std::nullopt;
    }
};

struct HittableList {
public:
    std::vector<Hittable> objects;

public:
    HittableList() = default;

    HittableList(Hittable object) { objects.push_back(object); }

    void Add(Hittable object) { objects.push_back(std::move(object)); }

    constexpr std::optional<HitRecord> Hit(const dRay &ray, f64 ray_tmin, f64 ray_tmax) const {
        auto return_record = HitRecord{};
        auto hit_anything = bool{false};
        auto closest_so_far = ray_tmax;

        for (const auto &object : objects) {
            if (auto temp_record = object.Hit(ray, ray_tmin, closest_so_far); temp_record != std::nullopt) {
                hit_anything = true;
                closest_so_far = temp_record->scalar;
                return_record = temp_record.value();
            } else {
                Log::Error("Received nullopt from object.Hit!");
            }
        }

        return hit_anything ? std::optional{return_record} : std::nullopt;
    }
};
