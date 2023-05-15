#include "geometry_utils.h"
#include <cmath>

namespace {
double CalculateCapHeight(const Sphere& lhs, const Sphere& rhs, const double distance_between_centers) {
    const auto square_distance_between_centers = distance_between_centers * distance_between_centers;
    const auto square_lhs_radius = lhs.radius * lhs.radius;
    const auto square_rhs_radius = rhs.radius * rhs.radius;

    return lhs.radius
         - (square_distance_between_centers - square_lhs_radius + square_rhs_radius) / (2. * distance_between_centers);
}

Point3D CalculateCentroidWhenPartiallyOverlapping(const Sphere& lhs, const Sphere& rhs) {
    const auto weighed_average_in_one_dimension
        = [](const double radius1, const double radius2, const double point1, const double point2) {
              return (point1 * radius2 + point2 * radius1) / (radius1 + radius2);
          };

    const double x = weighed_average_in_one_dimension(lhs.radius, rhs.radius, lhs.center.x, rhs.center.x);
    const double y = weighed_average_in_one_dimension(lhs.radius, rhs.radius, lhs.center.y, rhs.center.y);
    const double z = weighed_average_in_one_dimension(lhs.radius, rhs.radius, lhs.center.z, rhs.center.z);

    return {x, y, z};
}
} // namespace

double CalculateEuclidianDistance(const Point3D& lhs, const Point3D& rhs) {
    const double x_distance = lhs.x - rhs.x;
    const double y_distance = lhs.y - rhs.y;
    const double z_distance = lhs.z - rhs.z;
    return std::sqrt(x_distance * x_distance + y_distance * y_distance + z_distance * z_distance);
}

Point3D CalculateMidpoint(const Point3D& lhs, const Point3D& rhs) {
    return {(lhs.x + rhs.x) / 2., (lhs.y + rhs.y) / 2., (lhs.z + rhs.z) / 2.};
}

Point3D CalculateUnitNormalVector(const Point3D& lhs, const Point3D& rhs) {
    const double dx = rhs.x - lhs.x;
    const double dy = rhs.y - lhs.y;
    const double dz = rhs.z - lhs.z;
    const double magnitude = std::sqrt(dx * dx + dy * dy + dz * dz);

    return magnitude == 0. ? Point3D{0., 0., 0.} : Point3D{dx / magnitude, dy / magnitude, dz / magnitude};
}

// uses distance formula for sphere centers and compares to combined radii
bool IsCollision(const Sphere& lhs, const Sphere& rhs) {
    const double distance
        = std::sqrt(std::pow((rhs.center.x - lhs.center.x), 2) + std::pow((rhs.center.y - lhs.center.y), 2)
                    + std::pow((rhs.center.z - lhs.center.z), 2));
    const double combinedRadii = lhs.radius + rhs.radius;
    return distance <= combinedRadii;
}

SphereOverlapCentroidDetails CalculateCentroid(const Sphere& lhs, const Sphere& rhs) {
    const double distance_between_centers = CalculateEuclidianDistance(lhs.center, rhs.center);
    if (distance_between_centers > lhs.radius + rhs.radius) {
        // if no overlap
        return {SphereOverlapStatus::NoOverlap, {}, {}};
    }

    const auto centroid_normal_vector = CalculateUnitNormalVector(lhs.center, rhs.center);

    if (distance_between_centers == lhs.radius + rhs.radius) {
        // if surfaces touching
        const auto centroid = CalculateCentroidWhenPartiallyOverlapping(lhs, rhs);
        return {SphereOverlapStatus::SurfacesTouching, centroid, centroid_normal_vector};
    }

    if (distance_between_centers <= std::fabs(lhs.radius - rhs.radius)) {
        // if one sphere is fully contained in another
        const auto centroid = lhs.radius < rhs.radius ? lhs.center : rhs.center;
        return {SphereOverlapStatus::FullyEncapsulated, centroid, centroid_normal_vector};
    }

    // if there is partial overlap of the spheres
    const auto centroid = CalculateCentroidWhenPartiallyOverlapping(lhs, rhs);
    return {SphereOverlapStatus::PartialOverlap, centroid, centroid_normal_vector};
}

void CalculateCentroidThenPrintResults(const Sphere& lhs, const Sphere& rhs) {
    const auto sphere_overlap_centroid_details = CalculateCentroid(lhs, rhs);

    if (sphere_overlap_centroid_details.sphere_overlap_status == SphereOverlapStatus::NoOverlap) {
        std::cerr << "For Sphere" << lhs << " and Sphere" << rhs << ":"
                  << "\n    sphere_overlap_status=" << sphere_overlap_centroid_details.sphere_overlap_status
                  << std::endl;
    } else {
        std::cerr << "For Sphere" << lhs << " and Sphere" << rhs << ":"
                  << "\n    sphere_overlap_status=" << sphere_overlap_centroid_details.sphere_overlap_status
                  << "\n    centroid_of_overlap=" << sphere_overlap_centroid_details.centroid_of_overlap
                  << "\n    centroid_normal_vector=" << sphere_overlap_centroid_details.centroid_normal_vector
                  << std::endl;
    }
}
