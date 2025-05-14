#include "bit_buffer.h"
#include <cstdint>
#include <stdexcept>

namespace CPR {

BitBuffer BitBuffer::from_bits(const std::vector<bool> &bits) {
    BitBuffer this_{};

    this_.write_bits(bits);

    return this_;
}

void BitBuffer::write_bit(bool bit) {
    if (this->_position == this->_buffer.size() * 8) {
        this->_buffer.push_back(0);
    }
    // Yes if (bit) works, but this is more explicit. These aren't booleans in
    // the typical sense.
    if (bit == 1) {
        const auto pos = this->_position;
        // The position in the vector is which byte it's in, so pos / 8
        // Then we set the bit in the (pos % 8)th place in the byte to one
        this->_buffer[pos / 8] |= (1 << (7 - (pos % 8)));
    }
    this->_position++;
}

void BitBuffer::write_bits(const std::vector<bool> &bits) {
    for (bool bit : bits) {
        this->write_bit(bit);
    }
}

void BitBuffer::write_bytes(const std::vector<bool> &bits) {
    for (bool bit : bits) {
        this->write_bit(bit);
    }

    while (this->_position % 8 != 0) {
        this->write_bit(0);
    }
}

bool BitBuffer::read_bit(size_t index) const {
    if (index >= this->_position) {
        throw std::out_of_range("Index out of range");
    }

    // At the index / 8 byte, we create a bit mask a the (index % 8)th
    // position. Cast it to a bit using the comparison operator
    return (this->_buffer[index / 8] & (1 << (7 - (index % 8)))) != 0;
}

std::vector<bool> BitBuffer::read_bits() const {
    std::vector<bool> bits(this->_position);

    for (auto i = 0; i < this->_position; i++) {
        bits[i] = this->read_bit(i);
    }

    return bits;
}

size_t BitBuffer::size() const {
    return this->_position;
}

size_t BitBuffer::byte_size() const {
    return (this->_position + 7) / 8;
}

std::vector<uint8_t> BitBuffer::data() const {
    return this->_buffer;
}

std::ostream &operator<<(std::ostream &os, const BitBuffer &other) {
    uint32_t i = 0;

    for (auto bit : other.read_bits()) {
        // Add spaces after bytes
        if (i != 0 && (i % 8 == 0))
            os << " ";

        if (i != 0 && (i % 48 == 0))
            os << std::endl;

        os << (bit ? '1' : '0');

        i++;
    }

    // Pad with 0s so that output is complete bytes
    while (i % 8 != 0) {
        os << '0';
        i++;
    }

    return os;
}

}; // namespace CPR
