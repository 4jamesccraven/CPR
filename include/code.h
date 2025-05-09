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

    //! @brief Make a copy of the code with a bit added to it
    //! @param next_bit The bit to oadd
    //! @return The new code
    Code with(bool next_bit) const;
    //! @brief Represent the code as a string
    //! @return String representation of the code
    std::string repr() const;
    //! @brief Get the underlying data
    //! @return A vector of booleans that represents the code
    std::vector<bool> data() const;

    friend std::ostream &operator<<(std::ostream &os, const Code &cd);

    protected:
    std::vector<bool> _code;
};

}; // namespace CPR

#endif
