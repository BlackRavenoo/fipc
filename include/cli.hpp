#include <iostream>

struct Args {
    std::string image;
    std::string palette;

    Args() : image(""), palette("") {}
};

Args get_args(int argc, char *argv[]);