#include "Material.hpp"

#include "HittableList.hpp"
#include "Ray.hpp"

#include "Math/Vector.hpp"
#include "Util/Aliases.hpp"

namespace {
using Attenuation = dColor;
using ScatteredRay = dRay;

} // namespace

/*** Lambertian Material functions ***/
std::optional<ScatterRecord> Lambertian::Scatter(const dRay &ray_in, const HitRecord &record) const {
    // Correct Lambertian Reflection
    [[maybe_unused]] auto scatter_direction = record.normal + dVector3::GenerateRandomUnitVector();
    // Uniform Lambertian Reflection
    // [[maybe_unused]] dVector3 scatter_direction =
    // dVector3::RandomUnitVectorOnHemisphere(surface_hit_normal);
    if (scatter_direction.HasNearZeroFloatPointPrecision()) { scatter_direction = record.normal; }
    return ScatterRecord{Attenuation{albedo_}, ScatteredRay{record.end_point, scatter_direction}};
}

/*** Metal Material functions ***/
std::optional<ScatterRecord> Metal::Scatter(const dRay &ray_in, const HitRecord &record) const {
    auto reflected_ray = dVector3::ReflectFromSurfaceNormal(ray_in.direction, record.normal);
    reflected_ray = dVector3::UnitVector(reflected_ray) + (fuzz_ * dVector3::GenerateRandomUnitVector());
    return ScatterRecord{Attenuation{albedo_}, ScatteredRay{record.end_point, reflected_ray}};
}

/*** Dialectric Material functions ***/
std::optional<ScatterRecord> Dielectric::Scatter(const dRay &ray_in, const HitRecord &record) const {
    double final_refraction_index = record.front_face ? (1.0 / refraction_index_) : refraction_index_;
    auto unit_direction = dVector3::UnitVector(ray_in.direction);
    auto refracted_ray =
        dVector3::RefractFromSurfaceNormal(unit_direction, record.normal, final_refraction_index);
    return ScatterRecord{Attenuation{1.0, 1.0, 1.0}, ScatteredRay{record.end_point, refracted_ray}};
}
