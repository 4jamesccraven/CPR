#ifndef CPR_ENCODER
#define CPR_ENCODER

#include "CLI.h"
#include "bit_buffer.h"
#include "cpr_core.h"

#include <string>
#include <vector>

namespace CPR {

class Encoder {
    public:
    Encoder(const std::vector<std::string> &files);

    /// @brief Get the encoding associated with the input(s)
    CodeBook get_codes() const { return this->_cb; };
    /// @brief Get the character frequencies associated with the input(s)
    FreqTable get_frequency() const { return this->_fq; };
    /// @brief Encode the header for the frequency table
    BitBuffer encode_header() const;
    /// @brief Encode string data
    /// @param text The string to encode
    BitBuffer encode_body(const std::string &text);
    /// @brief Encode files to bits
    /// @param out_file Optional file to output to
    BitBuffer encode_files();
    void encode_files(std::string out_file);
    /// @brief Decode an archive
    /// @param file Path to the archive
    /// @param extract Whether or not the contents should be fully extracted to
    /// the current working directory
    static void decode_archive(CLI_t args);

    protected:
    FreqTable _fq;
    LengthBook _lb;
    CodeBook _cb;
    std::vector<std::string> _files;

    BitBuffer _make_encode();
};

}; // namespace CPR

#endif
