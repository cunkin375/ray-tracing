#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Hittable.hpp"

#include "Math/Vector.hpp"
#include "Util/Aliases.hpp"

class Camera {
public:
    f64 aspect_ratio{16.0 / 9.0};
    std::size_t image_width{400};
    std::size_t samples_per_pixel{10};
    std::size_t max_depth{10};

    f64 vertical_fov{90};
    dPoint3 look_from{0, 0, 0};
    dPoint3 look_at{0, 0, -1};
    dVector3 vertical_up{0, 1, 0};

    f64 defocus_angle{0};
    f64 focus_distance{10};

private:
    std::size_t image_height_{};
    f64 pixel_samples_scale_{};
    dPoint3 camera_center_{};
    dPoint3 pixel_00_location_{};
    dVector3 pixel_delta_u_;
    dVector3 pixel_delta_v_;
    dVector3 u, v, w;
    dVector3 defocus_disk_u;
    dVector3 defocus_disk_v;

public:
    Camera(f64 _aspect_ratio, std::size_t _image_width);
    Camera(f64 _aspect_ratio, std::size_t _image_width, std::size_t _samples_per_pixel);
    Camera(f64 _aspect_ratio, std::size_t _image_width, std::size_t _samples_per_pixel, f64 _vertical_fov);

    template <typename WorldType>
    void RenderPass(const WorldType &world);
    template <typename WorldType>
    void AntialiasingRenderPass(const WorldType &world);

private:
    dRay GetRay(std::size_t i, std::size_t j) const;
    dVector3 SampleSquare() const;
    dPoint3 DefocusDiskSample() const;

    template <typename WorldType>
    dColor RayToColor(const dRay &ray, std::size_t depth, const WorldType &world) const;

    void InitializePass();
};

#define CAMERA_TPP_INSIDE_
#include "Camera.tpp"
#undef CAMERA_TPP_INSIDE_

#endif // CAMERA_HPP
