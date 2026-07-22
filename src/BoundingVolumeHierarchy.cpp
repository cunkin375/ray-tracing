#include "Hittable.hpp"

#include <algorithm>

#include "Math/Random.hpp"
#include "Util/Aliases.hpp"

class BVH_Node {
private:
    AABB<f64> bounding_box_;
    std::unique_ptr<BVH_Node> left_, right_;
    // for leaf nodes
    std::size_t reference_start, reference_end;

public:
    BVH_Node(World &world) {}

    BVH_Node(std::vector<HittableReference> &refs, std::size_t start, std::size_t end) {
        auto axis = Math::Rand::GenerateRandomNumber<std::size_t>(0, 2);
        const auto &comparator = [axis](const HittableReference &a, const HittableReference &b) {
            return a.bounding_box.AxisInterval(axis).lower < b.bounding_box.AxisInterval(axis).lower;
        };

        auto span = end - start;
        if (span == 1) {
            // leaf : store ref range
        } else {
            std::sort(refs.begin() + start, refs.begin() + end, comparator);
            auto middle = start + span / 2;
            left_ = std::make_unique<BVH_Node>(refs, start, middle);
            right_ = std::make_unique<BVH_Node>(refs, start, middle);
            bounding_box_ = AABB{left_->bounding_box_, right_->bounding_box_};
        }
    }
};
