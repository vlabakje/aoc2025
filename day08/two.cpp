#include <fstream>
#include <vector>
#include <print>
#include <algorithm>
#include <numeric>
#include <set>
#include <map>
#include <iostream>
#include <sstream>
#include <cmath>

using Point = std::tuple<std::int64_t, std::int64_t, std::int64_t>;
using Points = std::map<int, Point>;
using Distances = std::vector<std::pair<std::pair<int, int>, std::int64_t>>;
using PointToCircuit = std::map<int, int>;
using CircuitToPoints = std::map<int, std::set<int>>;

void read_file(const std::string& filename, Points& points) {
    std::string line;

    std::ifstream file(filename);
	if (!file) throw std::runtime_error("No such file: " + filename);
    int index = 0;
    // Read all input lines
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string coord_x, coord_y, coord_z;
        if (std::getline(iss, coord_x, ',') && std::getline(iss, coord_y, ',') && std::getline(iss, coord_z)) {
            points[index++] = {std::stoi(coord_x), std::stoi(coord_y), std::stoi(coord_z)};
        }
    }
}

void generate_distances(const Points& points, Distances& distances){
    // take the product of points's keys
    // generate the distance between these points and store in distances
    for (auto it1 = points.begin(); it1 != points.end(); ++it1) {
        for (auto it2 = std::next(it1); it2 != points.end(); ++it2) {
            // Calculate Euclidean distance between two points
            auto [x1, y1, z1] = it1->second;
            auto [x2, y2, z2] = it2->second;

            std::int64_t distance = std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2) + std::pow(z2 - z1, 2);
            distances.push_back(std::make_pair(std::make_pair(it1->first, it2->first), distance));
        }
    }
    std::sort(distances.begin(), distances.end(), 
              [](const auto& a, const auto& b) { return a.second < b.second; });
}

std::int64_t one_circuit(const std::string& filename) {
    Points points;
    read_file(filename, points);
    Distances distances;
    generate_distances(points, distances);
    PointToCircuit point_circuit;
    CircuitToPoints circuit_points;
    int cid = 0;
    for (auto& [idx, pt] : points) {
        point_circuit[idx] = cid;
        circuit_points[cid++].insert(idx);
    }
    // connect nearest points until n connections have been made
    for (const auto& [po, distance] : distances) {
        auto [p1, p2] = po;
        // if connected to different circuits we merge
        if (point_circuit[p1] != point_circuit[p2]){
            if (circuit_points.size() == 2){
                auto [x1, y1, z1] = points[p1];
                auto [x2, y2, z2] = points[p2];
                return x1 * x2;
            }

            int orig = point_circuit[p1];
            int dest = point_circuit[p2];

            // Merge: move all points from orig → dest
            for (auto& [p, c] : point_circuit)
                if (c == orig)
                    c = dest;

            // Rebuild set for dest
            circuit_points[dest].clear();
            for (auto& [p, c] : point_circuit)
                if (c == dest)
                    circuit_points[dest].insert(p);

            circuit_points.erase(orig);
        }
    }
    return -1;
}

int main() {
    std::println("{}", one_circuit("example"));
    std::println("{}", one_circuit("input"));
	return 0;
}


