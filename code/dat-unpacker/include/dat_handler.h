#pragma once

#include <memory>
#include <iostream>
#include <string>
#include <map>
#include <cstdint>
#include <unistd.h>
#include <fstream>

#include <adtf_streaming.h>
#include <vector>


class DatHandler {
	public:
		DatHandler() {};

		~DatHandler();

		bool initDatHandler(std::string &inputFileName);
		void print_streams();
		tInt get_stream_count();
		void get_stream_data(tUInt16 stream_id);

		tTimeStamp getTimeDuration();
		const adtfstreaming::tADTFStreamDescriptor *get_descriptor(tInt n);

	private:
		bool deInitDatHandler();
		std::vector<const adtfstreaming::tADTFStreamDescriptor *> descriptors;
		std::unique_ptr<adtfstreaming::IADTFFileReader> m_fileReader;
		tInt stream_count;

};
