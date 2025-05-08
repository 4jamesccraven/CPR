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

    Code with(bool next_bit);
    std::string repr();

    friend std::ostream &operator<<(std::ostream &os, const Code &cd);

    protected:
    std::vector<bool> _code;
};

}; // namespace CPR

#endif
