#include "parse_input.h"
#include <iostream>
#include <boost/program_options.hpp>

int param_parser::parseInput(int argc, char *argv[]) {
	int status = 0;

	namespace po = boost::program_options;
	po::options_description desc("Allowed options");

	desc.add_options()
	("help,h", "Print help message")
	("input-file-name,i", po::value<std::string>(&input)->required(), "Input DAT file")
	("output-dir,o", po::value<std::string>(&output_dir)->required(), "Output direcotry")
	("all,a", "Process full data?")
	;

	po::positional_options_description p;
	p.add("input-file-name", 1);
	p.add("output-dir", 2);

	po::variables_map vm;

	try {
		po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl << std::endl;
		std::cout << desc;
		return -1;
	}

	if (vm.count("help")) {
		std::cout << desc;
		return -1;
	}

	if (vm.count("all")) {
		all = true;
	}
	else {
		all = false;
	}

	try {
		po::notify(vm);
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl << std::endl;
		std::cout << desc;
		return -1;
	}

	return status;
}
