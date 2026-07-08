#pragma once

#include <iostream>

#include "HittableList.hpp"
#include "ImageColor.hpp"
#include "Random.hpp"
#include "Sphere.hpp"
#include "Types.hpp"

#include "Math/Vector.hpp"
#include "Util/Aliases.hpp"

class Camera {
public:
    f64 aspect_ratio{16.0 / 9.0};
    f64 reflectance{0.5};
    std::size_t image_width{400};
    std::size_t samples_per_pixel{10};
    std::size_t max_depth{10};

private:
    std::size_t image_height_{};
    f64 pixel_samples_scale_{};
    dPoint3 camera_center_{};
    dPoint3 pixel_00_location_{};
    dVector3 pixel_delta_u_;
    dVector3 pixel_delta_v_;

public:
    Camera(f64 _aspect_ratio, std::size_t _image_width)
        : aspect_ratio{_aspect_ratio}, image_width{_image_width} {}

    Camera(f64 _aspect_ratio, std::size_t _image_width, std::size_t _samples_per_pixel)
        : aspect_ratio{_aspect_ratio}, image_width{_image_width}, samples_per_pixel{_samples_per_pixel} {}

    void RenderPass(const World &world) {
        InitializePass();

        std::cout << "P3\n" << image_width << " " << image_height_ << "\n255\n";

        for (auto j{0zu}; j < image_height_; ++j) {
            for (auto i{0zu}; i < image_width; ++i) {
                auto pixel_center = pixel_00_location_ + (i * pixel_delta_u_) + (j * pixel_delta_v_);
                auto ray_direction = pixel_center - camera_center_;
                auto ray = dRay{camera_center_, ray_direction};
                dColor pixel_color = RayToColor(ray, max_depth, world);
                ImageColor::WriteColor(std::cout, pixel_color);
            }
        }
    }

    void AntialiasingRenderPass(const World &world) {
        InitializePass();

        std::cout << "P3\n" << image_width << " " << image_height_ << "\n255\n";

        for (auto j{0zu}; j < image_height_; ++j) {
            for (auto i{0zu}; i < image_width; ++i) {
                auto pixel_color = dColor{};
                for (auto sample{0zu}; sample < samples_per_pixel; ++sample) {
                    auto ray = GetRay(i, j);
                    pixel_color += RayToColor(ray, max_depth, world);
                }
                ImageColor::WriteColor(std::cout, pixel_samples_scale_ * pixel_color);
            }
        }
    }

private:
    dRay GetRay(std::size_t i, std::size_t j) const {
        auto offset = SampleSquare();
        auto pixel_sample =
            pixel_00_location_ + ((i + offset.x) * pixel_delta_u_) + ((j + offset.y) * pixel_delta_v_);
        auto ray_origin = camera_center_;
        auto ray_direction = pixel_sample - ray_origin;
        return dRay{ray_origin, ray_direction};
    }

    dVector3 SampleSquare() const { return dVector3{Rand::RandomF64() - 0.5, Rand::RandomF64() - 0.5, 0}; }

    dColor RayToColor(const dRay &ray, std::size_t depth, const World &world) const {
        if (depth <= 0) return dColor{0, 0, 0};

        // if we hit something, render it
        if (auto record = world.Hit(ray, dInterval{0.001, dInterval::PositiveInfinity()});
            record != std::nullopt) {

            // Lambertian Reflection
            [[maybe_unused]] dVector3 direction = record->normal + dVector3::GenerateRandomUnitVector();

            // [[maybe_unused]] dVector3 direction = dVector3::RandomUnitVectorOnHemisphere(record->normal);

            // this returns a color map
            // return 0.5 * dColor{record->normal + dVector3{1, 1, 1}};

            // this returns a diffused color by using the hemisphere unit vector's direction with the ray's
            // end point
            return reflectance * RayToColor(dRay{record->end_point, direction}, depth - 1, world);
        }

        // otherwise, just color the background
        dVector3 unit_direction = dVector3::Normalize(ray.direction);
        f64 alpha = 0.5 * (unit_direction.y + 1.0);

        // blend from blue up top to white at the bottom
        return (1.0 - alpha) * /* white */ dColor{1.0, 1.0, 1.0} +
               alpha * /* light blue */ dColor{0.5, 0.7, 1.0};
    }

    void InitializePass() {
        image_height_ = std::size_t(image_width / aspect_ratio);
        // image height must at least be 1
        image_height_ = (image_height_ < 1) ? 1 : image_height_;

        pixel_samples_scale_ = 1.0 / samples_per_pixel;

        // Viewport dimensions
        f64 focal_length = 1.0;
        f64 viewport_height = 2.0;
        f64 viewport_width = viewport_height * (f64(image_width) / image_height_);
        camera_center_ = dPoint3{};

        // Vectors across viewport edges
        auto viewport_u = dVector3{viewport_width, 0, 0};
        auto viewport_v = dVector3{0, -viewport_height, 0};

        // Horizontal and Vertical unit vectors
        pixel_delta_u_ = viewport_u / image_width;
        pixel_delta_v_ = viewport_v / image_height_;

        // Location of upper left pixel
        auto viewport_upper_left =
            camera_center_ - dVector3{0, 0, focal_length} - viewport_u / 2 - viewport_v / 2;
        pixel_00_location_ = viewport_upper_left + 0.5 * (pixel_delta_u_ + pixel_delta_v_);
    }
};
