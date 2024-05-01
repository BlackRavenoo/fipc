#include <array>

std::array<double, 3> rgb2xyz(
    unsigned char r, unsigned char g, unsigned char b
);

std::array<double, 3> xyz2lab(
    double X, double Y, double Z
);