#include <fstream>
#include <vector>
#include <print>
#include <algorithm>
#include <numeric>

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
    if (op == '+') return std::accumulate(values.begin(), values.end(), 0);
    if (op == '*') return std::ranges::fold_left(values, 1, std::multiplies<>());
    throw std::runtime_error("unsupported operator " + op);
}

std::int64_t problems(const std::string& filename){
    Table table;
    read_file(filename, table);
    std::int64_t total = 0;
    std::vector<std::int64_t> values;
    std::string column;
    for (int n=table[0].size()-1; n>=0; n--){
        for (int i=0; i<table.size()-1; i++) column += table[i][n];
        if (std::ranges::all_of(column, [](char c){ return c == ' '; })) continue;
        // std::print("converting: '{}'\n", column);
        values.push_back(std::stoll(column));
        column = "";
        if (table.back()[n] != ' '){
            total += problem(values, table.back()[n]);
            values = {};
        }
    }
    return total;
}

int main() {
    std::println("{}", problems("example"));
    std::println("{}", problems("input"));
	return 0;
}
