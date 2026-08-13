#include "Scenes.hpp"

#include "Util/Aliases.hpp"

int main() {
    auto [world, camera] = Scenes::InitiateTwoCheckeredSpheres();

    // camera.RenderPass(world);
    camera.AntialiasingRenderPass(world);
}
