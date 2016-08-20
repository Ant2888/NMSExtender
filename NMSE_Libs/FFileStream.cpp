#include "FFileStream.h"

FFileStream::FFileStream(const char* path, bool append){
	m_file.open(path, std::ios::out | std::ios::app);
	if (!append && m_file.is_open()){
		m_file.clear();
	}
}

FFileStream::~FFileStream(){
	if (m_file.is_open())
		m_file.close();
}

bool FFileStream::WriteToFile(std::string toWrite){
	if (m_file.is_open()){
		m_file << toWrite << std::endl;
		return true;
	}
	return false;
}

void FFileStream::ClearFile(){
	if (m_file.is_open()){
		m_file.clear();
	}
}

static bool WriteToFile(std::ofstream& stream, std::string toWrite){
	if (stream.is_open()){
		stream << toWrite << std::endl;
		return true;
	}
	return false;
}

