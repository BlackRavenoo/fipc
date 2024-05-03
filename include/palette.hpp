#include <vector>
#include <nlohmann/json.hpp>

std::array<double, 3> nearest_palette(
    const std::array<double, 3>& color, 
    const std::vector<std::array<double, 3>>& palette
);

nlohmann::json read_palette_from_file(const std::string& filename);
nlohmann::json parse_palette(const std::string& palette);