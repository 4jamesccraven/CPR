#include "encoder.h"
#include "bit_buffer.h"
#include "cpr_core.h"
#include "tree.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

uint32_t read_u32(std::ifstream &f) {
    uint8_t buf[4];

    f.read(reinterpret_cast<char *>(buf), 4);

    // Shift each byte to occupy its significance place, and combine
    // using bitwise or
    return (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
}

char read_char(std::ifstream &f) {
    char byte;
    f.read(&byte, 1);

    return byte;
}

namespace CPR {

Encoder::Encoder(const std::vector<std::string> &files) {
    FreqTable table;
    for (auto file : files) {
        auto freq = freq_from_file(file);
        merge_freq(table, freq);
    }

    Tree tree = Tree::new_tree(table);

    this->_files = files;
    this->_fq = table;
    this->_lb = tree.get_code_lengths();
    this->_cb = cb_from_lengths(this->_lb);
};

BitBuffer Encoder::encode_header() const {
    BitBuffer buf{};

    // Encode number of k,v pairs in the LengthBook
    Code table_size = Code(this->_cb.size());

    buf.write_bytes(table_size.data());

    // For each value encode one byte for the character, four for the frequency
    const auto lb = this->_lb;
    std::for_each(lb.begin(), lb.end(), [&](const auto &pair) {
        const auto k = pair.first;
        const auto v = pair.second;

        buf.write_bytes(Code(k).data());
        buf.write_bytes(Code(v).data());
    });

    return buf;
}

BitBuffer Encoder::encode_body(const std::string &text) {
    // Encode the data into a temporary buffer
    BitBuffer temp_buf{};
    BitBuffer buf{};

    for (char c : text) {
        temp_buf.write_bits(this->_cb[c].data());
    }

    Code data_length = Code(temp_buf.size());

    // Dump actual encoding to final buffer
    buf.write_bits(data_length.data());
    buf.write_bits(temp_buf.read_bits());

    return buf;
}

BitBuffer Encoder::_make_encode() {
    BitBuffer buf{};

    // Encode information for decoding
    buf.write_bytes(this->encode_header().read_bits());

    for (auto file : this->_files) {
        // Encode the file's name
        BitBuffer filename{};
        for (char c : file) {
            filename.write_bytes(Code(c).data());
        }
        filename.write_bytes(Code('\0').data());

        // Read the file and encode its body
        std::ifstream file_handle(file);
        if (!file_handle)
            throw std::runtime_error("Unable to open file");

        std::stringstream body;
        body << file_handle.rdbuf();

        BitBuffer body_encoded = this->encode_body(body.str());

        buf.write_bytes(filename.read_bits());
        buf.write_bytes(body_encoded.read_bits());
    }

    return buf;
}

BitBuffer Encoder::encode_files() {
    return this->_make_encode();
}

void Encoder::encode_files(std::string out_file) {
    std::ofstream out(out_file, std::ios::binary);

    if (!out)
        throw std::runtime_error("unable to open archive destination");

    auto bits = this->_make_encode();

    std::vector<uint8_t> bytes = bits.data();

    // Reinterpret the u8s as chars, and write them to the file
    out.write(reinterpret_cast<const char *>(bytes.data()), bytes.size());

    out.close();
}

void Encoder::decode_archive(CLI_t args) {
    if (args.files.size() > 1)
        throw std::runtime_error("too many files");

    auto file = args.files[0];

    std::ifstream file_handle(file, std::ios::binary);

    if (!file_handle)
        throw std::runtime_error("unable to open archive");

    // Reconstruct the lengths
    LengthBook lb;
    uint32_t encoding_count = read_u32(file_handle);

    for (auto i = 0; i < encoding_count; i++) {
        char c = read_char(file_handle);
        uint32_t length = read_u32(file_handle);

        lb[c] = length;
    }

    // Recreate the true encoding
    auto cb = cb_from_lengths(lb);
    auto tree = Tree::from_codes(cb);

    if (args.show_encoding)
        print_codes(cb);

    // While there are characters to read
    char c;
    while (file_handle.get(c)) {
        file_handle.seekg(-1, std::ios_base::cur);

        // Get the null-terminated file name
        std::string filename;
        while (file_handle.get(c) && c != '\0') {
            filename += c;
        }

        if (args.print)
            std::cout << filename << ':' << std::endl;

        // Retrieve the raw encoded data for the file
        uint32_t bit_count = read_u32(file_handle);
        std::vector<bool> bits;
        bits.reserve(bit_count);

        // While there are bits left
        while (bits.size() < bit_count) {
            // Read a byte
            char byte;
            if (!file_handle.read(&byte, 1))
                throw std::runtime_error("unexpected EOF");

            // Push bits to our buffer until we use the byte or hit our limit
            uint8_t b = static_cast<uint8_t>(byte);
            for (int i = 7; i >= 0 && bits.size() < bit_count; i--) {
                bits.push_back((b >> i) & 1);
            }
        }

        // Decode the bits
        auto text = tree.decode(bits);

        if (args.print) {
            std::cout << text;
        }

        if (!args.print)
            std::cerr << "TODO" << std::endl;
    }
}

}; // namespace CPR
