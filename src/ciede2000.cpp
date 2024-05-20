#include <cmath>
#include "ciede2000.hpp"

const double C_25_7 = 6103515625.0;

constexpr double radians_to_degrees(const double radians) {
    return radians * (180.0 / M_PI);
}

constexpr double degrees_to_radians(const double degrees) {
    return degrees * (M_PI / 180.0);
}

double ciede2000(
    const std::array<double, 3>& lab1,
    const std::array<double, 3>& lab2,
    const std::array<double, 3> factors
) {
    const double luminanceFactor = factors[0];
    const double chromaFactor = factors[1];
    const double hueFactor = factors[2];

    const double L1 = lab1[0];
    const double a1 = lab1[1];
    const double b1 = lab1[2];

    const double L2 = lab2[0];
    const double a2 = lab2[1];
    const double b2 = lab2[2];

    const double C1 = std::hypot(a1, b1);
    const double C2 = std::hypot(a2, b2);
    const double C_avg = (C1 + C2) / 2.0;
    const double C_avg7 = std::pow(C_avg, 7);
    const double G = 0.5 * (1.0 - sqrt(C_avg7 / (C_avg7 + C_25_7)));

    const double a1_prime = (1.0 + G) * a1;
    const double a2_prime = (1.0 + G) * a2;

    double h1 = (b1 == 0 && a1_prime == 0) ? 0.0 : atan2(b1, a1_prime);
    double h2 = (b2 == 0 && a2_prime == 0) ? 0.0 : atan2(b2, a2_prime);

    h1 = (h1 >= 0) ? radians_to_degrees(h1) : radians_to_degrees(h1) + 360.0;
    h2 = (h2 >= 0) ? radians_to_degrees(h2) : radians_to_degrees(h2) + 360.0;

    const double C1_prime = std::hypot(a1_prime, b1);
    const double C2_prime = std::hypot(a2_prime, b2);
    
    const double delta_L_prime = L2 - L1;
    const double delta_C_prime = C2_prime - C1_prime;

    const double delta_h_prime = (C1_prime * C2_prime == 0) ? 0 : (std::abs(h2 - h1) <= 180) ? h2 - h1 : (h2 <= h1) ? h2 - h1 + 360 : h2 - h1 - 360;

    const double delta_H_prime = 2 * sqrt(C1_prime * C2_prime) * sin(degrees_to_radians(delta_h_prime) / 2.0);

    const double L_ = (L1 + L2) / 2.0;

    const double C_ = (C1_prime + C2_prime) / 2.0;

    double h_prime_avg;

    if (C1_prime * C2_prime == 0) {
        h_prime_avg = h1 + h2;
    } else {
        h_prime_avg = (std::abs(h2 - h1) <= 180) ? (h1 + h2) / 2.0 : (h1 + h2 + 360) / 2.0;
    }

    const double T = 1 - 0.17 * cos(degrees_to_radians(h_prime_avg - 30)) + 0.24 * cos(degrees_to_radians(2 * h_prime_avg)) + 0.32 * cos(degrees_to_radians(3 * h_prime_avg + 6)) - 0.20 * cos(degrees_to_radians(4 * h_prime_avg - 63));

    const double L_temp = (L_ - 50) * (L_ - 50);

    const double S_L = 1 + (0.015 * L_temp) / sqrt(20 + L_temp);

    const double S_C = 1 + 0.045 * C_;

    const double S_H = 1 + 0.015 * C_ * T;

    const double delta_theta = 30 * exp(-((h_prime_avg - 275) / 25) * ((h_prime_avg - 275) / 25));

    const double C_7 = std::pow(C_, 7);
    const double R_T = -std::sin(degrees_to_radians(2 * delta_theta)) * 2 * std::sqrt(C_7 / (C_7 + C_25_7));

    const double delta_L = delta_L_prime / (luminanceFactor * S_L);

    const double delta_C = delta_C_prime / (chromaFactor * S_C);

    const double delta_H = delta_H_prime / (hueFactor * S_H);

    const double delta_E = sqrt(pow(delta_L, 2) + pow(delta_C, 2) + pow(delta_H, 2) + R_T * delta_C * delta_H);

    return delta_E;
}