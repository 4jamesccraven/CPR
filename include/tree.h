#ifndef CPR_TREE
#define CPR_TREE

#include "cpr_core.h"

#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

namespace CPR {

class TreeNode {
    public:
    /// @brief Constructor for existing char
    /// @param char_val The character to store
    /// @param freq Its frequency
    TreeNode(char char_val, uint32_t freq)
        : _character(std::optional<char>{char_val}), _frequency(freq),
          p_left(nullptr), p_right(nullptr) {}

    /// @brief Constructor for Optional char
    /// @param val The optional char value
    /// @param freq Its frequency
    /* clang-format off */
    TreeNode(std::optional<char> val, uint32_t freq)
        : _character(val), _frequency(freq),
          p_left(nullptr), p_right(nullptr) {}
    /* clang-format on */

    /// @brief Setter for the left child
    void set_left(std::shared_ptr<TreeNode> l);
    /// @brief Setter for the right child
    void set_right(std::shared_ptr<TreeNode> r);

    /// @brief Getter for the left child
    std::shared_ptr<TreeNode> get_left();
    /// @brief Getter for the right child
    std::shared_ptr<TreeNode> get_right();

    /// @brief Setter for the char
    void set_char(char c);

    /// @brief Getter for the frequency
    uint32_t get_freq();
    /// @brief Getter for the character
    /// \warning Throws an error if char is None
    char get_char();

    protected:
    // data
    std::optional<char> _character;
    uint32_t _frequency;

    // node structure
    std::shared_ptr<TreeNode> p_left;
    std::shared_ptr<TreeNode> p_right;
};

class Tree {
    public:
    /// @brief Factory constructor
    /// @param freq_map A map that models a given char's probability
    static Tree new_tree(FreqTable freq_map);
    /// @brief Factory constructor for pre-existing codes
    /// @param codes Map of <char, Code>
    static Tree from_codes(CodeBook codes);
    /// @brief Generate the serialisable lengths
    LengthBook get_code_lengths();

    /// @brief Decode bits into a character
    /// @param bits The bits to decode
    std::string decode(const std::vector<bool> &bits);

    protected:
    std::shared_ptr<TreeNode> p_head;
    LengthBook _lb;

    Tree(std::shared_ptr<TreeNode> head) : p_head(head), _lb({}) {}

    /* clang-format off */
    void _gen_code_no_cannon(std::shared_ptr<TreeNode> root, Code code, CodeBook &cb);
    /* clang-format on */
};

} // namespace CPR

#endif
