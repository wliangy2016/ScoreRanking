// stdafx.cpp : source file that includes just the standard includes
// TEST_15082016.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#include "Ranking.h"
#include <fstream>
#include <sstream>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>

#define MIN_SCORE 0
#define MAX_SCORE 100
#define MAX_DATA 1024

Ranking::Ranking() {};

Ranking::Ranking(wstring file) : m_ImportFile(file) {};

bool Ranking::SetImportFileName(wstring& newFile)
{
	if (newFile == m_ImportFile || newFile.empty())
	{
		return false;
	}
	else
	{
		m_ImportFile = newFile;
		return true;
	}
}

wstring Ranking::GetImportFileName() const
{
	return m_ImportFile;
}

wstring Ranking::GetExportFileName() const
{
	return m_ExportFile;
}

bool Ranking::GenerateExportFileName()
{
	//Export file name is <import file name>-grated.txt

	if (m_ImportFile.empty())
	{
		return false;
	}

	auto pos = m_ImportFile.find_last_of(L".");//remove the extension name
	wstring tmpFile;
	if (pos == wstring::npos)
	{
		tmpFile = m_ImportFile;
	}
	else
	{
		tmpFile = m_ImportFile.substr(0, pos);
	}

	m_ExportFile = tmpFile.append(L"-graded.txt");

	return true;
}

bool Ranking::ParseLine(const string& line)
{
	if (line.empty())
		return false;

	auto pos1 = line.find_first_of(",");
	if (pos1 == 0 || pos1 == string::npos)
		return false;

	auto pos2 = line.find_first_of(",", pos1 + 1);
	if (pos2 == 0 || pos2 == string::npos)
		return false;

	auto scoreStr = line.substr(pos2 + 1);
	istringstream buffer(scoreStr.c_str());
	unsigned short value = -1;
	buffer >> value;

	if (value < MIN_SCORE || value > MAX_SCORE)
		return false;

	//order by (MAX_SCORE - score) + name
	m_ScanList.emplace(make_pair((MAX_SCORE - value), line));
	return true;
}

bool Ranking::ImportFile()
{
	ifstream infile;
	infile.open(m_ImportFile.c_str());
	std::string str;
	while (getline(infile, str))
	{
		if (!ParseLine(str))
		{
			infile.close();
			return false;
		}
	}

	infile.close();

	return true;
}

bool Ranking::ExportToText()
{
	if (m_ScanList.empty())
		return false;

	string data;
	for (auto it = m_ScanList.begin(); it != m_ScanList.end(); ++it)
	{
		data += it->second + "\n";
	}

	HANDLE hFile;
	DWORD dwBytesToWrite = (DWORD)strlen(data.c_str());
	DWORD dwBytesWritten = 0;
	BOOL bErrorFlag = FALSE;

	hFile = CreateFile(m_ExportFile.c_str(),                // name of the write
		GENERIC_WRITE,          // open for writing
		0,                      // do not share
		NULL,                   // default security
		CREATE_ALWAYS,          // create new file always
		NULL,                   // normal file
		NULL);                  // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	_tprintf(TEXT("Writing %d bytes to %s.\n"), dwBytesToWrite, m_ExportFile.c_str());

	bErrorFlag = WriteFile(
		hFile,           // open file handle
		data.c_str(),      // start of data to write
		dwBytesToWrite,  // number of bytes to write
		&dwBytesWritten, // number of bytes that were written
		NULL);            // no overlapped structure

	if (FALSE != bErrorFlag)
	{
		if (dwBytesWritten != dwBytesToWrite)
		{
			printf("Error: dwBytesWritten != dwBytesToWrite\n");
			return false;
		}
		else
		{
			_tprintf(TEXT("Wrote %d bytes to %s successfully.\n"), dwBytesWritten, m_ExportFile.c_str());
		}
	}

	CloseHandle(hFile);

	return true;
}

bool Ranking::Execute()
{
	if (m_ImportFile.empty())
		return false;

	if (!GenerateExportFileName())
		return false;

	if (!ImportFile())
		return false;

	if (!ExportToText())
		return false;

	return true;
}