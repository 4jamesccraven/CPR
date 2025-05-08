#include "cpr_core.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ranges>
#include <type_traits>
#include <vector>

template <typename T>
int count_digits(T n) {
    static_assert(
        std::is_unsigned<T>::value,
        "count_digits only supports unsigned integers"
    );

    if (n == 0)
        return 1;

    int count{};

    while (n > 0) {
        n /= 10;
        count++;
    }

    return count;
}

namespace CPR {

FreqTable freq_from_file(std::string filename) {
    std::fstream file{};
    std::map<char, unsigned long long> map{};
    char ch;

    file.open(filename);

    while (file.get(ch)) {
        map[ch]++;
    }

    file.close();

    return map;
}

void print_freq(FreqTable table) {
    auto width_element = std::ranges::max_element(
        table | std::views::values, {},
        [](const auto count) {
            return count;
        }
    );

    auto width = std::max(count_digits(*width_element), 5);

    std::vector<Freq> pairs;

    for (auto pair : table) {
        pairs.push_back(pair);
    }

    std::sort(pairs.begin(), pairs.end(), [](auto a, auto b) {
        return a.second < b.second;
    });

    std::cout << "Char | " << std::setw(width) << "Count" << std::endl;

    std::for_each(pairs.crbegin(), pairs.crend(), [=](const auto pair) {
        std::string key;

        switch (pair.first) {
            case '\n':
                key = "'\\n'";
                break;
            case '\r':
                key = "'\\r'";
                break;
            case ' ':
                key = "' '";
                break;
            default:
                key = pair.first;
                break;
        }

        std::cout << std::left << std::setw(4) << key << " | " << std::right
                  << std::setw(width) << pair.second << std::endl;
    });
}

void merge_freq(FreqTable &table, const FreqTable &other) {
    for (auto pair : other) {
        table[pair.first] += pair.second;
    }
}

void print_codes(CodeBook cb) {
    std::cout << "Char | Code" << std::endl;

    for (auto pair : cb) {
        std::string key;

        switch (pair.first) {
            case '\n':
                key = "'\\n'";
                break;
            case '\r':
                key = "'\\r'";
                break;
            case ' ':
                key = "' '";
                break;
            default:
                key = pair.first;
                break;
        }
        std::cout << std::setw(4) << key << " | " << pair.second << std::endl;
    }
}

} // namespace CPR
