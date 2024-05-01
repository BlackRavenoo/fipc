#include <array>

double ciede2000(
    const std::array<double, 3>& lab1,
    const std::array<double, 3>& lab2,
    std::array<double, 3> factors = {1.0, 1.0, 1.0}
);