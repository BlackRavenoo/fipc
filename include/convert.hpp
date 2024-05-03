#include <array>
#include <vector>
#include <nlohmann/json.hpp>

std::array<double, 3> rgb2xyz(
    unsigned char r, unsigned char g, unsigned char b
);

std::array<double, 3> xyz2lab(
    double X, double Y, double Z
);

std::array<double, 3> rgb2lab(
    unsigned char r, unsigned char g, unsigned char b
);

std::vector<std::array<double, 3>> hex_to_lab(const nlohmann::json& palette);