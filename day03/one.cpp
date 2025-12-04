#include <algorithm>
#include <iostream>
#include <fstream>
#include <numeric>
#include <vector>
#include <sstream>
#include <string>
#include <ranges>
#include <print>

using Bank = std::vector<std::int64_t>;
using Banks = std::vector<Bank>;

Banks load_banks(const std::string& filename) {
	std::ifstream file(filename);
	if (!file) throw std::runtime_error("No such file: " + filename);
	Banks banks;
    std::string line;
    while (std::getline(file, line)) {
        Bank bank;
        for (char c : line) if (std::isdigit(c)) bank.push_back(c - '0');
        if (!bank.empty()) banks.push_back(bank);
    }
	return banks;
}

std::int64_t max_joltage(const Bank bank) {
	// loop 9-1 and find the first instance in bank:-1 =x
	auto it_x = std::ranges::find(bank, *std::max_element(bank.begin(), bank.end()-1));
	// find the largest number in the remainder of the bank =y
	auto y = *std::max_element(it_x+1, bank.end());
	// std::print("{}: {}*{}\n", bank, *it_x, y);
	return (*it_x) * 10 + y;
}

std::int64_t total_joltage(const std::string& filename) {
	std::int64_t total = 0;
	for (const auto& bank: load_banks(filename)) {
		auto m = max_joltage(bank);
		// std::print("max_joltage: {}\n", m);
		total += m;
	}
	return total;
}

int main() {
	std::cout << total_joltage("example") << std::endl;
	std::cout << total_joltage("input") << std::endl;
	return 0;
}
