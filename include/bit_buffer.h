#ifndef CPR_BIT_BUFFER
#define CPR_BIT_BUFFER

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

namespace CPR {

class BitBuffer {
    public:
    BitBuffer() : _buffer(), _position(0) {}

    /// @brief Conversion factory to convert from a raw bit stream
    static BitBuffer from_bits(const std::vector<bool> &bits);

    /// @brief Writes a single bit to the buffer
    /// @param bit The bit to write
    void write_bit(bool bit);
    /// @brief Writes multiple bits to the buffer
    /// @param bits Vector of bits
    void write_bits(const std::vector<bool> &bits);

    /// @brief Read a bit from the buffer
    /// @return The bit
    bool read_bit(size_t index) const;
    /// @brief Makes a copy of all the bits of the buffer
    /// @return The bits
    std::vector<bool> read_bits() const;

    /// @brief Get number of bits in buffer
    size_t size() const;
    /// @brief Get number of bytes in buffer
    size_t byte_size() const;
    /// @brie Get the underlying contiguous memory of the bits
    std::vector<uint8_t> data() const;

    friend std::ostream &operator<<(std::ostream &os, const BitBuffer &other);

    protected:
    std::vector<uint8_t> _buffer;
    size_t _position;
};

}; // namespace CPR

#endif
