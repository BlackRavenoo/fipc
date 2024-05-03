#include <array>
#include <string>

std::array<double, 3> rgb2xyz(
    unsigned char r, unsigned char g, unsigned char b
);

std::array<double, 3> xyz2lab(
    double X, double Y, double Z
);

std::array<double, 3> rgb2lab(
    unsigned char r, unsigned char g, unsigned char b
);

std::array<double, 3> hex2lab(const std::string& hex_color);

std::array<int, 3> lab2rgb(double l, double a, double b);