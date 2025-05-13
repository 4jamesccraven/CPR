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
        temp_buf.write_bytes(this->_cb[c].data());
    }

    Code data_length = Code(temp_buf.size());

    // Dump actual encoding to final buffer
    buf.write_bytes(data_length.data());
    buf.write_bytes(temp_buf.read_bits());

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
        throw std::runtime_error("Unable to open archive destination");

    auto bits = this->_make_encode();

    std::vector<uint8_t> bytes = bits.data();

    // Reinterpret the u8s as chars, and write them to the file
    out.write(reinterpret_cast<const char *>(bytes.data()), bytes.size());

    out.close();
}

// HOW: STORE
// void Tree::write_encode(std::string filename, std::string text) {
//     std::ofstream out(filename, std::ios::binary);
//
//     if (!out)
//         throw std::runtime_error("Unable to open file for writing");
//
//     BitBuffer buf = this->encode(text);
//
//     std::vector<uint8_t> data = buf.data();
//
//     out.write(reinterpret_cast<const char *>(data.data()), data.size());
//
//     out.close();
// }

}; // namespace CPR
