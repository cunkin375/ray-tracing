#include "Material.hpp"

#include "Ray.hpp"

#include "Math/Vector.hpp"
#include "Util/Aliases.hpp"

namespace {
using Attenuation = dColor;
using ScatteredRay = dRay;

} // namespace

/*** Lambertian Material functions ***/
std::optional<ScatterRecord> Lambertian::Scatter(const dRay &ray_in, const dVector3 &surface_hit_normal,
                                                 const dPoint3 &end_point) const {

    // Correct Lambertian Reflection
    [[maybe_unused]] auto scatter_direction = surface_hit_normal + dVector3::GenerateRandomUnitVector();

    // Uniform Lambertian Reflection
    // [[maybe_unused]] dVector3 scatter_direction = dVector3::RandomUnitVectorOnHemisphere(surface_hit_normal);

    if (scatter_direction.HasNearZeroFloatPointPrecision()) { scatter_direction = surface_hit_normal; }
    return ScatterRecord{Attenuation{albedo_}, ScatteredRay{end_point, scatter_direction}};
}

/*** Metal Material functions ***/
std::optional<ScatterRecord> Metal::Scatter(const dRay &ray_in, const dVector3 &surface_hit_normal,
                                            const dPoint3 &end_point) const {

    auto reflected_ray = dVector3::ReflectFromSurfaceNormal(ray_in.direction, surface_hit_normal);
    reflected_ray = dVector3::UnitVector(reflected_ray) + (fuzz_ * dVector3::GenerateRandomUnitVector());
    return ScatterRecord{Attenuation{albedo_}, ScatteredRay{end_point, reflected_ray}};
}
