#pragma once

#include "Types.hpp"

#include "Math/Vector.hpp"
#include "Util/Aliases.hpp"

class Camera {
public:
    f64 aspect_ratio{16.0 / 9.0};
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
    Camera(f64 _aspect_ratio, std::size_t _image_width);
    Camera(f64 _aspect_ratio, std::size_t _image_width, std::size_t _samples_per_pixel);

    void RenderPass(const World &world);
    void AntialiasingRenderPass(const World &world);

private:
    dRay GetRay(std::size_t i, std::size_t j) const;
    dVector3 SampleSquare() const;
    dColor RayToColor(const dRay &ray, std::size_t depth, const World &world) const;

    void InitializePass();
};
