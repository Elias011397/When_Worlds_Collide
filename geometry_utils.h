#ifndef GEOMETRY_UTILS_H
#define GEOMETRY_UTILS_H

#include "point3d.h"
#include "sphere.h"

enum class SphereOverlapStatus {
    NoOverlap,
    SurfacesTouching,
    PartialOverlap,
    FullyEncapsulated
};

inline std::ostream& operator<<(std::ostream& os, const SphereOverlapStatus& sphere_overlap_status) {
    os << "SphereOverlapStatus::";
    switch (sphere_overlap_status) {
        case SphereOverlapStatus::NoOverlap:
            os << "NoOverlap";
            break;
        case SphereOverlapStatus::SurfacesTouching:
            os << "SurfacesTouching";
            break;
        case SphereOverlapStatus::PartialOverlap:
            os << "PartialOverlap";
            break;
        case SphereOverlapStatus::FullyEncapsulated:
            os << "FullyEncapsulated";
            break;
        default:
            os << "UnknownState";
    }
    return os;
}

struct SphereOverlapCentroidDetails {
    SphereOverlapStatus sphere_overlap_status{SphereOverlapStatus::NoOverlap};
    Point3D centroid_of_overlap{0., 0., 0.};
    Point3D centroid_normal_vector{0., 0., 0.};
};

inline std::ostream& operator<<(std::ostream& os, const SphereOverlapCentroidDetails& sphere_overlap_centroid_details) {
    os << "{sphere_overlap_status=" << sphere_overlap_centroid_details.sphere_overlap_status
       << ", centroid_of_overlap=" << sphere_overlap_centroid_details.centroid_of_overlap << ", centroid_normal_vector"
       << sphere_overlap_centroid_details.centroid_normal_vector << "}";
    return os;
}

double CalculateEuclidianDistance(const Point3D& lhs, const Point3D& rhs);

Point3D CalculateMidpoint(const Point3D& lhs, const Point3D& rhs);

// A function to calculate the unit normal vector between two points
Point3D CalculateUnitNormalVector(const Point3D& lhs, const Point3D& rhs);

// uses distance formula for sphere centers and compares to combined radii
bool IsCollision(const Sphere& lhs, const Sphere& rhs);

SphereOverlapCentroidDetails CalculateCentroid(const Sphere& lhs, const Sphere& rhs);

void CalculateCentroidThenPrintResults(const Sphere& lhs, const Sphere& rhs);

#endif
