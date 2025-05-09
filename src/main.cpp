#include "bit_buffer.h"
#include "cpr_core.h"
#include "tree.h"

#include <fstream>
#include <sstream>
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

    std::string out_file = "";
    app.add_option(
           "-o,--out", out_file, "Save the (de)compressed data to a file"
    )
        ->option_text("FILE")
        ->check(CLI::NonexistentPath);

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

        for (auto file : files) {
            if (print)
                std::cout << file << ':' << std::endl;

            std::ifstream file_handle(file);

            if (!file_handle)
                throw std::runtime_error("No such file");

            std::stringstream buf;

            buf << file_handle.rdbuf();

            CPR::BitBuffer bits = cpr_tree.encode(buf.str());

            if (print)
                std::cout << bits << std::endl;

            if (!print || !out_file.empty()) {
                if (!out_file.empty()) {
                    cpr_tree.write_encode(out_file, buf.str());
                } else {
                    std::string filename;
                    auto dot = file.find_last_of('.');

                    if (dot != std::string::npos) {
                        filename = file.substr(0, dot) + ".cprx";
                    } else {
                        filename = file + ".cprx";
                    }

                    cpr_tree.write_encode(filename, buf.str());
                }
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
