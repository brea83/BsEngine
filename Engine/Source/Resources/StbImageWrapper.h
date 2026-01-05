#pragma once
#include <string>
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
#undef LoadImage

namespace Pixie
{
	struct StbImageData
	{
		int Width, Height, ChannelsCount;
		unsigned int Format;
		unsigned char* StbData;
		bool BLoadSuccess;
	};
	class StbImageWrapper
	{
	public:

		static void LoadImage(const std::string& filePath, StbImageData& imageData);

		static void FreeImageData(StbImageData& imageData);
	};

}