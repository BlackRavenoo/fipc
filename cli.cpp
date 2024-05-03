#include <CLI/CLI.hpp>
#include "palette.hpp"
#include "cli.hpp"

Args get_args(int argc, char *argv[]) {
    CLI::App app{"cpp_project"};

    Args a;

    app.add_option("image", a.image, "Image to process")->required();

    app.add_option("-p", a.palette, "Palette to use")->required();

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        app.exit(e);
        exit(1);
    }

    return a;
}