#include "CLI.h"

#include <CLI/CLI.hpp>

CLI_t parse_args(CLI::App &app, int argc, char *argv[]) {
    argv = app.ensure_utf8(argv);

    // Initialise booleans to false, allow classes to default initialise
    CLI_t args;
    args.encode = false;
    args.decode = false;
    args.show_frequency = false;
    args.show_encoding = false;
    args.no_output = false;
    args.print = false;

    /* clang-format off */
    app.add_option(
        "files",
        args.files,
        "The file(s) to compress/decompress"
    )
    ->option_text("FILES")
    ->required(true)
    ->check(CLI::ExistingFile);

    app.add_option(
        "-o,--out",
        args.out_file,
        "Save the (de)compressed data to a file"
    )
    ->option_text("FILE");

    auto e = app.add_flag(
        "-c,--compress,-e,--encode",
        args.encode,
        "Compress the file(s)"
    );


    auto d = app.add_flag(
        "-d,--decompress,--decode",
        args.decode,
        "Decompress the file(s)"
    );

    auto f = app.add_flag(
        "-t,--print-frequency",
        args.show_frequency,
        "Print the frequency table"
    );

    app.add_flag(
        "-C,--print-codebook",
        args.show_encoding,
        "Print the character encoding"
    );

    app.add_flag(
        "-p,--print",
        args.print,
        "Print the output of (de)compression"
    );

    auto O = app.add_flag(
        "-O,--no-extract",
        args.no_output,
        "Do not reconstruct files, even when not using -p"
    );
    /* clang-format on */

    e->excludes(d);
    d->excludes(e);
    d->excludes(f); // It is not possible to determine this from an encoded file

    app.parse(argc, argv);

    return args;
}
