#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct StbImageData
{
	int Width, Height, ChannelsCount;
	GLenum Format;
	unsigned char* StbData;
	bool BLoadSuccess;
};
/*TODO: IMPLEMENT IMAGE CLASS*/
class StbImageWrapper
{
public:
	
	static void LoadImage(const std::string& filePath, StbImageData& imageData);

	static void FreeImageData(StbImageData& imageData);
};

