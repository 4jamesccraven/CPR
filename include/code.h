#ifndef CPR_CODE
#define CPR_CODE

#include <iostream>
#include <ostream>
#include <string>
#include <vector>

namespace CPR {

class Code {
    public:
    Code() : _code(std::vector<bool>()) {};
    Code(std::vector<bool> code) : _code(code) {};
    template <typename T>
    Code(T value);

    /// @brief Make a copy of the code with a bit added to it
    /// @param next_bit The bit to oadd
    /// @return The new code
    Code with(bool next_bit) const;
    /// @brief Represent the code as a string
    /// @return String representation of the code
    std::string repr() const;
    /// @brief Get the underlying data
    /// @return A vector of booleans that represents the code
    std::vector<bool> data() const;
    /// @brief Get number of bits in the code
    size_t size() const;

    friend std::ostream &operator<<(std::ostream &os, const Code &cd);

    protected:
    std::vector<bool> _code;
};

template <typename T>
Code::Code(T value) {
    static_assert(
        std::is_integral<T>::value,
        "Codes can only be constructed from std::vector<bool> or integral types"
    );

    // The byte size should be one for a character, 4 for an integer
    /* clang-format off */
    constexpr size_t num_bits =
        std::is_same<T, char>::value
        || std::is_same<T, signed char>::value
        || std::is_same<T, unsigned char>::value
        ? 8 : 32;
    /* clang-format on */

    using unsigned_t = typename std::make_unsigned<T>::type;
    unsigned_t uvalue = static_cast<unsigned_t>(value);

    std::vector<bool> bits(num_bits);
    for (size_t i = 0; i < num_bits; i++) {
        bits[i] = (uvalue >> (num_bits - 1 - i)) & 1;
    }

    this->_code = bits;
}

}; // namespace CPR

#endif
