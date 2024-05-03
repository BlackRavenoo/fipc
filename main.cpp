#include <thread>
#include "ciede2000.hpp"
#include "convert.hpp"
#include "palette.hpp"
#include "cli.hpp"
#include "image.hpp"

int main(int argc, char *argv[]) {
    Args args = get_args(argc, argv);

    auto palette = parse_palette(args.palette).flatten();

    Image image(args.image.c_str());

    std::vector<std::array<double, 3>> lab_colors;
    for (const auto& hex_color : palette) {
        lab_colors.push_back(hex2lab(hex_color));
    }

    int num_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads(num_threads);

    int chunk_size = image.size / num_threads;

    for (int t = 0; t < num_threads; ++t) {
        threads[t] = std::thread([&, t]() {
            int start = t * chunk_size;
            int end = (t == num_threads - 1) ? image.size : start + chunk_size;

            for (int i = start; i < end; i += image.channels) {
                std::array<double, 3> lab = rgb2lab(image.data[i], image.data[i + 1], image.data[i + 2]);
                auto res = nearest_palette(lab, lab_colors);
                auto res_rgb = lab2rgb(res[0], res[1], res[2]);
                image.data[i] = res_rgb[0];
                image.data[i + 1] = res_rgb[1];
                image.data[i + 2] = res_rgb[2];
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    if (args.output.empty()) {
        args.output = "output.png";
    }

    std::filesystem::path outputPath(args.output);
    if (!outputPath.parent_path().empty() && !std::filesystem::exists(outputPath.parent_path())) {
        std::filesystem::create_directories(outputPath.parent_path());
    }

    image.write(args.output.c_str());

    return 0;
}