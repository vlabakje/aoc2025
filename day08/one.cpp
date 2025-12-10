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

using Point = std::tuple<int, int, int>;
using Points = std::map<int, Point>;
using Distances = std::vector<std::pair<std::pair<int, int>, float>>;
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
            float distance = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2) + std::pow(z2 - z1, 2));
            distances.push_back(std::make_pair(std::make_pair(it1->first, it2->first), distance));
            //distances[{it2->first, it1->first}] = distance;
        }
    }
    std::sort(distances.begin(), distances.end(), 
              [](const auto& a, const auto& b) { return a.second < b.second; });
}

std::uint64_t three_largest_circuits(const std::string& filename, int connections) {
    Points points;
    read_file(filename, points);
    std::println("points: {}", points);
    Distances distances;
    generate_distances(points, distances);
    PointToCircuit point_circuit;
    CircuitToPoints circuit_points;
    // connect nearest points until n connections have been made
    int n = 0;
    for (const auto& [po, distance] : distances) {
        std::println("circuit_points: {}", circuit_points);
        std::println("p1: {} p2: {}", points[po.first], points[po.second]);
        if (n > (connections-1)) break;
        if (point_circuit.count(po.first) && point_circuit.count(po.second)){
            // if points both points are already connected to the same circuit, skip
            if (point_circuit[po.first] == point_circuit[po.second]) {
                n++;
                continue;
            }
            std::println("merge");
            // if points connect to different circuits, merge
            int orig = point_circuit[po.second];
            int dest = point_circuit[po.first];
            for (auto& [p, c] : point_circuit) if (c == orig) point_circuit[p] = dest;
            circuit_points[dest].insert(circuit_points[orig].begin(), circuit_points[orig].end());
            circuit_points[orig] = {};
        } else if (point_circuit.count(po.first)) {
            // if only one is connected to a circuit, connect the other as well
            point_circuit[po.second] = point_circuit[po.first];
            circuit_points[point_circuit[po.first]].insert(po.second);
        } else if (point_circuit.count(po.second)) {
            point_circuit[po.first] = point_circuit[po.second];
            circuit_points[point_circuit[po.second]].insert(po.first);
        } else {
            // else connect the two points to form a new circuit
            point_circuit[po.first] = n;
            point_circuit[po.second] = n;
            circuit_points[n].insert(po.first);
            circuit_points[n].insert(po.second);
        }
        n++;
    }
    // now get the size of the three largest circuit_point values and multiply them together
    std::vector<int> sizes;
    for (const auto& [c, points] : circuit_points) sizes.push_back(points.size());
    std::sort(sizes.begin(), sizes.end(), std::greater<int>());
    std::println("sizes: {}", sizes);
    if (sizes.size() < 3) return 0;
    return static_cast<std::uint64_t>(sizes[0]) * static_cast<std::uint64_t>(sizes[1]) * static_cast<std::uint64_t>(sizes[2]);
}

int main() {
    std::println("{}", three_largest_circuits("example", 10));
    std::println("{}", three_largest_circuits("input", 1000));
	return 0;
}
