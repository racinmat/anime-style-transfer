#include "parse_input.h"
#include <iostream>
#include <boost/program_options.hpp>


using namespace std;

int param_parser::parseInput(int argc, char *argv[]) {
	int status = 0;

	//parse command line inputs
	namespace po = boost::program_options;
	po::options_description desc("Allowed options");

	desc.add_options()
	("help,h", "Print help message")
	("input-file-name,i", po::value<string>(&input)->required(), "Input DAT file(s)")
	("output-dir,o", po::value<string>(&output_dir)->required(), "Output direcotry")
	("raw,r", "Raw dat file?")
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
		cerr << e.what() << endl << endl;
		cout << desc;
		return -1;
	}

	if (vm.count("help")) {
		cout << desc;
		return -1;
	}

	if (vm.count("raw")) {
		raw = true;
	}
	else {
		raw = false;
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
		cerr << e.what() << endl << endl;
		cout << desc;
		return -1;
	}

	return status;
}
