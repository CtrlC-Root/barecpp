// C++ Standard Library
#include <iostream>

// BARE
#include <bare/bare.hpp>

// cxxopts
#include <cxxopts.hpp>

/**
 * Application entry point.
 */
int main(int argc, char* argv[]) {
    // parse command line arguments
    cxxopts::Options options("bq", "BARE Query");
    options.add_options()
        ("h,help", "Display usage help", cxxopts::value<bool>()->default_value("false"));

    auto args = options.parse(argc, argv);
    if (args.count("help")) {
        std::cout << options.help() << std::endl;
        return EXIT_SUCCESS;
    }

    // TODO

    // quit
    return EXIT_SUCCESS;
}
