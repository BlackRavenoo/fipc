#include <vector>
#include <nlohmann/json.hpp>

enum class Palette {
    TokyoNight,
    Dracula,
    Solarized,
    Nord,
    Gruvbox,
    RosePine,
    MaterialSakura,
    Catppuccin,
    Custom,
};

std::array<double, 3> nearest_palette(
    const std::array<double, 3>& color, 
    const std::vector<std::array<double, 3>>& palette
);

nlohmann::json read_palette(const Palette palette);
nlohmann::json read_palette_from_file(const std::string& filename);