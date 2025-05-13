#include "CLI.h"
#include "cpr_core.h"
#include "encoder.h"

#include <CLI11.hpp>

int main(int argc, char *argv[]) {
    CLI::App app{"file compression app", "cpr"};
    CLI_t args;

    try {
        args = parse_args(app, argc, argv);
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    if (args.encode) {

        bool action_to_do = !args.out_file.empty() || args.print ||
                            args.show_frequency || args.show_encoding;

        CPR::Encoder encoder(args.files);

        if (!action_to_do) {
            std::cerr << "Nothing to do" << std::endl;
            return 0;
        }

        if (args.show_frequency) {
            CPR::print_freq(encoder.get_frequency());
        }

        if (args.show_encoding) {
            CPR::print_codes(encoder.get_codes());
        }

        if (args.print) {
            std::cout << encoder.encode_files() << std::endl;
        } else {
            /* clang-format off */
            encoder.encode_files(!args.out_file.empty() ? args.out_file : "out.cprx");
            /* clang-format on */
        }

    } else if (args.decode) {

        CPR::Encoder::decode_archive(args);

    } else {
        std::cerr << "No action specified (expected one of [-d/-e])."
                  << std::endl
                  << std::endl;
        std::cout << app.help() << std::flush;
    }
}
