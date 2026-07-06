#pragma once

#include <iostream>

#include "ImageColor.hpp"
#include "Hittable.hpp"
#include "Math/Vector.hpp"
#include "Sphere.hpp"
#include "Util/Aliases.hpp"

class Camera {
private:
    std::size_t image_height{};
    dPoint3 camera_center{};
    dPoint3 pixel_00_location{};
    dVector3 pixel_delta_u;
    dVector3 pixel_delta_v;

public:
    f64 aspect_ratio{16.0 / 9.0};
    std::size_t image_width{400zu};

public:
    Camera(f64 aspect_ratio, std::size_t image_width)
        : aspect_ratio{aspect_ratio}, image_width{image_width} {}

    void RenderPass(const HittableList &world) {
        InitializePass();

        std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

        for (auto j{0zu}; j < image_height; j++) {
            // Log::Info("\rScanlines remaining: {}", (image_height - j));
            for (auto i{0zu}; i < image_width; i++) {
                auto pixel_center = pixel_00_location + (i * pixel_delta_u) + (j * pixel_delta_v);
                auto ray_direction = pixel_center - camera_center;
                auto ray = dRay{camera_center, ray_direction};
                auto pixel_color = RayToColor(ray, world);
                ImageColor::WriteColor(std::cout, pixel_color);
            }
        }
    }

private:
    dColor RayToColor(const dRay &ray, const HittableList &world) {
        if (auto record = world.Hit(ray, dInterval{0, dInterval::PositiveInfinity()});
            record != std::nullopt) {
            return 0.5 * dColor{record->normal + dVector3{1, 1, 1}};
        }
        dVector3 unit_direction = dVector3::Normalize(ray.direction);
        f64 alpha = 0.5 * (unit_direction.y + 1.0);
        return (1.0 - alpha) * /* white */ dColor{1.0, 1.0, 1.0} +
               alpha * /* light blue */ dColor{0.5, 0.7, 1.0};
    }

    void InitializePass() {
        image_height = std::size_t(image_width / aspect_ratio);
        // image height must at least be 1
        image_height = (image_height < 1) ? 1 : image_height;

        // Viewport dimensions
        f64 focal_length = 1.0;
        f64 viewport_height = 2.0;
        f64 viewport_width = viewport_height * (f64(image_width) / image_height);
        camera_center = dPoint3{};

        // Vectors across viewport edges
        auto viewport_u = dVector3{viewport_width, 0, 0};
        auto viewport_v = dVector3{0, -viewport_height, 0};

        // Horizontal and Vertical unit vectors
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Location of upper left pixel
        auto viewport_upper_left =
            camera_center - dVector3{0, 0, focal_length} - viewport_u / 2 - viewport_v / 2;
        pixel_00_location = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }
};
