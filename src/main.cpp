#include "Camera.hpp"
#include "Material.hpp"
#include "Sphere.hpp"
#include "Types.hpp"

#include "Math/Vector.hpp"
#include "Util/Aliases.hpp"

int main() {
    // Init World
    auto world = World{};

    // Init World Objects
    using Radius = f64;
    using Reflection = f64;
    using RefractionIndex = f64;

    auto center_sphere = Sphere{dPoint3{0, 0, -1.2}, Radius{0.5},
                                Material{std::in_place_type<Lambertian>, dColor{0.8, 0.0, 0.2}}};

    auto big_bottom_sphere = Sphere{dPoint3{0, -100.5, -1}, Radius{100},
                                    Material{std::in_place_type<Lambertian>, dColor{0.8, 0.8, 0.0}}};

    auto left_sphere = Sphere{dPoint3{-1, 0, -1}, Radius{0.5},
                              Material{std::in_place_type<Dielectric>, RefractionIndex{1.50}}};

    auto left_sphere_bubble = Sphere{dPoint3{-1, 0, -1}, Radius{0.4},
                                     Material{std::in_place_type<Dielectric>, RefractionIndex{1.00 / 1.50}}};

    auto right_sphere = Sphere{dPoint3{1, 0, -1}, Radius{0.5},
                               Material{std::in_place_type<Metal>, dColor{0.2, 0.2, 0.8}, Reflection{1.0}}};

    world.Add(center_sphere);
    world.Add(big_bottom_sphere);
    world.Add(left_sphere);
    world.Add(left_sphere_bubble);
    world.Add(right_sphere);

    // auto R = std::cos(std::numbers::pi / 4);
    //
    // auto sphere_left =
    //     Sphere{dPoint3{-R, 0, -1}, Radius{R}, Material{std::in_place_type<Lambertian>, dColor{0, 0, 1}}};
    //
    // auto sphere_right =
    //     Sphere{dPoint3{R, 0, -1}, Radius{R}, Material{std::in_place_type<Lambertian>, dColor{1, 0, 0}}};
    //
    // world.Add(sphere_left);
    // world.Add(sphere_right);

    // Init Camera
    using AspectRatio = f64;
    using ImageWidth = std::size_t;
    using SamplesPerPixel = std::size_t;
    using VerticalFOV = f64;

    auto camera = Camera{AspectRatio{16.0 / 9.0}, ImageWidth{400}, SamplesPerPixel{100}, VerticalFOV{20}};

    camera.look_from = dPoint3{-2, 2, 1};
    camera.look_at = dPoint3{0, 0, -1};
    camera.vertical_up = dVector3{0, 1, 0};

    // camera.RenderPass(world);
    camera.AntialiasingRenderPass(world);
}
