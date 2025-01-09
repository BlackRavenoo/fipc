#include <map>
#include <string>

#include "catppuccin.h"
#include "dracula.h"
#include "edge.h"
#include "gruvbox.h"
#include "gruvbox_material.h"
#include "nord.h"
#include "rose_pine.h"
#include "solarized.h"
#include "sonokai.h"
#include "tokyo_night.h"

std::map<std::string, std::pair<const unsigned char*, size_t>> palettes = {
    {"catppuccin", {palettes_catppuccin_json, palettes_catppuccin_json_len}},
    {"dracula", {palettes_dracula_json, palettes_dracula_json_len}},
    {"edge", {palettes_edge_json, palettes_edge_json_len}},
    {"gruvbox", {palettes_gruvbox_json, palettes_gruvbox_json_len}},
    {"gruvbox_material", {palettes_gruvbox_material_json, palettes_gruvbox_material_json_len}},
    {"nord", {palettes_nord_json, palettes_nord_json_len}},
    {"rose_pine", {palettes_rose_pine_json, palettes_rose_pine_json_len}},
    {"solarized", {palettes_solarized_json, palettes_solarized_json_len}},
    {"sonokai", {palettes_sonokai_json, palettes_sonokai_json_len}},
    {"tokyo_night", {palettes_tokyo_night_json, palettes_tokyo_night_json_len}},
};
