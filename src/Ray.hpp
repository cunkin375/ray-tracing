#pragma once

#include "Math/Vector.hpp"

struct Ray {
    Point3 origin;
    Vector3 direction;

    Ray() = default;

    Ray(const Point3 &_origin, const Vector3 &_direction) : origin{_origin}, direction{_direction} {}

    Point3 At(double scalar) const {
        return origin + scalar * direction;
    }

};

struct dRay {
    dPoint3 origin;
    dVector3 direction;

    dRay() = default;

    dRay(const dPoint3 &_origin, const dVector3 &_direction) : origin{_origin}, direction{_direction} {}

    dPoint3 At(double t) const {
        return origin + t * direction;
    }

};
