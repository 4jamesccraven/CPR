#ifndef CPR_CORE
#define CPR_CORE

#include "code.h"

#include <cstdint>
#include <map>
#include <string>

namespace CPR {

typedef std::map<char, uint32_t> FreqTable;
typedef std::pair<char, uint32_t> Freq;
typedef std::map<char, Code> CodeBook;
typedef std::pair<char, Code> Encoding;
typedef std::map<char, size_t> LengthBook;

/**
 * @brief Gets a frequency map of all characters in a file
 *
 * @param filename The name of the file to open
 *
 * \warning Does not check for valid file as that should be handled at CLI
 *
 * @return The frequency map, as a std::map<char, uint32_t>
 */
FreqTable freq_from_file(std::string filename);

/**
 * @brief Prints the frequency table of the encoding, as in cpr -pf FILE
 *
 * @param The frequency table
 */
void print_freq(FreqTable table);

/**
 * @brief Updates a frequency table by adding summing the values of the second
 * into the first
 *
 * @param table The table to update
 *
 * @param other The other table
 */
void merge_freq(FreqTable &table, const FreqTable &other);

/**
 * @brief Prints the generated encoding
 *
 * @param cb The encoding map, or codebook
 */
void print_codes(CodeBook cb);

/**
 * @brief Generate a Codebook from byte, length encoding
 *
 * @param lengths Map of code lengths for each bit
 *
 * @return A CodeBook
 */
CodeBook cb_from_lengths(LengthBook lengths);

} // namespace CPR

#endif
