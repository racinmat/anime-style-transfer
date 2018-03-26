#include "dat_handler.h"
#include "parse_input.h"

int main(int argc, char **argv) {
	param_parser pp;

	if (pp.parseInput(argc, argv) != 0) {
		return -1;
	}

	DatHandler *dat = new DatHandler();

	if (!dat->initDatHandler(pp.input)) {
		return -1;
	}

	dat->print_streams();
	dat->get_stream_data(pp.stream);

	return 0;
}
