#include <algorithm>
#include <limits>
#include <thread>
#include <array>
#include <fstream>
#include "ciede2000.hpp"
#include "palette.hpp"

//TODO: Remove hardcoded values
std::array<double, 3> nearest_palette(
    const std::array<double, 3>& color, 
    const std::vector<std::array<double, 3>>& palette
) {
    std::array<std::thread, 4> threads;
    std::array<std::array<double, 3>, 4> results;
    std::array<double, 4> min_distances = {
        std::numeric_limits<double>::max(),
        std::numeric_limits<double>::max(),
        std::numeric_limits<double>::max(),
        std::numeric_limits<double>::max()
    };

    for (int i = 0; i < 4; ++i) {
        std::vector<std::array<double, 3>> subset;
        int start = i * 7 - (i > 1 ? i - 1 : 0);
        int end = start + 7 - (i > 1 ? 1 : 0);
        for (int j = start; j < std::min(end, static_cast<int>(palette.size())); ++j) {
            subset.push_back(palette[j]);
        }

        threads[i] = std::thread([&results, &min_distances, i, color, &subset]() {
                for (const auto& palette_color : subset) {
                    double delta = ciede2000(color, palette_color);

                    if (delta < min_distances[i]) {
                        min_distances[i] = delta;
                        results[i] = palette_color;
                    }
                }
            }
        );
    }

    for (auto& thread : threads) {
        thread.join();
    }

    double min_distance = min_distances[0];
    std::array<double, 3> new_color = results[0];
    for (int i = 1; i < 4; ++i) {
        if (min_distances[i] < min_distance) {
            min_distance = min_distances[i];
            new_color = results[i];
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
        case Palette::Catpuccin:
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