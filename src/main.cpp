#include "Scenes.hpp"

#include "Math/Vector.hpp"
#include "Util/Aliases.hpp"

int main() {
    // Init World
    auto world = Scenes::InitiateManySphereScene();

    // Init Camera
    using AspectRatio = f64;
    using ImageWidth = std::size_t;
    using SamplesPerPixel = std::size_t;
    using VerticalFOV = f64;

    auto camera = Camera{AspectRatio{16.0 / 9.0}, ImageWidth{1080}, SamplesPerPixel{25}, VerticalFOV{20}};

    camera.look_from = dPoint3{13, 2, 3};
    camera.look_at = dPoint3{0, 0, 0};
    camera.vertical_up = dVector3{0, 1, 0};

    camera.defocus_angle = 0.6;
    camera.focus_distance = 10.0;

    // camera.RenderPass(world);
    camera.AntialiasingRenderPass(world);
}
