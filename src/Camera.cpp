#include "Camera.hpp"

#include <iostream>
#include <thread>
#include <vector>

#include "ImageColor.hpp"
#include "Math/Numbers.hpp"
#include "Math/Random.hpp"
#include "Math/Vector.hpp"

// ==== PUBLIC METHODS
// ======================================================================================================

Camera::Camera(f64 _aspect_ratio, std::size_t _image_width)
    : aspect_ratio{_aspect_ratio}, image_width{_image_width} {}

Camera::Camera(f64 _aspect_ratio, std::size_t _image_width, std::size_t _samples_per_pixel)
    : aspect_ratio{_aspect_ratio}, image_width{_image_width}, samples_per_pixel{_samples_per_pixel} {}

Camera::Camera(f64 _aspect_ratio, std::size_t _image_width, std::size_t _samples_per_pixel, f64 _vertical_fov)
    : aspect_ratio{_aspect_ratio}, image_width{_image_width}, samples_per_pixel{_samples_per_pixel},
      vertical_fov{_vertical_fov} {}

void Camera::RenderPass(const World &world) {
    Camera::InitializePass();

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

void Camera::AntialiasingRenderPass(const World &world) {
    InitializePass();

    // all pixels are stored here
    auto framebuffer = std::vector<dColor>(image_width * image_height_);
    const auto thread_count = std::thread::hardware_concurrency();

    auto threads = std::vector<std::jthread>{};
    threads.reserve(thread_count);

    // because there are no data races when rendering, trace in parallel
    // each thread gets a scanline
    for (auto t{0zu}; t < thread_count; ++t) {
        threads.emplace_back([&, t]() {
            for (auto j{t}; j < image_height_; j += thread_count) {
                for (auto i{0zu}; i < image_width; ++i) {
                    auto pixel_color = dColor{};
                    for (auto sample{0zu}; sample < samples_per_pixel; ++sample) {
                        auto ray = GetRay(i, j);
                        pixel_color += RayToColor(ray, max_depth, world);
                    }
                    framebuffer[j * image_width + i] = pixel_samples_scale_ * pixel_color;
                }
            }
        });
    }

    // NOTE: threads must end before writing to ppm
    threads.clear();

    // write to the ppm file
    std::cout << "P3\n" << image_width << " " << image_height_ << "\n255\n";
    for (const auto &color : framebuffer) {
        ImageColor::WriteColor(std::cout, color);
    }
}

// ==== PRIVATE METHODS
// =====================================================================================================

dRay Camera::GetRay(std::size_t i, std::size_t j) const {
    auto offset = SampleSquare();
    auto pixel_sample =
        pixel_00_location_ + ((i + offset.x) * pixel_delta_u_) + ((j + offset.y) * pixel_delta_v_);
    auto ray_origin = (defocus_angle <= 0) ? camera_center_ : DefocusDiskSample();
    auto ray_direction = pixel_sample - ray_origin;
    return dRay{ray_origin, ray_direction};
}
dPoint3 Camera::DefocusDiskSample() const {
    auto point = dVector2::GenerateRandomUnitVector();
    return camera_center_ + (point.x * defocus_disk_u) + (point.y * defocus_disk_v);
}

dVector3 Camera::SampleSquare() const {
    using namespace Math;
    return dVector3{Rand::GenerateRandomNormalizedNumber<f64>() - 0.5, Rand::GenerateRandomNormalizedNumber<f64>() - 0.5, 0};
}

dColor Camera::RayToColor(const dRay &ray, std::size_t depth, const World &world) const {
    if (depth <= 0) return dColor{0, 0, 0};

    // if we hit something, render it
    if (auto record = world.Hit(ray, dInterval{0.001, dInterval::PositiveInfinity()});
        record != std::nullopt) {

        auto scatter_result = std::visit([&](const auto &material) { return material.Scatter(ray, *record); },
                                         *record->material_view);

        // this returns a color map
        // return 0.5 * dColor{record->normal + dVector3{1, 1, 1}};

        // this returns a diffused color
        if (scatter_result)
            return scatter_result->attenuation * RayToColor(scatter_result->scattered_ray, depth - 1, world);
    }

    // otherwise, just color the background
    dVector3 unit_direction = dVector3::Normalize(ray.direction);
    f64 alpha = 0.5 * (unit_direction.y + 1.0);

    // blend from blue up top to white at the bottom
    return (1.0 - alpha) * /* white */ dColor{1.0, 1.0, 1.0} + alpha * /* light blue */ dColor{0.5, 0.7, 1.0};
}

void Camera::InitializePass() {
    image_height_ = std::size_t(image_width / aspect_ratio);
    // image height must at least be 1
    image_height_ = (image_height_ < 1) ? 1 : image_height_;

    pixel_samples_scale_ = 1.0 / samples_per_pixel;

    camera_center_ = look_from;

    // Viewport dimensions
    // f64 focal_length = (look_from - look_at).Magnitude();
    f64 theta = Math::DegreesToRadians(vertical_fov);
    auto h_ratio = std::tan(theta / 2);

    f64 viewport_height = 2.0 * h_ratio * focus_distance;
    f64 viewport_width = viewport_height * (f64(image_width) / image_height_);

    // u, v, w unit basis vectors for camerate coordinate frame
    w = dVector3::UnitVector(look_from - look_at);
    u = dVector3::UnitVector(dVector3::CrossProduct(vertical_up, w));
    v = dVector3::CrossProduct(w, u);

    // Vectors across viewport edges
    auto viewport_u = viewport_width * u;
    auto viewport_v = viewport_height * -v;

    // Horizontal and Vertical unit vectors
    pixel_delta_u_ = viewport_u / image_width;
    pixel_delta_v_ = viewport_v / image_height_;

    // Location of upper left pixel
    auto viewport_upper_left = camera_center_ - (focus_distance * w) - viewport_u / 2 - viewport_v / 2;
    pixel_00_location_ = viewport_upper_left + 0.5 * (pixel_delta_u_ + pixel_delta_v_);

    // Camera defocus disk basis vectors
    auto defocus_radius = focus_distance * std::tan(Math::DegreesToRadians(defocus_angle / 2));
    defocus_disk_u = u * defocus_radius;
    defocus_disk_v = v * defocus_radius;
}
