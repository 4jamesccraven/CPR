#ifndef CPR_TREE
#define CPR_TREE

#include "bit_buffer.h"
#include "cpr_core.h"

#include <memory>
#include <optional>
#include <string>

namespace CPR {

class TreeNode {
    public:
    /// @brief Constructor for existing char
    /// @param char_val The character to store
    /// @param freq Its frequency
    TreeNode(char char_val, unsigned long long freq)
        : _character(std::optional<char>{char_val}), _frequency(freq),
          p_left(nullptr), p_right(nullptr) {
    }

    /// @brief Constructor for Optional char
    /// @param val The optional char value
    /// @param freq Its frequency
    TreeNode(std::optional<char> val, unsigned long long freq)
        : _character(val), _frequency(freq), p_left(nullptr), p_right(nullptr) {
    }

    /// @brief Setter for the left child
    void set_left(std::shared_ptr<TreeNode> l);
    /// @brief Setter for the right child
    void set_right(std::shared_ptr<TreeNode> r);

    /// @brief Getter for the left child
    std::shared_ptr<TreeNode> get_left();
    /// @brief Getter for the right child
    std::shared_ptr<TreeNode> get_right();

    /// @brief Getter for the frequency
    unsigned long long get_freq();
    /// @brief Getter for the character
    /// \warning Throws an error if char is None
    char get_char();

    protected:
    // data
    std::optional<char> _character;
    unsigned long long _frequency;

    // node structure
    std::shared_ptr<TreeNode> p_left;
    std::shared_ptr<TreeNode> p_right;
};

class Tree {
    public:
    //! @brief Factory Constructor
    //! @param freq_map A map that models a given char's probability
    static Tree new_tree(FreqTable freq_map);
    //! @brief Generate the final map of character encodings
    CodeBook get_codes();
    //! @brief Encode the string to a buffer
    //! @param text The text to encode
    BitBuffer encode(std::string text);
    //! @brief Encode the string to a file
    //! @param text The text to encode
    void write_encode(std::string filename, std::string text);

    protected:
    std::shared_ptr<TreeNode> p_head;

    Tree(std::shared_ptr<TreeNode> head) : p_head(head) {
    }

    void _gen_code(std::shared_ptr<TreeNode> root, Code code, CodeBook &cb);
};

} // namespace CPR

#endif
