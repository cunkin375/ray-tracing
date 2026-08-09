#include "Scenes.hpp"

#include "Util/Aliases.hpp"

int main() {
    auto [world, camera] = Scenes::InitiateManySphereScene();

    // camera.RenderPass(world);
    camera.AntialiasingRenderPass(world);
}
