#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <print>
#include <cctype> // for std::isspace
#include <cstdlib> // for std::system (optional, for testing)

using Row = std::vector<int>;
using Operators = std::vector<char>;
using Table = std::vector<Row>;

void read_file(const std::string& filename, Table& table, Operators& operators) {
    std::vector<std::string> lines;
    std::string line;

    std::ifstream file(filename);
	if (!file) throw std::runtime_error("No such file: " + filename);

    // Read all input lines
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    if (lines.empty()) throw std::runtime_error("Error: No input provided.");

    // Parse the table of integers
    for (size_t i = 0; i < lines.size() - 1; ++i) {
        const std::string& current_line = lines[i];
        std::istringstream iss(current_line);
        Row row;
        int num;
        while (iss >> num) {
            row.push_back(num);
        }
        table.push_back(row);
    }

    // Parse the operator line
    const std::string& operator_line = lines.back();
    for (char c : operator_line) {
        if (!std::isspace(static_cast<unsigned char>(c))) {
            operators.push_back(c);
        }
    }
}

std::int64_t problem(const Table& table, const Operators& operators, int i){
    std::int64_t total = operators[i] == '*';
    for (auto row : table){
        if (operators[i] == '+') total += row[i];
        else total *= row[i];
    }
    return total;
}

std::int64_t total(const std::string& filename){
    std::int64_t total = 0;
    Table table;
    Operators operators;
    read_file(filename, table, operators);
    for (int i=0; i<table[0].size(); i++) {
        std::int64_t result = problem(table, operators, i);
        std::print("result: {}\n", result);
        total += result;
    }
    return total;
}

int main() {
	std::cout << total("example") << std::endl;
	std::cout << total("input") << std::endl;
	return 0;
}
