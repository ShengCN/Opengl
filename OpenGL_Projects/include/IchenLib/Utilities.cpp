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

std::wstring DisplayPathInfo(const std::string dirName)
{
	std::experimental::filesystem::path pathToDisplay(dirName);

	std::wostringstream wos;
	int i = 0;
	wos << L"Displaying path info for: " << pathToDisplay << std::endl;
	for (std::experimental::filesystem::path::iterator itr = pathToDisplay.begin(); itr != pathToDisplay.end(); ++itr)
	{
		wos << L"path part: " << i++ << L" = " << *itr << std::endl;
	}

	wos << L"root_name() = " << pathToDisplay.root_name() << std::endl
		<< L"root_path() = " << pathToDisplay.root_path() << std::endl
		<< L"relative_path() = " << pathToDisplay.relative_path() << std::endl
		<< L"parent_path() = " << pathToDisplay.parent_path() << std::endl
		<< L"filename() = " << pathToDisplay.filename() << std::endl
		<< L"stem() = " << pathToDisplay.stem() << std::endl
		<< L"extension() = " << pathToDisplay.extension() << std::endl;

	return wos.str();
}