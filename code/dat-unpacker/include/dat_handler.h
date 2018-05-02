#pragma once
#ifndef _GNU_SOURCE
	#define _GNU_SOURCE
#endif
#include <adtf_streaming.h>
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>
#include <deque>
#include <cstring>
#include <boost/filesystem.hpp>
#include <fstream>


#define MATRIX_NAME "matrix"
#define CONV_SCANS_NAME "scan"
#define CHECK_STREAM_NAME "3dod"


namespace fs = boost::filesystem;
namespace adtf = adtfstreaming;

class dat_file {
	public:
		explicit dat_file(std::string &input, bool turn_off_stdout, std::string output_dir);
		~dat_file();
		virtual bool process_data(bool) = 0;
		void print_streams();

		tTimeStamp duration;
		bool is_valid;
		uint32_t stream_count;

	protected:
		void save_data(int id, const char *ext, adtf::cADTFDataBlock *block);
		void save_data(int id, const char *ext, const char *data, size_t data_size);
		int find_stream_by_name(std::string name);
		std::unique_ptr<adtf::IADTFFileReader> file_reader;
		bool stdout_off;
		fs::path output_dir;
};

class conv_datfile: public dat_file {
	public:
		using dat_file::dat_file;
		bool process_data(bool all);
	private:
		int32_t process_matrix(adtf::cADTFDataBlock *block);

		uint32_t read_samples;
};
