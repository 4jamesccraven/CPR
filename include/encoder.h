#ifndef CPR_ENCODER
#define CPR_ENCODER

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
    /// @brief Encode string data
    /// @param text The string to encode
    BitBuffer encode(const std::string &text);

    protected:
    FreqTable _fq;
    LengthBook _lb;
    CodeBook _cb;
    std::vector<std::string> _files;
};

}; // namespace CPR

#endif
