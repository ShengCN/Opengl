#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
#include <sstream>

std::vector<std::string> Get_All_Files(std::string dirName);

// TODO
std::wstring DisplayPathInfo(const std::string dirName);

// todo
template<typename T> void Print_Container(T c)
{
	int index = 0;
	std::for_each(std::begin(c),std::end(c),[&](auto& e)
	{
		std::cout << index++ << " value: " << e << std::endl;
	});
}