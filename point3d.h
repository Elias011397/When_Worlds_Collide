#ifndef POINT3D_H
#define POINT3D_H

#include <iostream>

struct Point3D {
    double x;
    double y;
    double z;
};

inline bool operator==(const Point3D& lhs, const Point3D& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

inline bool operator!=(const Point3D& lhs, const Point3D& rhs) {
    return !(lhs == rhs);
}

inline std::ostream& operator<<(std::ostream& os, const Point3D& point) {
    os << "{" << point.x << ", " << point.y << ", " << point.z << "}";
    return os;
}

#endif
