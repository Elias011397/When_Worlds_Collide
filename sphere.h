#ifndef SPHERE_H
#define SPHERE_H

#include "point3d.h"
#include <cstdlib>
#include <iostream>

class Sphere {
  public:
    Sphere(Point3D center, double radius) : center{center}, radius{radius} {};
    Sphere() : Sphere{{0., 0., 0.}, 0.} {};
    Sphere(char* x, char* y, char* z, char* radius) :
        Sphere{{std::atof(x), std::atof(y), std::atof(z)}, std::atof(radius)} {};

    friend std::ostream& operator<<(std::ostream& os, const Sphere& sphere) {
        os << "{center=" << sphere.center << ", radius=" << sphere.radius << "}";
        return os;
    }

    Point3D center;
    double radius;
};

#endif
