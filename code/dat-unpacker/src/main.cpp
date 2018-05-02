#include "dat_handler.h"
#include "parse_input.h"

int main(int argc, char **argv) {
	param_parser pp;

	if (pp.parseInput(argc, argv) != 0) {
		return -1;
	}

	dat_file *dat;


	dat = new conv_datfile(pp.input, true, pp.output_dir);


	if (!dat->is_valid) {
		return -1;
	}

	dat->print_streams();

	if (!dat->process_data(pp.all)) {
		return -1;
	}

	return 0;
}
