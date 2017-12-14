#pragma once
#include "EasonTriangle.h"

class GrahicsGenerator
{
public:
	GrahicsGenerator();
	~GrahicsGenerator();
	static EasonTriangle GenerateTriangle(float length); // edge length
};

