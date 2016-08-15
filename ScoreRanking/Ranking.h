// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef _456AF3C6_71A0_49A1_B747_0560142EB606_
#define _456AF3C6_71A0_49A1_B747_0560142EB606_

#include <string>
#include <map>

using namespace std;

class Ranking
{
public:
	/// \brief Default constructor
	Ranking();

	/// \brief Parameterized constructor
	Ranking(wstring);

	/// \brief Set import file name
	/// \param[in] import file name
	/// \param[out] successfully or not
	bool SetImportFileName(wstring&);

	/// \brief Get import file name
	/// \param[out] import file name
	wstring GetImportFileName() const;

	/// \ brief Execute the ranking algorithm
	/// \param[out] successfully or not
	bool Execute();

	/// \brief Destructor
	virtual ~Ranking() {};

private:
	/// \brief Generate export file name
	/// \param[out] successfully or not
	/// \remarks Export file name is <import file name>-graded.txt
	bool GenerateExportFileName();

	/// \brief Get export file name
	/// \param[out] export file name
	wstring GetExportFileName() const;

	/// \brief Import file
	/// \param[out] successfully or not
	bool ImportFile();

	/// \brief Export file to a text file
	/// \param[out] successfully or not
	bool ExportToText();

	/// \brief Parse a line
	/// \param[in] string of a line
	/// \param[out] successfully or not
	bool ParseLine(const string&);// Does the line include correct information like "surname,firstname,score"
	
	//import file name
	wstring m_ImportFile;

	//export file name
	wstring m_ExportFile;

	typedef multimap<unsigned short, string> scanList;//ordered by (MAX_SCORE - score) + name

	//Ordered lines
	scanList m_ScanList;

	Ranking(const Ranking&) = delete;
	Ranking& operator=(Ranking&) = delete;
};


#endif


