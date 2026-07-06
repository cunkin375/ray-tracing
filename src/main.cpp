#include "Camera.hpp"
#include "Hittable.hpp"
#include "Sphere.hpp"

#include "Util/Aliases.hpp"

int main() {
    // Init World
    using Radius = f64;
    auto center_sphere = Sphere{dPoint3{0, 0, -1}, Radius{0.5}};
    auto big_bottom_sphere = Sphere{dPoint3{0, -100.5, -1}, Radius{100}};

    auto world = HittableList{};
    world.Add(center_sphere);
    world.Add(big_bottom_sphere);

    using AspectRatio = f64;
    using ImageWidth = std::size_t;
    using SamplesPerPixel = std::size_t;
    auto camera = Camera{AspectRatio{16.0 / 9.0}, ImageWidth{400}, SamplesPerPixel{100}};

    // camera.RenderPass(world);
    camera.AntialiasingRenderPass(world);

    // Log::Info("Hello, world!");
}
