#pragma once

#include "AxisAlignedBoundingBox.hpp"
#include "Hittable.hpp"

#include "Util/Aliases.hpp"

class BVH_Node {
private:
    AABB<f64> bounding_box_;
    std::unique_ptr<BVH_Node> left_, right_;
    // for leaf nodes
    std::size_t reference_start, reference_end;

public:
    BVH_Node(World &world);

    BVH_Node(std::vector<HittableReference> &refs, std::size_t start, std::size_t end);
};
