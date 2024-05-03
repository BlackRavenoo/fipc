#include <iostream>

struct Args {
    std::string image;
    std::string palette;
    std::string output;

    Args() : image(""), palette(""), output("") {}
};

Args get_args(int argc, char *argv[]);