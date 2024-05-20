#include <algorithm>
#include <cmath>
#include "convert.hpp"

inline std::array<double, 3> rgb2xyz(unsigned char r, unsigned char g, unsigned char b) {
    double R = static_cast<double>(r) / 255.0;
    double G = static_cast<double>(g) / 255.0;
    double B = static_cast<double>(b) / 255.0;

    if (R > 0.04045) 
        R = std::pow((R + 0.055) / 1.055, 2.4);
    else 
        R /= 12.92;

    if (G > 0.04045)
        G = std::pow((G + 0.055) / 1.055, 2.4);
    else 
        G /= 12.92;

    if (B > 0.04045)
        B = std::pow((B + 0.055) / 1.055, 2.4);
    else 
        B /= 12.92;

    R *= 100;
    G *= 100;
    B *= 100;

    double X = R * 0.4124 + G * 0.3576 + B * 0.1805;
    double Y = R * 0.2126 + G * 0.7152 + B * 0.0722;
    double Z = R * 0.0193 + G * 0.1192 + B * 0.9505;

    return {X, Y, Z};
}

inline std::array<double, 3> xyz2lab(double X, double Y, double Z) {
    double x = X / 95.047;
    double y = Y / 100.0;
    double z = Z / 108.883;

    x = (x > 0.008856) ? std::pow(x, 1.0 / 3.0) : (7.787 * x) + (16.0 / 116.0);
    y = (y > 0.008856) ? std::pow(y, 1.0 / 3.0) : (7.787 * y) + (16.0 / 116.0);
    z = (z > 0.008856) ? std::pow(z, 1.0 / 3.0) : (7.787 * z) + (16.0 / 116.0);

    double L = (116.0 * y) - 16.0;
    double a = 500.0 * (x - y);
    double b = 200.0 * (y - z);

    return {L, a, b};
}

std::array<double, 3> hex2lab(const std::string& hex_color) {
    int r, g, b;
    std::sscanf(hex_color.c_str(), "#%02x%02x%02x", &r, &g, &b);

    std::array<double, 3> xyz = rgb2xyz(r, g, b);
    std::array<double, 3> lab = xyz2lab(xyz[0], xyz[1], xyz[2]);

    return lab;
}

std::array<double, 3> rgb2lab(unsigned char r, unsigned char g, unsigned char b) {
    std::array<double, 3> xyz = rgb2xyz(r, g, b);

    std::array<double, 3> lab = xyz2lab(xyz[0], xyz[1], xyz[2]);

    return lab;
}

std::array<int, 3> lab2rgb(double l, double a, double B) {
    double y = (l + 16) / 116;
    double x = a / 500 + y;
    double z = y - B / 200;

    x = 95.047 * ((x > 0.206897) ? x * x * x : (x - 16.0 / 116) / 7.787);
    y = 100.000 * ((y > 0.206897) ? y * y * y : (y - 16.0 / 116) / 7.787);
    z = 108.883 * ((z > 0.206897) ? z * z * z : (z - 16.0 / 116) / 7.787);

    x /= 100;
    y /= 100;
    z /= 100;

    double r = x * 3.2406 + y * -1.5372 + z * -0.4986;
    double g = x * -0.9689 + y * 1.8758 + z * 0.0415;
    double b = x * 0.0557 + y * -0.2040 + z * 1.0570;

    r = (r > 0.0031308) ? 1.055 * std::pow(r, 1 / 2.4) - 0.055 : 12.92 * r;
    g = (g > 0.0031308) ? 1.055 * std::pow(g, 1 / 2.4) - 0.055 : 12.92 * g;
    b = (b > 0.0031308) ? 1.055 * std::pow(b, 1 / 2.4) - 0.055 : 12.92 * b;

    auto clamp = [](double value) {
        return static_cast<int>(std::round(std::clamp(value, 0.0, 1.0) * 255));
    };

    return {clamp(r), clamp(g), clamp(b)};
}