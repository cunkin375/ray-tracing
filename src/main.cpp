#include <bit>
#include <iostream>
#include <limits>
#include <numbers>

#include "Math/Vector.hpp"
#include "Util/Aliases.hpp"
#include "Util/Log.hpp"

#include "Color.hpp"
#include "Hittable.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"

// constants
constexpr f64 infitnity = std::numeric_limits<f64>::infinity();
constexpr f64 pi = std::numbers::pi;

dColor RayToColor(const dRay &ray, const HittableList &world) {
    if (auto record = world.Hit(ray, 0, infitnity); record != std::nullopt) {
        return 0.5 * dColor{record->normal + dVector3{1, 1, 1}};
    }
    dVector3 unit_direction = dVector3::Normalize(ray.direction);
    f64 alpha = 0.5 * (unit_direction.y + 1.0);
    return (1.0 - alpha) * /* white */ dColor{1.0, 1.0, 1.0} + alpha * /* light blue */ dColor{0.5, 0.7, 1.0};
}

int main() {

    // Image
    auto image_width{400zu};
    f64 aspect_ratio{16.0 / 9.0};

    auto image_height = std::size_t(image_width / aspect_ratio);
    // image height must at least be 1
    image_height = (image_height < 1) ? 1 : image_height;

    // World
    using Radius = f64;
    auto center_sphere = Sphere{dPoint3{0, 0, -1}, Radius{0.5}};
    auto big_bottom_sphere = Sphere{dPoint3{0, -100.5, -1}, Radius{100}};

    auto world = HittableList{};
    world.Add(center_sphere);
    world.Add(big_bottom_sphere);

    // Camera
    f64 focal_length = 1.0;
    f64 viewport_height = 2.0;
    f64 viewport_width = viewport_height * (f64(image_width) / image_height);
    auto camera_center = dPoint3{};

    auto viewport_u = dVector3{viewport_width, 0, 0};
    auto viewport_v = dVector3{0, -viewport_height, 0};

    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    auto viewport_upper_left = camera_center - dVector3{0, 0, focal_length} - viewport_u / 2 - viewport_v / 2;
    auto pixel100_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (auto j{0zu}; j < image_height; j++) {
        // Log::Info("\rScanlines remaining: {}", (image_height - j));
        for (auto i{0zu}; i < image_width; i++) {
            auto pixel_center = pixel100_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            auto ray = dRay{camera_center, ray_direction};
            auto pixel_color = RayToColor(ray, world);
            WriteColor(std::cout, pixel_color);
        }
    }

    // Log::Info("Hello, world!");
}
