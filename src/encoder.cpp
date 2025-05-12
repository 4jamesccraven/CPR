#include "encoder.h"
#include "bit_buffer.h"
#include "cpr_core.h"
#include "tree.h"

#include <algorithm>
#include <string>
#include <vector>

namespace CPR {

Encoder::Encoder(const std::vector<std::string> &files) {
    this->_files = files;

    FreqTable table;
    for (auto file : files) {
        auto freq = freq_from_file(file);
        merge_freq(table, freq);
    }

    Tree tree = Tree::new_tree(table);

    this->_fq = table;
    this->_lb = tree.get_code_lengths();
    this->_cb = cb_from_lengths(this->_lb);
};

BitBuffer Encoder::encode(const std::string &text) {
    // Encode the data into a temporary buffer
    BitBuffer temp_buf{};
    BitBuffer buf{};

    for (auto c : text) {
        temp_buf.write_bits(this->_cb[c].data());
    }

    Code table_size = Code(this->_cb.size());
    Code data_length = Code(temp_buf.size());

    buf.write_bits(table_size.data());

    const auto lb = this->_lb;
    std::for_each(lb.begin(), lb.end(), [&](const auto &pair) {
        const auto k = pair.first;
        const auto v = pair.second;

        buf.write_bits(Code(k).data());
        buf.write_bits(Code(v).data());
    });

    buf.write_bits(data_length.data());

    buf.write_bits(temp_buf.read_bits());

    return buf;
}

}; // namespace CPR
