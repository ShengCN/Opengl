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

inline float Degree2Radian(float d)
{
	return static_cast<float>((d / 180.0f) * 3.1415926);
}

inline float Radian2Degree(float r)
{
	return static_cast<float>((r / 3.14159265) * 180.0f);
}

