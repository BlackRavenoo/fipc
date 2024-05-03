#include <algorithm>
#include <limits>
#include <thread>
#include <future>
#include <array>
#include <fstream>
#include "ciede2000.hpp"
#include "palette.hpp"

std::array<double, 3> nearest_palette(
    const std::array<double, 3>& color, 
    const std::vector<std::array<double, 3>>& palette
) {
    int num_threads = std::thread::hardware_concurrency();
    std::vector<std::future<std::pair<double, std::array<double, 3>>>> futures(num_threads);

    int chunk_size = palette.size() / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        int start = i * chunk_size;
        int end = (i == num_threads - 1) ? palette.size() : start + chunk_size;

        futures[i] = std::async(std::launch::async, [color, start, end, &palette]() {
            double min_distance = std::numeric_limits<double>::max();
            std::array<double, 3> nearest_color;

            for (int j = start; j < end; ++j) {
                double delta = ciede2000(color, palette[j]);

                if (delta < min_distance) {
                    min_distance = delta;
                    nearest_color = palette[j];
                }
            }

            return std::make_pair(min_distance, nearest_color);
        });
    }

    double min_distance = std::numeric_limits<double>::max();
    std::array<double, 3> new_color;

    for (auto& future : futures) {
        auto [distance, color] = future.get();
        if (distance < min_distance) {
            min_distance = distance;
            new_color = color;
        }
    }

    return new_color;
}

nlohmann::json read_palette(const Palette palette) {
    switch (palette) {
        case Palette::TokyoNight:
            return read_palette_from_file("./palettes/tokyo_night.json");
        case Palette::Dracula:
            return read_palette_from_file("./palettes/dracula.json");
        case Palette::Solarized:
            return read_palette_from_file("./palettes/solarized.json");
        case Palette::Nord:
            return read_palette_from_file("./palettes/nord.json");
        case Palette::Gruvbox:
            return read_palette_from_file("./palettes/gruvbox.json");
        case Palette::RosePine:
            return read_palette_from_file("./palettes/rose_pine.json");
        case Palette::MaterialSakura:
            return read_palette_from_file("./palettes/material_sakura.json");
        case Palette::Catppuccin:
            return read_palette_from_file("./palettes/catpuccin.json");
        default:
            throw std::runtime_error("Invalid palette");
    }
}

nlohmann::json read_palette_from_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open palette file: " + filename);
    }
    nlohmann::json palette;
    file >> palette;
    return palette;
}