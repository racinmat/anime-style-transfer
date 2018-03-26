#include "dat_handler.h"

using namespace adtfstreaming;
DatHandler::~DatHandler() {
	deInitDatHandler();
}

tTimeStamp DatHandler::getTimeDuration() {
	return m_fileReader->GetTimeDuration();
}

bool DatHandler::initDatHandler(std::string &inputFileName) {
	// Create the referenced files extension object reading from file
	freopen("/dev/null", "w", stdout);

	m_fileReader = std::unique_ptr<IADTFFileReader>(IADTFFileReader::Create());
	tResult res = m_fileReader->Open(inputFileName.c_str());


	if (!IS_OK(res)) {
		std::cerr << "Error while opening file: " << inputFileName << std::endl;
	}

	m_fileReader->GetStreamCount(stream_count);

	for (tInt i = 1; i <= stream_count; i++) {
		descriptors.push_back(m_fileReader->GetStreamDescriptor(i));
	}

	return IS_OK(res);
}

tInt DatHandler::get_stream_count() {
	return stream_count;
}

void DatHandler::print_streams() {
	for (int i = 1; i <= stream_count; i++) {
		std::string sname = m_fileReader->GetStreamName(i);
		tInt64 dcount = m_fileReader->GetDataBlockCount(i);
		std::cerr << i << ":\t" << dcount << "\t" << sname << std::endl;
	}
}

const tADTFStreamDescriptor *DatHandler::get_descriptor(tInt n) {
	return descriptors[n - 1];
}

void DatHandler::get_stream_data(tUInt16 stream_id) {
	cADTFDataBlock *block = nullptr;
	tResult res = ERR_NOERROR;
	static int j = 0;
	const tVoid *data = nullptr;

	while (IS_OK(res)) {
		res = m_fileReader->Read(&block);
		tUInt16 sid = block->GetStreamId();

		if (sid == stream_id) {
			tInt64 dSize = block->GetData(&data);
			std::cerr << dSize << std::endl;
			// char * cdata = (char *)data;
			// std::ofstream outfile;
			// outfile.open(std::to_string(j) + ".bin");
			// outfile.write(cdata, dSize);
			// outfile.close();
			// j++;
			float *fdata = (float *)data;
			std::ofstream infile;
			infile.open(std::to_string(j) + ".txt");

			for (tInt64 i = 0; i < dSize / (4 * 4); i++) {
				infile << fdata[i * 4] << "\t" << fdata[i * 4 + 1] << "\t" << fdata[i * 4 + 2] << "\t" << fdata[i * 4 + 3] << std::endl;
			}

			infile.close();
			j++;
		}
	}
}

bool DatHandler::deInitDatHandler() {
	tResult res = ERR_NOERROR;

	//disable console output for adtf streaming lib
	freopen("/dev/null", "w", stdout);

	if (m_fileReader) {
		res = IADTFFileReader::Release(m_fileReader.release());
	}

	//re-enable output for the rest of the program

	return IS_OK(res);
}
