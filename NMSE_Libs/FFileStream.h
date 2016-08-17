#pragma once
#include <stdio.h>

class FFileStream{

	public:
		FFileStream();
		~FFileStream();
		// For the file just pass the file name. It will all go into NMSE\Dump\<file>
		FILE* Open(const char* file);

	private:

};