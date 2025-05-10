#include "tree.h"
#include "cpr_core.h"

#include <algorithm>
#include <iterator>
#include <map>
#include <memory>
#include <queue>
#include <stdexcept>
#include <sys/types.h>
#include <utility>

namespace CPR {

void TreeNode::set_left(std::shared_ptr<TreeNode> t) {
    this->p_left = t;
}

void TreeNode::set_right(std::shared_ptr<TreeNode> t) {
    this->p_right = t;
}

std::shared_ptr<TreeNode> TreeNode::get_left() {
    return this->p_left;
}

std::shared_ptr<TreeNode> TreeNode::get_right() {
    return this->p_right;
}

unsigned long long TreeNode::get_freq() {
    return this->_frequency;
}

char TreeNode::get_char() {
    if (!this->_character.has_value()) {
        throw std::runtime_error("Attempted to access non-existant character.");
    }

    return this->_character.value();
}

Tree Tree::new_tree(std::map<char, unsigned long long> freq_map) {
    typedef std::shared_ptr<TreeNode> NodePtr;

    // See below.
    auto pair_cmp = [](NodePtr a, NodePtr b) {
        return a->get_freq() > b->get_freq();
    };

    // This takes all characters and makes it so that the least probable
    // characters have the highest priority
    std::priority_queue<NodePtr, std::vector<NodePtr>, decltype(pair_cmp)>
        queue{pair_cmp};

    for (auto pair : freq_map) {
        queue.push(std::make_shared<TreeNode>(TreeNode(pair.first, pair.second))
        );
    }

    // Until we have one node (i.e., the complete tree)
    while (queue.size() != 1) {
        // Get the bottom two by frequency
        NodePtr left = queue.top();
        queue.pop();
        NodePtr right = queue.top();
        queue.pop();

        // Make a new node with their sum and no character
        auto sum = left->get_freq() + right->get_freq();
        auto new_node = std::make_shared<TreeNode>(TreeNode({}, sum));

        // Set them as children of the node
        new_node->set_left(left);
        new_node->set_right(right);

        // Add it back to the queue
        queue.push(new_node);
    }

    // Return the one head node as a Tree object
    return Tree(queue.top());
}

void Tree::_gen_code_no_cannon(
    std::shared_ptr<TreeNode> root, Code code, CodeBook &cb
) {
    // If this is null we've gone too far
    if (!root) {
        return;
    }

    // If there's no children this is a leaf node, and
    // thus our code.
    if (!root->get_left() && !root->get_right()) {
        cb[root->get_char()] = code;
    }

    // Recurse to the left...
    this->_gen_code_no_cannon(root->get_left(), code.with(0), cb);
    // ...then the right
    this->_gen_code_no_cannon(root->get_right(), code.with(1), cb);
}

LengthBook Tree::get_codes() {
    // If we've calculated this before, return the result
    if (this->_lb.has_value())
        return this->_lb.value();

    // Generate non-cannonical code lengths
    CodeBook cb;
    std::vector<bool> empty;
    this->_gen_code_no_cannon(this->p_head, empty, cb);

    std::map<char, size_t> sizes;
    std::transform(
        cb.cbegin(), cb.cend(), std::inserter(sizes, sizes.end()),
        [](auto &p) {
            return std::make_pair(p.first, p.second.size());
        }
    );

    this->_lb = std::optional<LengthBook>(sizes);

    return sizes;
}

// BitBuffer Tree::encode(std::string text) {
//     BitBuffer temp_buf{};
//
//     // Get encoding data
//     auto cb = this->get_codes();
//
//     // Encode the input text in a temporary buffer
//     for (auto c : text) {
//         temp_buf.write_bits(cb[c].data());
//     }
//
//     BitBuffer buf{};
//
//     auto size = temp_buf.size();
//
//     // Encode the byte size into the final buffer
//     for (int i = 31; i >= 0; i--) {
//         buf.write_bit((size >> i) & 1);
//     }
//
//     // Add in the encoded data
//     buf.write_bits(temp_buf.read_bits());
//
//     return buf;
// }
//
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

} // namespace CPR
