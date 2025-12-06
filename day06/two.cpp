#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <print>
#include <cctype> // for std::isspace
#include <cstdlib> // for std::system (optional, for testing)

using Row = std::string;
using Table = std::vector<Row>;

void read_file(const std::string& filename, Table& table) {
    std::string line;

    std::ifstream file(filename);
	if (!file) throw std::runtime_error("No such file: " + filename);

    // Read all input lines
    while (std::getline(file, line)) {
        if (line != "") table.push_back(line);
    }
}

std::int64_t problem(const std::vector<std::int64_t>& values, char op){
    std::int64_t total = op == '*';
    for (auto v : values){
        if (op == '+') total += v;
        else total *= v;
    }
    return total;
}

std::int64_t problems(const std::string& filename){
    Table table;
    read_file(filename, table);
    std::int64_t total = 0;
    std::vector<std::int64_t> values;
    std::string column;
    bool skip = false;
    for (int n=table[0].size()-1; n>=0; n--){
        if (skip){
            skip = false;
            continue;
        }
        for (int i=0; i<table.size()-1; i++) column += table[i][n];
        // std::print("converting: '{}'\n", column);
        values.push_back(std::stoll(column));
        column = "";
        if (table.back()[n] != ' '){
            total += problem(values, table.back()[n]);
            values = {};
            skip = true; // ugly
        }        
    }
    return total;
}

int main() {
	std::cout << problems("example") << std::endl;
	std::cout << problems("input") << std::endl;
	return 0;
}
