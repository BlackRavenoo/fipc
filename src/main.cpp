#include <thread>
#include <chrono>
#include "ciede2000.hpp"
#include "convert.hpp"
#include "palette.hpp"
#include "cli.hpp"
#include "image.hpp"

struct ArrayHash {
    template <typename T, std::size_t N>
    std::size_t operator()(const std::array<T, N>& array) const {
        std::size_t hash = 0;
        for (const auto& element : array) {
            hash ^= std::hash<T>{}(element) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};

int main(int argc, char *argv[]) {
    auto start_time1 = std::chrono::high_resolution_clock::now();
    Args args = get_args(argc, argv);

    auto palette = parse_palette(args.palette).flatten();

    Image image(args.image.c_str());

    std::vector<std::array<double, 3>> lab_colors;
    for (const auto& hex_color : palette) {
        lab_colors.push_back(hex2lab(hex_color));
    }

    int num_threads = std::thread::hardware_concurrency();
    if (num_threads <= 0) {
        num_threads = 1;
    }
    std::vector<std::thread> threads(num_threads);

    int num_pixels = image.width * image.height;
    int channels = image.channels;

    int base_pixels = num_pixels / num_threads;
    int remainder = num_pixels % num_threads;

    int pixel_offset = 0;
    for (int t = 0; t < num_threads; ++t) {
        int pixels_for_thread = base_pixels + (t < remainder ? 1 : 0);
        int start = pixel_offset * channels;
        int end = start + pixels_for_thread * channels;
        pixel_offset += pixels_for_thread;

        threads[t] = std::thread([&, start, end]() {
            std::unordered_map<std::array<uint8_t, 3>, std::array<double, 3>, ArrayHash> rgb2lab_cache;
            std::unordered_map<std::array<double, 3>, std::array<double, 3>, ArrayHash> nearest_palette_cache;

            for (int i = start; i < end; i += image.channels) {
                std::array<uint8_t, 3> rgb = {image.data[i], image.data[i + 1], image.data[i + 2]};
                std::array<double, 3> lab;

                if (rgb2lab_cache.find(rgb) != rgb2lab_cache.end()) {
                    lab = rgb2lab_cache[rgb];
                } else {
                    lab = rgb2lab(rgb[0], rgb[1], rgb[2]);
                    rgb2lab_cache[rgb] = lab;
                }

                std::array<double, 3> nearest;

                if (nearest_palette_cache.find(lab) != nearest_palette_cache.end()) {
                    nearest = nearest_palette_cache[lab];
                } else {
                    nearest = nearest_palette(lab, lab_colors, args.factors);
                    nearest_palette_cache[lab] = nearest;
                }

                auto res_rgb = lab2rgb(nearest[0], nearest[1], nearest[2]);
                image.data[i] = res_rgb[0];
                image.data[i + 1] = res_rgb[1];
                image.data[i + 2] = res_rgb[2];
            }
        });
    }

    auto start_time = std::chrono::high_resolution_clock::now();

    for (auto& thread : threads) {
        thread.join();
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();

    if (args.output.empty()) {
        args.output = "output.png";
    } else if (args.output.find('.') == std::string::npos) {
        args.output += ".png";
    } else if (args.output.substr(args.output.find_last_of('.')) != ".png" && image.get_format(args.output.c_str()) == ImageFormat::PNG) {
        args.output += ".png";
    }

    std::filesystem::path outputPath(args.output);
    if (!outputPath.parent_path().empty() && !std::filesystem::exists(outputPath.parent_path())) {
        std::filesystem::create_directories(outputPath.parent_path());
    }

    image.write(args.output.c_str());

    auto end_time1 = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    std::cout << "Execution time: " << duration << "ms" << std::endl;

    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end_time1 - start_time1).count();

    std::cout << "Total execution time: " << duration1 << "ms" << std::endl;

    return 0;
}