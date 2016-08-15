#include "stdafx.h"
#include "CppUnitTest.h"

#include <fstream>
#include <sstream>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include "Ranking.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ScoreRanking
{		
	void GenerateTestFile(LPCTSTR file, const char* data)
	{
		HANDLE hFile;
		DWORD dwBytesToWrite = (DWORD)strlen(data);
		DWORD dwBytesWritten = 0;
		BOOL bErrorFlag = FALSE;

		hFile = CreateFile(file,                // name of the write
			GENERIC_WRITE,          // open for writing
			0,                      // do not share
			NULL,                   // default security
			CREATE_ALWAYS,          // create new file always
			NULL,                   // normal file
			NULL);                  // no attr. template

		if (hFile == INVALID_HANDLE_VALUE)
		{
			return;
		}

		_tprintf(TEXT("Writing %d bytes to %s.\n"), dwBytesToWrite, file);

		bErrorFlag = WriteFile(
			hFile,           // open file handle
			data,      // start of data to write
			dwBytesToWrite,  // number of bytes to write
			&dwBytesWritten, // number of bytes that were written
			NULL);            // no overlapped structure

		if (FALSE != bErrorFlag)
		{
			if (dwBytesWritten != dwBytesToWrite)
			{
				// This is an error because a synchronous write that results in
				// success (WriteFile returns TRUE) should write all data as
				// requested. This would not necessarily be the case for
				// asynchronous writes.
				printf("Error: dwBytesWritten != dwBytesToWrite\n");
			}
			else
			{
				_tprintf(TEXT("Wrote %d bytes to %s successfully.\n"), dwBytesWritten, file);
			}
		}

		CloseHandle(hFile);
	}

	TEST_CLASS(UnitTest1)
	{
	public:
		TEST_METHOD(Constructor)
		{
			//Default constructor
			Ranking* test1(new Ranking());
			Assert::IsTrue(test1 != nullptr);
			Assert::IsTrue(test1->GetImportFileName().empty());

			//Parameterized constructor
			wstring file = L"test.txt";
			Ranking* test2(new Ranking(file));
			Assert::IsTrue(test2 != nullptr);
			Assert::AreEqual(file, test2->GetImportFileName());
		}

		TEST_METHOD(SetImportFileName)
		{
			Ranking* test(new Ranking());
			Assert::IsTrue(test != nullptr);

			wstring file1 = L"";
			bool ok1 = test->SetImportFileName(file1);
			Assert::IsFalse(ok1);

			wstring file2 = L"file.txt";
			bool ok2 = test->SetImportFileName(file2);
			Assert::IsTrue(ok2);
			Assert::AreEqual(file2, test->GetImportFileName());

			wstring file3 = L"file.txt";
			bool ok3 = test->SetImportFileName(file3);
			Assert::IsFalse(ok3);
			Assert::AreEqual(file3, test->GetImportFileName());
		}

		TEST_METHOD(Execute)
		{
			const char* data1 = "A,B,60\nA,C,60\nB,D,70\n";
			LPCTSTR file1 = L"test1.txt";

			GenerateTestFile(file1, data1);

			Ranking* test1(new Ranking(file1));
			Assert::IsTrue(test1 != nullptr);

			bool ok1 = test1->Execute();
			Assert::IsTrue(ok1);

			std::ifstream t("test1-graded.txt");
			std::stringstream buffer;
			buffer << t.rdbuf();

			string result("B,D,70\nA,B,60\nA,C,60\n");

			Assert::IsTrue(buffer.str() == result);

			//negative test
			const char* data2 = "A,B,60\nA,60\nB,D\n";
			LPCTSTR file2 = L"test2.txt";

			GenerateTestFile(file2, data2);

			Ranking* test2(new Ranking(file2));
			Assert::IsTrue(test2 != nullptr);

			bool ok2 = test2->Execute();
			Assert::IsFalse(ok2);
		}
	};
}