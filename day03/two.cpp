#include <algorithm>
#include <iostream>
#include <fstream>
#include <numeric>
#include <vector>
#include <sstream>
#include <string>
#include <ranges>
#include <print>
#include <set>

using Joltage = std::int64_t;
using Bank = std::vector<Joltage>;
using Banks = std::vector<Bank>;
using IndicesX = std::vector<std::int32_t>;
using Indices = std::set<std::int32_t>;

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

Joltage joltage(const Bank& bank, const Indices& idxs){
	std::string output;
    output.reserve(idxs.size());
    for (int idx : idxs) output += char('0' + bank[idx]);
    return std::stoll(output);
}

Joltage joltage_swapped(const Bank& bank, Indices& idxs, int out, int in){
	idxs.erase(out);
	idxs.insert(in);
	Joltage output = joltage(bank, idxs);
	idxs.erase(in);
	idxs.insert(out);
	return output;
}

Joltage max_joltage(const Bank& bank, int n) {
	// fill a set with 0-n
	Indices idxs, all_idxs;
	for (int i = 0; i < n; i++) idxs.insert(i);
	for (int i = 0; i < bank.size(); i++) all_idxs.insert(i);
	Joltage top = joltage(bank, idxs);
	bool replaced = true;
	while (replaced) {
		replaced = false;
        Indices diff;
        std::ranges::set_difference(all_idxs, idxs, std::inserter(diff, diff.begin()));
        // for every index in the set
		for (auto idx : idxs) {
			// check if swapping it with a number not in the set results in a bigger joltage
			for (auto option : diff){
				Joltage next = joltage_swapped(bank, idxs, idx, option);
				if (next > top) {
					idxs.erase(idx);
					idxs.insert(option);
					top = next;
					replaced = true;
					break;
				}
			}
			if (replaced) break;
		}
	}
	// if it doens't we've reached the max
	return top;
}

std::int64_t total_joltage(const std::string& filename) {
	std::int64_t total = 0;
	for (const auto& bank: load_banks(filename)) {
		auto m = max_joltage(bank, 12);
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
