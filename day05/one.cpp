#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>

struct Range {
    std::int64_t start;
    std::int64_t end;
};

using Ranges = std::vector<Range>;
using Ingredients = std::vector<std::int64_t>;

void read_ranges(const std::string& filename, Ranges& ranges, Ingredients& ingredients) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("no such file: " + filename);
    }

    std::string line;
    bool in_ranges = true;
    while (std::getline(file, line)) {
        std::istringstream stream(line);
        if (line == "") {
            in_ranges = false;
        } else if (in_ranges) {
            std::string start, end;
            if (std::getline(stream, start, '-') && std::getline(stream, end, '-')) {
                ranges.push_back({std::stoll(start), std::stoll(end)});
            } else {
                std::cerr << "malformed line " << line << std::endl;
            }
        } else {
            ingredients.push_back(std::stoll(line));
        }
    }
}

bool matches_any_range(const Ranges& ranges, std::int64_t ingredient) {
    for (const auto& range: ranges) {
        if (ingredient >= range.start && ingredient <= range.end) return true;
    }
    return false;
}

int fresh_ingredients(const std::string& filename) {
    int count = 0;
    Ranges ranges;
    Ingredients ingredients;
    read_ranges(filename, ranges, ingredients);
    for (const auto& ingredient: ingredients) {
        if (matches_any_range(ranges, ingredient)) count++;
    }
    return count;
}

int main() {
    std::cout << fresh_ingredients("example") << std::endl;
    std::cout << fresh_ingredients("input") << std::endl;
    return 0;
}
