#include <iostream>
#include <cmath>
#include "convert.hpp"

std::array<double, 3> rgb2xyz(unsigned char r, unsigned char g, unsigned char b) {
    double R = static_cast<double>(r) / 255.0;
    double G = static_cast<double>(g) / 255.0;
    double B = static_cast<double>(b) / 255.0;

    if (R > 0.04045) 
        R = std::pow((R + 0.055) / 1.055, 2.4);
    else 
        R = R / 12.92;

    if (G > 0.04045)
        G = std::pow((G + 0.055) / 1.055, 2.4);
    else 
        G = G / 12.92;

    if (B > 0.04045)
        B = std::pow((B + 0.055) / 1.055, 2.4);
    else 
        B = B / 12.92;

    R *= 100;
    G *= 100;
    B *= 100;

    double X = R * 0.4124 + G * 0.3576 + B * 0.1805;
    double Y = R * 0.2126 + G * 0.7152 + B * 0.0722;
    double Z = R * 0.0193 + G * 0.1192 + B * 0.9505;

    return {X, Y, Z};
}

std::array<double, 3> xyz2lab(double X, double Y, double Z) {
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

std::vector<std::array<double, 3>> hex_to_lab(const nlohmann::json& palette) {
    std::vector<std::array<double, 3>> lab_colors;
    
    for (auto& [key, hex_color] : palette.items()) {
        int r, g, b;
        std::sscanf(hex_color.get<std::string>().c_str(), "#%02x%02x%02x", &r, &g, &b);

        std::array<double, 3> xyz = rgb2xyz(r, g, b);
        std::array<double, 3> lab = xyz2lab(xyz[0], xyz[1], xyz[2]);

        lab_colors.push_back(lab);
    }

    return lab_colors;
}