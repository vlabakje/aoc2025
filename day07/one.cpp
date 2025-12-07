#include <fstream>
#include <vector>
#include <print>
#include <algorithm>
#include <numeric>
#include <set>

using Row = std::string;
using Map = std::vector<Row>;
using Beams = std::set<int>;

void read_file(const std::string& filename, Map& map) {
    std::string line;

    std::ifstream file(filename);
	if (!file) throw std::runtime_error("No such file: " + filename);

    // Read all input lines
    while (std::getline(file, line)) {
        if (line != "") map.push_back(line);
    }
}

int split_count(const std::string& filename) {
    int splits = 0;
    Map map;
    read_file(filename, map);
    Beams beams;
    beams.insert(map[0].find('S'));
    for (int i=1; i<map.size(); i++) {
        // std::println("beeams: {}", beams);
        Beams next;
        for (auto beam : beams) {
            if (map[i][beam] == '^') {
                next.insert(beam-1);
                next.insert(beam+1);
                splits++;
            } else {
                next.insert(beam);
            }
        }
        beams = next;
    }
    return splits;
}

int main() {
    std::println("{}", split_count("example"));
    std::println("{}", split_count("input"));
	return 0;
}
