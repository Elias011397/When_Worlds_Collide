#include "geometry_utils.h"
#include "sphere.h"
#include <iostream>
#include <string>

bool CheckIfHelp(int argc, char* argv[]) {
    std::cout << argc << std::endl;
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help") {
            return true;
        }
    }
    return false;
}

void PrintHelpText(const std::string& program_name) {
    std::cerr << program_name;
    std::cerr << "\nThis Program must take in 8 arguments which define two spheres.";
    std::cerr
        << "\nAll arguments are cast to doubles, where x, y, and z arguments define the sphere center followed by the sphere's radius.";
    std::cerr << "\nInvoke this program as follows:\n"
              << program_name << " x1 y1 z1 radius1 x2 y2 z2 radius2" << std::endl;
    std::cerr << std::string(program_name.size(), ' ') << "|    Sphere 1    |    Sphere 2    |" << std::endl;
}

bool AreArgsValid(int argc, char* argv[]) {
    static constexpr int expectedNumArgs = 8;
    const int actualNumArgs = argc - 1;
    if (actualNumArgs != expectedNumArgs) {
        std::cerr << "ERROR: Incorrect number of arguments; Expected=" << expectedNumArgs << " Actual=" << actualNumArgs
                  << std::endl;
        return false;
    }
    // TODO: add check to ensure all numbers are double convertable
    return true;
}

void PrintIsCollision(const Sphere& s1, const Sphere& s2) {
    std::cout << "Sphere" << s1 << " and Sphere" << s2 << " ";
    if (IsCollision(s1, s2)) {
        std::cout << "do collide";
    } else {
        std::cout << "do NOT collide";
    }
    std::cout << ".\n";
}

int main(int argc, char* argv[]) {
    if (CheckIfHelp(argc, argv)) {
        PrintHelpText(argv[0]);
        return 0;
    }
    if (!AreArgsValid(argc, argv)) {
        PrintHelpText(argv[0]);
        return 1;
    }

    Sphere s1{argv[1], argv[2], argv[3], argv[4]};
    Sphere s2{argv[5], argv[6], argv[7], argv[8]};

    PrintIsCollision(s1, s2);
    std::cout << "\n";
    CalculateCentroidThenPrintResults(s1, s2);
}
