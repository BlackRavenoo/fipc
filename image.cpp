#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stdexcept>
#include "stb_image.h"
#include "stb_image_write.h"
#include "image.hpp"

Image::Image(const char *filename) {
    if(!read(filename)) {
        throw std::runtime_error("Failed to read image");
    }
    size = width * height * channels;
}

Image::Image(int width, int height, int channels) : width(width), height(height), channels(channels) {
    size = width * height * channels;
    data = new uint8_t[size];
}

Image::Image(const Image &other) : width(other.width), height(other.height), channels(other.channels), size(other.size){
    data = new uint8_t[size];
    std::copy(other.data, other.data + size, data);
}

Image& Image::operator=(const Image &other) {
    if(this != &other) {
        delete[] data;

        width = other.width;
        height = other.height;
        channels = other.channels;
        size = other.size;

        data = new uint8_t[size];
        std::copy(other.data, other.data + size, data);
    }
    return *this;
}

Image::~Image() {
    stbi_image_free(data);
}

bool Image::read(const char* filename) {
    data = stbi_load(filename, &width, &height, &channels, 0);
    return data != nullptr;
}

bool Image::write(const char* filename) {
    ImageFormat format = get_format(filename);
    int status = 0;
    switch(format) {
        case ImageFormat::PNG:
            status = stbi_write_png(filename, width, height, channels, data, width * channels);
            break;
        case ImageFormat::JPG:
            status = stbi_write_jpg(filename, width, height, channels, data, 100);
            break;
        case ImageFormat::BMP:
            status = stbi_write_bmp(filename, width, height, channels, data);
            break;
        case ImageFormat::TGA:
            status = stbi_write_tga(filename, width, height, channels, data);
            break;
    }

    return status != 0;
}

ImageFormat Image::get_format(const char* filename) {
    const char *ext = strrchr(filename, '.');
    if(ext) {
        if(strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) {
            return ImageFormat::JPG;
        } else if(strcmp(ext, ".bmp") == 0) {
            return ImageFormat::BMP;
        } else if(strcmp(ext, ".tga") == 0) {
            return ImageFormat::TGA;
        }
    }

    return ImageFormat::PNG;
}
