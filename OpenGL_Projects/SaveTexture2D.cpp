#include"SaveTexture2D.h"
#include "FreeImage.h"
#include <fstream>
#include <iostream>


// Must be RGB texture
// When finished, return true, otherwise return false
bool SaveTexture2D(const std::string filename, GLuint texture, const int width, const int height)
{
	bool isSuccess = false;
	const unsigned int size = width * height * 3;
	BYTE *buffer = new BYTE[size];
	glGetTextureImage(texture, 0, GL_RGB, GL_BYTE, size ,buffer);

	FreeImage_Initialise();
	FIBITMAP* bitmap = FreeImage_Allocate(width, height, 8 * 3);
	RGBQUAD color;

	if(!bitmap)
	{
		std::cout << "Init bitmap failed!\n";
		return false;
	}

	int count = 0;
	for(int i = 0; i < width; ++i)
	{
		for(int j = 0; j < height; ++j)
		{
			color.rgbRed = buffer[count++];
			color.rgbGreen = buffer[count++];
			color.rgbBlue = buffer[count++];
			FreeImage_SetPixelColor(bitmap, j, i, &color);
		}
	}
	if (FreeImage_Save(FIF_PNG, bitmap, "test.png", 0))
	{
		std::cout << "File " << filename << " saved! \n";
		isSuccess = true;
	}
	else
	{
		std::cout << "File " << filename << " saved failed! \n";
		isSuccess = false;
	}
	FreeImage_DeInitialise();
	delete buffer;
	return isSuccess;
}
