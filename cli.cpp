#include <CLI/CLI.hpp>
#include "palette.hpp"
#include "cli.hpp"

Args get_args(int argc, char *argv[]) {
    CLI::App app{"cpp_project"};

    Args a;

    std::vector<std::string> available_palettes = {"TokyoNight", "Dracula", "Solarized", "Nord", "Gruvbox", "RosePine", "MaterialSakura", "Catpuccin"};

    std::string palette_desc = "Palette to use.\nThis can be either a name of a palette or a path to a JSON file with a palette.\nAvailable palettes:";
    for (const auto& palette : available_palettes) {
        palette_desc += "\n\t - " + palette;
    }

    app.add_option("palette", a.palette, palette_desc)->required();

    app.add_option("image", a.image, "Image to process")->required();

    app.add_option("-o,--output", a.output, "Output file path");

    app.add_option("-f, --factors", a.factors, "Factors: Lightness, Chroma, Hue");

    app.set_version_flag("-V,--version", "cpp_project v0.1");

    app.set_help_flag("-h,--help", "Print this message and exit");

    app.set_help_all_flag("--help-all", "Print all help messages and exit");

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        app.exit(e);
        exit(1);
    }

    return a;
}