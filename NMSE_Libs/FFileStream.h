#pragma once
#include <stdio.h>
#include <string>
#include <fstream>

class FFileStream{

	public:
		FFileStream(const char* path, bool append);
		~FFileStream();
		bool WriteToFile(std::string);
		void ClearFile();
	private:
		std::ofstream m_file;
};

static bool WriteToFile(std::ofstream&, std::string);