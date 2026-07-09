#include "Camera.hpp"
#include "Material.hpp"
#include "Sphere.hpp"
#include "Types.hpp"

#include "Math/Vector.hpp"
#include "Util/Aliases.hpp"

int main() {
    // Init World Objects
    using Radius = f64;
    using Reflection = f64;
    using RefractionIndex = f64;

    auto center_sphere = Sphere{dPoint3{0, 0, -1.2}, Radius{0.5},
                                Material{std::in_place_type<Lambertian>, dColor{0.8, 0.0, 0.2}}};

    auto big_bottom_sphere = Sphere{dPoint3{0, -100.5, -1}, Radius{100},
                                    Material{std::in_place_type<Lambertian>, dColor{0.8, 0.8, 0.0}}};

    // auto left_sphere = Sphere{dPoint3{-1, 0, -1}, Radius{0.5},
    //                           Material{std::in_place_type<Metal>, dColor{0.8, 0.8, 0.8}, Reflection{0.3}}};

    auto left_sphere = Sphere{dPoint3{-1, 0, -1}, Radius{0.5},
                              Material{std::in_place_type<Dielectric>, RefractionIndex{1.00 / 1.33}}};

    auto right_sphere = Sphere{dPoint3{1, 0, -1}, Radius{0.5},
                               Material{std::in_place_type<Metal>, dColor{0.2, 0.2, 0.8}, Reflection{1.0}}};

    // Init World
    auto world = World{};
    world.Add(center_sphere);
    world.Add(big_bottom_sphere);
    world.Add(left_sphere);
    world.Add(right_sphere);

    // Init Camera
    using AspectRatio = f64;
    using ImageWidth = std::size_t;
    using SamplesPerPixel = std::size_t;
    auto camera = Camera{AspectRatio{16.0 / 9.0}, ImageWidth{400}, SamplesPerPixel{100}};

    // camera.RenderPass(world);
    camera.AntialiasingRenderPass(world);

    // Log::Info("Hello, world!");
}
