#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <print>

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

bool overlap(const Range& a, const Range& b){
    if (a.start <= b.start){
        return a.end >= (b.start-1);
    }
    return b.end >= (a.start-1);
}

Ranges merge_ranges(Ranges sources) {
    Ranges target;
    bool merged = true;
    while (merged) {
        std::print("sources ({})\n", sources.size());
        for (auto s: sources) std::print("  source: {}-{}\n", s.start, s.end);
        merged = false;
        // find two ranges that can be merged and merge them
        for (int i=0; i< sources.size()-1; i++){
            for (int j=i+1; j<sources.size(); j++){
                if (overlap(sources[i], sources[j])){
                    target.push_back({
                        std::min(sources[i].start, sources[j].start),
                        std::max(sources[i].end, sources[j].end)});
                    for (int k=0; k<sources.size(); k++){
                        if (k != i && k != j){
                            target.push_back(sources[k]);
                        }
                    }
                    sources = target;
                    target = {};
                    merged = true;
                    break;
                }
            }
            if (merged) break;
        }
    }
    return sources;
}

std::uint64_t fresh_ingredients(const std::string& filename) {
    std::uint64_t count = 0;
    Ranges ranges;
    Ingredients ingredients;
    read_ranges(filename, ranges, ingredients);
    ranges = merge_ranges(ranges);
    for (auto range: ranges) {
        std::print("range: {}-{}\n", range.start, range.end);
        count += (range.end + 1)-range.start;
    }
    return count;
}

int main() {
    std::cout << fresh_ingredients("example") << std::endl;
    std::cout << fresh_ingredients("input") << std::endl;
    return 0;
}
