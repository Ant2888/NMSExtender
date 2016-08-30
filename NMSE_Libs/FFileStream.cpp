#include "FFileStream.h"

FFileStream global_Logger;

FFileStream::FFileStream()
	:m_File(nullptr){
	//
}

FFileStream::~FFileStream(){
	if (m_File)
		fclose(m_File);
}

bool FFileStream::OpenFile(const char* path, bool append){
	if (m_File){
		fclose(m_File);
	}
	if (append){
		m_File = fopen(path, "a+");
		return m_File != NULL;
	}
	m_File = fopen(path, "w+");
	return m_File != NULL;
}

bool FFileStream::WriteToFile(const char* buffer){
	if (m_File){
		return fputs(buffer, m_File) >= 0;
	}
	return false;
}

const char* modErr = "[ERROR] ";
bool FFileStream::WriteError(const char* errMsg, int errNum){
	if (m_File){
		if((fputs(modErr, m_File) < 0))
			return false;
		if((fputs(errMsg, m_File) < 0))
			return false;
		if (errNum){
			char* buff;
			_itoa(errNum, buff, sizeof(int)*8+1);
			if (fputs(buff, m_File) < 0)
				return false;
		}
		if (fputs("\n", m_File) < 0)
			return false;
		return true;
	}
	return false;
}

const char* modSuccess = "[SUCCESS] ";
bool FFileStream::WriteSuccess(const char* msg){
	if (m_File){
		if(fputs(modSuccess, m_File) < 0)
			return false;
		if(fputs(msg, m_File) < 0)
			return false;
		if (fputs("\n", m_File) < 0)
			return false; 
		return true;
	}
	return false;
}

