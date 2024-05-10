#include <array>
#include <fstream>
#include "ciede2000.hpp"
#include "palette.hpp"

std::array<double, 3> nearest_palette(
    const std::array<double, 3>& color, 
    const std::vector<std::array<double, 3>>& palette,
    const std::array<double, 3>& factors
) {
    double min_distance = std::numeric_limits<double>::max();
    std::array<double, 3> nearest_color;

    for (const auto& palette_color : palette) {
        double delta = ciede2000(color, palette_color, factors);

        if (delta < min_distance) {
            min_distance = delta;
            nearest_color = palette_color;
        }
    }

    return nearest_color;
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

nlohmann::json parse_palette(const std::string& palette) {
    std::map<std::string, std::string> palette_files = {
        {"TokyoNight", "./palettes/tokyo_night.json"},
        {"Dracula", "./palettes/dracula.json"},
        {"Solarized", "./palettes/solarized.json"},
        {"Nord", "./palettes/nord.json"},
        {"Gruvbox", "./palettes/gruvbox.json"},
        {"RosePine", "./palettes/rose_pine.json"},
        {"Catppuccin", "./palettes/catppuccin.json"},
        {"Edge", "./palettes/edge.json"},
        {"Sonokai", "./palettes/sonokai.json"}
    };

    if (palette_files.count(palette)) {
        return read_palette_from_file(palette_files[palette]);
    } else if (palette.ends_with(".json") && std::filesystem::exists(palette)) {
        return read_palette_from_file(palette);
    } else {
        try {
            return nlohmann::json::parse(palette);
        } catch (nlohmann::json::parse_error&) {
            throw std::runtime_error("Invalid palette");
        }
    }
}