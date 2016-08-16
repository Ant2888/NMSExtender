#pragma once
#include <string>
#include <Windows.h>

/*
*	File was pretty much just taken from SE. It's just an iterator to search through all the DLL's inside the modDir
*/

class ModIterator
{
public:
	ModIterator(const char* path, const char* match = NULL);
	virtual ~ModIterator();

	WIN32_FIND_DATAA* Get(void){ return &m_result; }
	void GetFullPath(char* out, unsigned long outLen);
	std::string	GetFullPath(void);

	void Next(void);
	bool Done(void);

private:
	ModIterator();

	HANDLE m_searchHandle;
	WIN32_FIND_DATAA m_result;
	bool m_done;

	char m_path[MAX_PATH];
};