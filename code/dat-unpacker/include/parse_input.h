#pragma once

#include <string>
#include <vector>

class param_parser {
	public:
		std::string input;
		std::vector<std::string> to_dump;
		bool raw;
		int stream;

		int parseInput(int argc, char *argv[]);
};
