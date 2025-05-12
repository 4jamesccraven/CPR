#include "cpr_core.h"
#include "encoder.h"

#include <string>
#include <vector>

#include <CLI11.hpp>

int main(int argc, char *argv[]) {
    CLI::App app{"file compression app"};
    argv = app.ensure_utf8(argv);

    std::vector<std::string> files;
    app.add_option("files", files, "The file(s) to compress/decompress")
        ->option_text("FILES")
        ->required(true)
        ->check(CLI::ExistingFile);

    std::string out_file = "";
    app.add_option(
           "-o,--out", out_file, "Save the (de)compressed data to a file"
    )
        ->option_text("FILE");
    // ->check(CLI::NonexistentPath);

    bool encode{false};
    auto e = app.add_flag(
        "-c,--compress,-e,--encode", encode, "Compress the file(s)"
    );

    bool decode{false};
    auto d = app.add_flag(
        "-d,--decompress,--decode", decode, "Decompress the file(s)"
    );

    bool show_table{false};
    app.add_flag("-t,--print-table", show_table, "Print the frequency table");

    bool show_coding{false};
    app.add_flag(
        "-C,--print-codebook", show_coding, "Print the character encoding"
    );

    bool print{false};
    app.add_flag("-p,--print", print, "Print the output of (de)compression");

    e->excludes(d);
    d->excludes(e);

    CLI11_PARSE(app, argc, argv);

    if (encode) {

        CPR::Encoder encoder(files);

        bool action_to_do = print || show_table || show_table;

        if (!action_to_do) {
            std::cerr << "Nothing to do" << std::endl;
            return 0;
        }

        if (show_table) {
            CPR::print_freq(encoder.get_frequency());
        }

        if (show_coding) {
            CPR::print_codes(encoder.get_codes());
        }

        for (auto file : files) {
            if (print) {

            } else {
            }
        }

    } else if (decode) {

    } else {
        std::cerr << "No action specified (expected one of [-d/-e])."
                  << std::endl
                  << std::endl;
        std::cout << app.help() << std::flush;
    }
}
