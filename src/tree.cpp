#include "tree.h"
#include "cpr_core.h"

#include <algorithm>
#include <iterator>
#include <map>
#include <memory>
#include <optional>
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

uint32_t TreeNode::get_freq() {
    return this->_frequency;
}

void TreeNode::set_char(char c) {
    this->_character = std::make_optional(c);
}

char TreeNode::get_char() {
    if (!this->_character.has_value()) {
        throw std::runtime_error("Attempted to access non-existant character.");
    }

    return this->_character.value();
}

Tree Tree::new_tree(FreqTable freq_map) {
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

Tree Tree::from_codes(CodeBook codes) {
    auto root = std::make_shared<TreeNode>(TreeNode({}, 0));

    for (const auto &[ch, code] : codes) {
        TreeNode *current = root.get();
        for (bool bit : code.data()) {
            if (bit == 1) {
                if (!current->get_right())
                    current->set_right(
                        std::make_shared<TreeNode>(TreeNode({}, 0))
                    );
                current = current->get_right().get();
            } else {
                if (!current->get_left())
                    current->set_left(std::make_shared<TreeNode>(TreeNode({}, 0)
                    ));
                current = current->get_left().get();
            }
        }
        current->set_char(ch);
    }

    return Tree(root);
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

LengthBook Tree::get_code_lengths() {
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

    this->_lb = sizes;

    return sizes;
}

std::string Tree::decode(const std::vector<bool> &bits) {
    std::string out;
    TreeNode *current = this->p_head.get();

    for (bool bit : bits) {
        current = bit ? current->get_right().get() : current->get_left().get();
        if (!current)
            throw std::runtime_error("invalid encoding");

        if (!current->get_left() && !current->get_right()) {
            out += current->get_char();
            current = this->p_head.get();
        }
    }

    return out;
}

} // namespace CPR
