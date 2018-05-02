#include "dat_handler.h"

namespace adtf = adtfstreaming;

dat_file::dat_file(std::string &input, bool turn_off_stdout, std::string out_dir) {
	if (turn_off_stdout) {
		freopen("/dev/null", "w", stdout);
		stdout_off = true;
	}
	else {
		stdout_off = false;
	}

	file_reader = std::unique_ptr<adtf::IADTFFileReader>(adtf::IADTFFileReader::Create());
	tResult res = file_reader->Open(input.c_str());

	if (!IS_OK(res)) {
		std::cerr << "Error while opening file: " << input << std::endl;
		freopen("/dev/tty", "a", stdout);
		is_valid = false;
		return;
	}

	is_valid = true;
	tInt scount;
	file_reader->GetStreamCount(scount);
	stream_count = scount;
	duration = file_reader->GetTimeDuration();
	output_dir = fs::path(out_dir);
	fs::create_directories(output_dir);

}

dat_file::~dat_file() {
	if (is_valid) {

		if (stdout_off) {
			freopen("/dev/tty", "a", stdout);
		}

		adtf::IADTFFileReader::Release(file_reader.release());
	}
}

int dat_file::find_stream_by_name(std::string name) {
	for (uint32_t i = 1; i <= stream_count; i++) {
		std::string sname = file_reader->GetStreamName(i);

		if (strcasestr(sname.c_str(), name.c_str()) != NULL) {
			return i;
		}
	}

	return -1;
}

void dat_file::save_data(int id, const char *ext, adtf::cADTFDataBlock *block) {
	const tVoid *data = nullptr;
	int size = block->GetData(&data);
	save_data(id, ext, (const char *)data, size);
}

void dat_file::save_data(int id, const char *ext, const char *data, size_t size) {
	char buf[5];
	std::snprintf(buf, 5, "%04d", id);
	fs::path npath = output_dir / (std::string(buf) + ext);
	std::ofstream ofs(npath.native(), std::ofstream::out | std::ofstream::binary);
	ofs.write(data, size);
	ofs.close();
}

void dat_file::print_streams() {
	for (uint32_t i = 1; i <= stream_count; i++) {
		std::string sname = file_reader->GetStreamName(i);
		tInt scount = file_reader->GetDataBlockCount(i);
		std::cerr << i << ":\t" << sname << "\tBlock count:\t" << scount << std::endl;
	}
}

bool conv_datfile::process_data(bool all) {
	int sid_pts = find_stream_by_name(CONV_SCANS_NAME);
	int sid_matrix = find_stream_by_name(MATRIX_NAME);
	int sid_3dod = find_stream_by_name(CHECK_STREAM_NAME);

	if (sid_pts == -1) {
		std::cerr << "No stream of name \"" << CONV_SCANS_NAME << "\"" << std::endl;
		return false;
	}

	if (sid_matrix == -1) {
		std::cerr << "No stream of name \"" << MATRIX_NAME << "\"" << std::endl;
		return false;
	}

	int32_t stream_size_pts = file_reader->GetDataBlockCount(sid_pts);
	int32_t stream_size_matrix = file_reader->GetDataBlockCount(sid_matrix);
	int32_t stream_size_dod = file_reader->GetDataBlockCount(sid_3dod);

	int32_t read_pts = 0, read_matrix, saved_dod = 0;

	adtf::cADTFDataBlock *block = nullptr;
	tInt sid;
	tResult result = ERR_NOERROR;

	while (IS_OK(result)) {
		result = file_reader->Read(&block);

		if (!IS_OK(result)) {
			break;
		}

		sid = block->GetStreamId();

		if (all && sid == sid_3dod) {
			save_data(saved_dod++, ".dod", block);
		}

		if (sid == sid_pts) {
			save_data(read_pts++, ".pts", block);
		}

		if (sid == sid_matrix) {
			read_matrix = process_matrix(block);

			if (read_matrix == -1) {
				break;
			}
		}
	}

	return (read_matrix == stream_size_matrix && read_pts == stream_size_pts && ((!all) ^ (saved_dod == stream_size_dod)));
}

int32_t conv_datfile::process_matrix(adtf::cADTFDataBlock *block) {
	static int32_t read = 0;
	const tVoid *data = nullptr;
	block->GetData(&data);
	tTimeStamp ts = block->GetTime();
	double matrix[16];

	int bytes_done;
	int bytes_now = 0;
	uint8_t *view = (uint8_t *)data;

	if (read > 790) {
		read = read;
	}

	sscanf((char *)data, "<matrix>[%n", &bytes_done);
	bytes_now += bytes_done;
	int ret = 0;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			ret += sscanf(((char *) data) + bytes_now, " %lg%n", &matrix[i * 4 + j], &bytes_done);
			bytes_now += bytes_done;
		}
	}

	if (ret == 16) {
		save_data(read, ".matrix", (const char *)matrix, sizeof(double) * 16);
		save_data(read, ".ts", (const char *) &ts, sizeof(ts));
		read++;
	}
	else {
		read = -1;
	}

	return read;
}
