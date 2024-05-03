#include <array>
#include <iostream>
#include <iomanip>
#include "ciede2000.hpp"
#include "convert.hpp"
#include "palette.hpp"
#include "cli.hpp"
#include "image.hpp"

int main(int argc, char *argv[]) {
    Args args = get_args(argc, argv);

    std::cout << args.image << std::endl;

    std::cout << args.palette << std::endl;

    unsigned char r1 = 255, g1 = 0, b1 = 0;
    unsigned char r2 = 0, g2 = 255, b2 = 0;

    std::array<double, 3> xyz1 = rgb2xyz(r1, g1, b1);
    std::array<double, 3> xyz2 = rgb2xyz(r2, g2, b2);

    std::array<double, 3> lab1 = xyz2lab(xyz1[0], xyz1[1], xyz1[2]);
    std::array<double, 3> lab2 = xyz2lab(xyz2[0], xyz2[1], xyz2[2]);

    std::cout << "Цвет 1 (RGB): " << static_cast<int>(r1) << ", " << static_cast<int>(g1) << ", " << static_cast<int>(b1) << std::endl;
    std::cout << "Цвет 1 (LAB): " << lab1[0] << ", " << lab1[1] << ", " << lab1[2] << std::endl;

    std::cout << "Цвет 2 (RGB): " << static_cast<int>(r2) << ", " << static_cast<int>(g2) << ", " << static_cast<int>(b2) << std::endl;
    std::cout << "Цвет 2 (LAB): " << lab2[0] << ", " << lab2[1] << ", " << lab2[2] << std::endl;
    
    std::array<double, 3> lab1_a = {lab1[0], lab1[1], lab1[2]};
    std::array<double, 3> lab2_a = {lab2[0], lab2[1], lab2[2]};
    double ciede2000Result = ciede2000(lab1_a, lab2_a);
    std::cout << std::fixed << std::setprecision(15) << ciede2000Result << std::endl;

    auto palette = read_palette(Palette::Catppuccin)["Latte"];

    auto lab_colors = hex_to_lab(palette);

    auto res = nearest_palette(lab1_a, lab_colors);

    std::cout << res[0] << " " << res[1] << " " << res[2] << std::endl;

    Image image("anime-city-2.jpg");

    std::cout << image.width << " " << image.height << " " << image.channels << image.size << std::endl;

    image.write("output.png");
    
    return 0;
}