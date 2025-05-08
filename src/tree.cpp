#include "tree.h"
#include "cpr_core.h"

#include <map>
#include <memory>
#include <queue>
#include <stdexcept>

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

void Tree::_gen_code(std::shared_ptr<TreeNode> root, Code code, CodeBook &cb) {
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
    this->_gen_code(root->get_left(), code.with(0), cb);
    // ...then the right
    this->_gen_code(root->get_right(), code.with(1), cb);
}

CodeBook Tree::get_codes() {
    CodeBook cb;
    std::vector<bool> empty;

    this->_gen_code(this->p_head, empty, cb);

    return cb;
}

} // namespace CPR
