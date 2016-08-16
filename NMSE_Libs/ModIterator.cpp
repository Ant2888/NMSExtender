#pragma once
#include "ModIterator.h"
#include <string>

ModIterator::ModIterator(const char * path, const char * match)
	:m_searchHandle(INVALID_HANDLE_VALUE), m_done(false)
{
	if (!match) match = "*";

	strcpy_s(m_path, sizeof(m_path), path);

	char	wildcardPath[MAX_PATH];
	sprintf_s(wildcardPath, sizeof(wildcardPath), "%s\\%s", path, match);

	m_searchHandle = FindFirstFileA(wildcardPath, &m_result);
	if (m_searchHandle == INVALID_HANDLE_VALUE)
		m_done = true;
}

ModIterator::~ModIterator()
{
	if (m_searchHandle != INVALID_HANDLE_VALUE)
		FindClose(m_searchHandle);
}

void ModIterator::GetFullPath(char* out, unsigned long outLen)
{
	sprintf_s(out, outLen, "%s\\%s", m_path, m_result.cFileName);
}

std::string ModIterator::GetFullPath(void)
{
	return std::string(m_path) + "\\" + std::string(m_result.cFileName);
}

void ModIterator::Next(void)
{
	BOOL result = FindNextFileA(m_searchHandle, &m_result);
	if (!result)
		m_done = true;
}

bool ModIterator::Done(void)
{
	return m_done;
}
