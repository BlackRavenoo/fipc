#include <iostream>

struct Args {
    std::string image;
    std::string palette;
    std::string output;
    std::array<double, 3> factors;

    Args() : image(""), palette(""), output(""), factors{1.0, 1.0, 1.0} {}
};

Args get_args(int argc, char *argv[]);