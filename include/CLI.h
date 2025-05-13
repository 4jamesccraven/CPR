#ifndef CPR_CLI
#define CPR_CLI

#include <string>
#include <vector>

#include <CLI11.hpp>

struct CLI_t {
    // Positional
    std::vector<std::string> files;
    // Pseudo positional
    std::string out_file;
    // Subcommand
    bool encode;
    bool decode;
    // Flags
    bool show_frequency;
    bool show_encoding;
    bool print;
};

CLI_t parse_args(CLI::App &app, int argc, char *argv[]);

#endif
