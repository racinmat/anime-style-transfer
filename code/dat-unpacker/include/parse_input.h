#pragma once

#include <string>
#include <vector>

class param_parser {
	public:
		std::string input;
		std::string output_dir;
		std::vector<std::string> to_dump;
		bool raw;
		bool all;

		int parseInput(int argc, char *argv[]);
};
