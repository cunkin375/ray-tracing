#include <iostream>

#include "Math/Vector.hpp"
#include "Util/Aliases.hpp"
#include "Util/Log.hpp"

#include "Color.hpp"
#include "Ray.hpp"

f64 HitSphere(const dPoint3 &center, f64 radius, const dRay &ray) {
    dVector3 oc = center - ray.origin;
    f64 a = dVector3::DotProduct(ray.direction, ray.direction);
    f64 b = -2.0 * dVector3::DotProduct(ray.direction, oc);
    f64 c = dVector3::DotProduct(oc, oc) - radius * radius;
    f64 discriminant = b * b - 4 * a * c;
    if (discriminant < 0) return -1.0;
    else return (-b - std::sqrt(discriminant)) / (2.0 * a);
}

dColor RayToColor(const dRay &ray) {
    f64 t = HitSphere(dPoint3{0, 0, -1}, 0.5, ray);
    if (t > 0.0) {
        dVector3 N = dVector3::UnitVector(ray.at(t) - dVector3{0, 0, -1});
        return 0.5 * dColor{N.x + 1, N.y + 1, N.z + 1};
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
            auto pixel_color = RayToColor(ray);
            WriteColor(std::cout, pixel_color);
        }
    }

    // Log::Info("Hello, world!");
}
