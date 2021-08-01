#include "pch.h"
#include "FileIO.h"

using namespace std;
using namespace Geothermal;

/// <summary>
/// Ported from Microsoft tutorial
/// </summary>
string FileIO::ReadData(winrt::hstring const& filename)
{
	CREATEFILE2_EXTENDED_PARAMETERS extendedParams = { 0 };
	extendedParams.dwSize = sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
	extendedParams.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
	extendedParams.dwFileFlags = FILE_FLAG_SEQUENTIAL_SCAN;
	extendedParams.dwSecurityQosFlags = SECURITY_ANONYMOUS;
	extendedParams.lpSecurityAttributes = nullptr;
	extendedParams.hTemplateFile = nullptr;

	winrt::file_handle file
	{
		CreateFile2
		(
			filename.c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			OPEN_EXISTING,
			&extendedParams
		)
	};
	winrt::check_bool(bool{ file });
	if (file.get() == INVALID_HANDLE_VALUE)
	{
		winrt::throw_hresult(E_FAIL);
	}

	FILE_STANDARD_INFO fileInfo = { 0 };
	if (!GetFileInformationByHandleEx(
		file.get(),
		FileStandardInfo,
		&fileInfo,
		sizeof(fileInfo)
	))
	{
		winrt::throw_hresult(E_FAIL);
	}

	if (fileInfo.EndOfFile.HighPart != 0)
	{
		winrt::throw_hresult(E_OUTOFMEMORY);
	}

	char* data = new char[fileInfo.EndOfFile.LowPart];
	if (!ReadFile(
		file.get(),
		data,
		(DWORD)fileInfo.EndOfFile.LowPart,
		nullptr,
		nullptr
	))
	{
		winrt::throw_hresult(E_FAIL);
	}

	string s(data);
	delete[] data;
	return s;
}
