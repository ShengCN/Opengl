#include "Utilities.h"
#include "Common.h"

std::vector<std::string> Get_All_Files(std::string dirName)
{
	std::vector<std::string> allFiles;
	
	// file System
	dirName.append("*");
	std::wstring tmp(dirName.begin(), dirName.end());
	WIN32_FIND_DATA FindFileData;
	const HANDLE hFind = FindFirstFile(tmp.c_str(), &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		DEBUG("FindFirstFile failed (%d)\n", GetLastError());
	}
	do
	{
		if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			std::wstring tmp(FindFileData.cFileName);
			const std::string str(tmp.begin(), tmp.end());
			allFiles.push_back(str);
		}
	} while (FindNextFile(hFind, &FindFileData));
	FindClose(hFind);

	return allFiles;
}
