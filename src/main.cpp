#include "CLI.h"
#include "cpr_core.h"
#include "encoder.h"

#include <CLI/CLI.hpp>
#include <string>

void run(CLI_t args, std::string help);

int main(int argc, char *argv[]) {
    CLI::App app{"A compression format based on huffman coding", "cpr"};
    CLI_t args;

    try {
        args = parse_args(app, argc, argv);
        run(args, app.help());
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    } catch (const std::runtime_error &e) {
        std::cerr << "CPR error: " << e.what() << std::endl;
        return 1;
    }
}

void run(CLI_t args, std::string help) {
    if (args.encode) {

        CPR::encode(args);

    } else if (args.decode) {

        CPR::Encoder::decode_archive(args);

    } else {
        std::cerr << "No action specified (expected one of [-d/-e])."
                  << std::endl
                  << std::endl;
        std::cout << help << std::flush;
    }
}
