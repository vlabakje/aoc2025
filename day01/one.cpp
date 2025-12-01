#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

struct Instruction {
    char dir;
    int value;
};

std::vector<Instruction> instructions(const std::string& filename) {
	std::ifstream file(filename);
	if (!file) throw std::runtime_error("No such file: " + filename);

	std::vector<Instruction> output;
	for (std::string line; std::getline(file, line); ) {
		std::istringstream ss(line);
		char lr;
		int c;
        if (!(ss >> lr >> c) || (lr != 'L' && lr != 'R'))
            throw std::runtime_error("Bad instruction: " + line);
        output.push_back({lr, c});
	}
	file.close();
	return output;
}

int num_zeros(const std::string& filename){
	int zeros = 0; // heh
	int dial = 50;
	for (const auto& [lr, c] : instructions(filename)) {
		dial += (lr == 'L' ? -c : c);
		dial = (dial + 100) % 100;
		// std::cout << lr << c << " dial: " << dial << std::endl;
		if (dial == 0) zeros++;
	}
	return zeros;
}

int main() {
	std::cout << num_zeros("example") << std::endl;
	std::cout << num_zeros("input") << std::endl;
	return 0;
}
