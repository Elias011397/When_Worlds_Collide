#include "geometry_utils.h"
#include "point3d.h"
#include "sphere.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

using IsCollisionTestParams = std::tuple<Sphere, Sphere, bool>;
std::vector<IsCollisionTestParams> is_collision_test_cases{
    {Sphere{{0., 0., 0.}, 2.}, Sphere{{5., 0., 0.}, 3.}, true},
    {Sphere{{0., 0., 0.}, 2.}, Sphere{{8., 0., 0.}, 3.}, false},
    {Sphere{{0., 0., 0.}, 1.}, Sphere{{2., 2., 2.}, 1.}, false},
    {Sphere{{0., 0., 0.}, 2.}, Sphere{{5., 5., 5.}, 3.}, false},
    {Sphere{{0., 0., 0.}, 1.}, Sphere{{1., 0., 0.}, 1.}, true},
    {Sphere{{0., 0., 0.}, 2.}, Sphere{{4., 0., 0.}, 3.}, true},
    {Sphere{{-0.1, -0.1, 0.}, 1.}, Sphere{{1., 1., 0.}, 1.}, true},
    {Sphere{{-1.1, -1.1, 0.}, 1.}, Sphere{{1., 1., 0.}, 1.}, false},
    {Sphere{{0., 0., 0.}, 0.}, Sphere{{0., 0., 0.}, 0.}, true},
    {Sphere{{1., 0., 0.}, 1.}, Sphere{{-1., 0., 0.}, 1.}, true},
    {Sphere{{1., 0., 0.}, 1.}, Sphere{{-1.0001, 0., 0.}, 1.}, false},
};

void TestIsCollision() {
    for (std::size_t i = 0; i < is_collision_test_cases.size(); ++i) {
        const auto test_case = is_collision_test_cases[i];
        const Sphere s1{std::get<0>(test_case)};
        const Sphere s2{std::get<1>(test_case)};
        const bool is_collision{IsCollision(s1, s2)};
        const bool expected_is_collision = std::get<2>(test_case);
        if (is_collision != expected_is_collision) {
            std::cout << "Failed i=" << i << ": Sphere" << s1 << " and Sphere" << s2
                      << "; result is_collision=" << is_collision
                      << ", however expected_is_collision=" << expected_is_collision << std::endl;
            assert(false);
        }
    }
}

using CalculateCentroidTestParams = std::tuple<Sphere, Sphere, SphereOverlapCentroidDetails>;
std::vector<CalculateCentroidTestParams> CalculateCentroidTestCases{
    {Sphere{{0, 0., 0.}, 1.}, Sphere{{5., 5., 0.}, 1.}, SphereOverlapCentroidDetails{SphereOverlapStatus::NoOverlap}},
    {Sphere{{.9, .9, 0.}, 5.},
     Sphere{{1., 1., 0.}, 1.},
     SphereOverlapCentroidDetails{SphereOverlapStatus::FullyEncapsulated, {1., 1., 0.}, {.7071, .7071, 0.}}},
    {Sphere{{0., 0., 0.}, 1.},
     Sphere{{1., 0., 0.}, 1.},
     SphereOverlapCentroidDetails{SphereOverlapStatus::PartialOverlap, {0.5, 0., 0.}, {1., 0., 0.}}},
};

bool IsSphereOverlapCentroidDetailsApproximatelyEqual(const SphereOverlapCentroidDetails& lhs,
                                                      const SphereOverlapCentroidDetails& rhs,
                                                      const double epsilon) {
    if (lhs.sphere_overlap_status != rhs.sphere_overlap_status) {
        return false;
    }

    if (lhs.sphere_overlap_status == SphereOverlapStatus::NoOverlap) {
        return true;
    }

    const auto is_point3d_approximately_equal = [epsilon](const Point3D& lhs, const Point3D& rhs) -> bool {
        return std::abs(lhs.x - rhs.x) < epsilon && std::abs(lhs.y - rhs.y) < epsilon
            && std::abs(lhs.z - rhs.z) < epsilon;
    };

    const auto is_point3d_approximately_equal_including_negative
        = [epsilon](const Point3D& lhs, const Point3D& rhs) -> bool {
        const bool is_positive_approximately_equal = std::abs(lhs.x - rhs.x) < epsilon
                                                  && std::abs(lhs.y - rhs.y) < epsilon
                                                  && std::abs(lhs.z - rhs.z) < epsilon;
        const bool is_negative_approximately_equal = std::abs(-lhs.x - rhs.x) < epsilon
                                                  && std::abs(-lhs.y - rhs.y) < epsilon
                                                  && std::abs(-lhs.z - rhs.z) < epsilon;
        return is_positive_approximately_equal || is_negative_approximately_equal;
    };

    return is_point3d_approximately_equal(lhs.centroid_of_overlap, rhs.centroid_of_overlap)
        && is_point3d_approximately_equal_including_negative(lhs.centroid_normal_vector, rhs.centroid_normal_vector);
};

void TestCalculateCentroid() {
    for (std::size_t i = 0; i < CalculateCentroidTestCases.size(); ++i) {
        const auto test_case = CalculateCentroidTestCases[i];
        const Sphere s1{std::get<0>(test_case)};
        const Sphere s2{std::get<1>(test_case)};
        const auto centroid_details{CalculateCentroid(s1, s2)};
        const auto expected_centroid_details{std::get<2>(test_case)};
        if (!IsSphereOverlapCentroidDetailsApproximatelyEqual(centroid_details, expected_centroid_details, 0.001)) {
            std::cout << "Failed i=" << i << ": Sphere" << s1 << " and Sphere" << s2 << ":"
                      << "\n\tcentroid_details=" << centroid_details
                      << "\n\texpected_centroid_details=" << expected_centroid_details << std::endl;
            assert(false);
        }
    }
}

int main(int argc, char* argv[]) {
    TestIsCollision();
    TestCalculateCentroid();
    std::cerr << argv[0] << ": Passed all tests" << std::endl;
}
