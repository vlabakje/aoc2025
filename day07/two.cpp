#include <fstream>
#include <vector>
#include <print>
#include <algorithm>
#include <numeric>
#include <set>
#include <map>
#include <iostream>

using Row = std::string;
using Map = std::vector<Row>;
using Beams = std::set<int>;
using PathMap = std::map<int, std::int64_t>;

void read_file(const std::string& filename, Map& map) {
    std::string line;

    std::ifstream file(filename);
	if (!file) throw std::runtime_error("No such file: " + filename);

    // Read all input lines
    while (std::getline(file, line)) {
        if (line != "") map.push_back(line);
    }
}

std::uint64_t path_count(const std::string& filename) {
    PathMap pathmap;
    Map map;
    read_file(filename, map);
    pathmap[map[0].find('S')] = 1;
    for (int i=1; i<map.size(); i++) {
        // std::println("paths: {}", pathmap);
        PathMap nextpathmap;
        for (auto beam : pathmap) {
            if (map[i][beam.first] == '^') {
                nextpathmap[beam.first-1] += beam.second;
                nextpathmap[beam.first+1] += beam.second;
            } else {
                nextpathmap[beam.first] += beam.second;
            }
        }
        pathmap = nextpathmap;
    }
    return std::ranges::fold_left(pathmap, 0, [](std::uint64_t acc, const auto& p){ return acc + p.second;});
}

int main() {
    std::println("{}", path_count("example"));
    std::println("{}", path_count("input"));
	return 0;
}
