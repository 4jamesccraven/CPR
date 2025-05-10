#include "code.h"

#include <iostream>
#include <ostream>
#include <string>
#include <vector>

namespace CPR {

Code Code::with(bool next_bit) const {
    std::vector<bool> copy(this->_code);

    copy.push_back(next_bit);

    return CPR::Code(copy);
}

std::string Code::repr() const {
    std::string s;

    for (auto bit : this->_code) {
        s += (bit ? '1' : '0');
    }

    return s;
}

std::vector<bool> Code::data() const {
    return this->_code;
}

size_t Code::size() const {
    return this->_code.size();
}

std::ostream &operator<<(std::ostream &os, const Code &cd) {
    for (auto bit : cd._code) {
        os << (bit ? '1' : '0');
    }

    return os;
}

}; // namespace CPR
