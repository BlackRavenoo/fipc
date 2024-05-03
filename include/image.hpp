#include <cstdint>
#include <cstddef>

enum ImageFormat {
    PNG,
    JPG,
    BMP,
    TGA,
    HDR
};

struct Image {
    uint8_t *data = nullptr;
    size_t size = 0;
    int width;
    int height;
    int channels;

    Image(const char *filename);
    Image(int width, int height, int channels);
    Image(const Image &other);
    ~Image();

    Image& operator=(const Image &other);

    bool read(const char* filename);
    bool write(const char* filename);

    ImageFormat get_format(const char* filename);
};