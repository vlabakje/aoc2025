#include <algorithm>
#include <iostream>
#include <fstream>
#include <numeric>
#include <vector>
#include <sstream>
#include <string>
#include <ranges>
#include <print>

struct Range {
    long long start;
    long long end;
};

std::vector<Range> load_ranges(const std::string& filename) {
	std::ifstream file(filename);
	if (!file) throw std::runtime_error("No such file: " + filename);
	std::string line;
	if (!std::getline(file, line)) throw std::runtime_error("empty file " + filename);

	std::vector<Range> output;
	std::istringstream ss(line);
	for (std::string part; std::getline(ss, part, ',');) {
		auto dash = part.find('-');
		if (dash == std::string::npos) throw std::runtime_error("Invalid range: " + part);

		output.push_back({std::stoll(part.substr(0, dash)), std::stoll(part.substr(dash+1))});
	}
	file.close();
	return output;
}

std::vector<int> int_digits(long long n) {
    return std::to_string(n)
         | std::views::transform([](char c) { return c - '0'; })
         | std::ranges::to<std::vector>();
}

bool invalid_id(long long n){
	// should only return true if the number contains a repeating pattern of digits
	if (n < 10) return false; // weird corner case I guess
	auto digits = int_digits(n);
	auto slice = [&](size_t start, size_t count) {
  		return digits | std::views::drop(start) | std::views::take(count);
	};
	for (long long i=1; i<=uint(digits.size()/2); i++){
		if (digits.size()%i!=0) continue;
		bool same = true;
		for (long long j=1; j<uint(digits.size()/i); j++){
			if (!std::ranges::equal(slice(0, i), slice(j*i, i))) same = false;
		}
		if (same) return true;
	}
	return false;
}

std::vector<long long> invalid_ids(Range range) {
	// std::cout << "range: " << range.start << " " << range.end << std::endl;
	std::vector<long long> output;
	for (long long i=range.start; i <= range.end; i++) {
		if (invalid_id(i)) {
			output.push_back(i);
			// std::cout << "found: " << i << std::endl;
			// std::print("{}\n", int_digits(i));
		}
	}
	return output;
}

long long sum_invalid_ids(const std::string& filename){
	long long sum_invalid = 0;
	for (const auto& range : load_ranges(filename)) {
		auto invalid = invalid_ids(range);
		sum_invalid = std::accumulate(invalid.begin(), invalid.end(), sum_invalid);
	}
	return sum_invalid;
}

int main() {
	// std::cout << invalid_ids({5859767462, 5859911897}).size() << std::endl;
	std::cout << sum_invalid_ids("example") << std::endl;
	std::cout << sum_invalid_ids("input") << std::endl;
	return 0;
}
