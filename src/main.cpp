#include "cpr_core.h"
#include "tree.h"

#include <string>

#include <CLI11.hpp>

int main(int argc, char *argv[]) {
    CLI::App app{"file compression app"};
    argv = app.ensure_utf8(argv);

    std::vector<std::string> files;
    app.add_option("-f,--files", files, "The file(s) to compress/decompress")
        ->option_text("FILE")
        ->required(true)
        ->check(CLI::ExistingFile);

    bool encode{false};
    app.add_flag("-c,--compress,-e,--encode", encode, "Compress the file(s)");

    bool decode{false};
    app.add_flag("-d,--decompress,--decode", decode, "Decompress the file(s)");

    bool show_table{false};
    app.add_flag("-t,--print-table", show_table, "Print the frequency table");

    bool show_coding{false};
    app.add_flag(
        "-C,--print-codebook", show_coding, "Print the final encoding"
    );

    CLI11_PARSE(app, argc, argv);

    if (encode) {

        CPR::FreqTable freq_table;
        for (auto file : files) {
            CPR::FreqTable other = CPR::freq_from_file(file);
            CPR::merge_freq(freq_table, other);
        }

        if (show_table)
            CPR::print_freq(freq_table);

        CPR::Tree cpr_tree = CPR::Tree::new_tree(freq_table);

        if (show_coding) {
            CPR::CodeBook cb = cpr_tree.get_codes();
            CPR::print_codes(cb);
        }

    } else if (decode) {

    } else {
        std::cerr << "No action specified (expected one of [-d/-e])."
                  << std::endl
                  << std::endl;
        std::cout << app.help() << std::flush;
    }
}
