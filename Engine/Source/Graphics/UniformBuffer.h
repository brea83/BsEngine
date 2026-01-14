#pragma once
#include "glm/glm.hpp"
namespace Pixie
{
	class UniformBuffer
	{
	public:
		// data offsets must be in multiples of 16, and glEnumUsage is GL_STATIC_DRAW / GL_DYNAMIC_DRAW / GL_STREAM_DRAW etc
		// also note: layout in locations need to be set in shader files
		UniformBuffer(uint32_t sizeOfData, uint32_t glEnumUsage, uint32_t slot /*const std::string& uniformBlockName, std::vector<uint32_t> shaderIDs*/);

		UniformBuffer() = default;

		void UpdateBool(uint32_t dataOffset, bool data);
		void UpdateInt(uint32_t dataOffset, int data);
		void UpdateFloat(uint32_t dataOffset, float data);
		void UpdateVec4(uint32_t dataOffset, glm::vec4& dataRef);
		void UpdateMat4(uint32_t dataOffset, glm::mat4& dataRef);
		void UpdateRaw(uint32_t dataOffset, uint32_t sizeOfData, const char* data);

		void Bind();
		void UnBind();
	private:
		uint32_t m_UniformBufferID{ 0 };
		uint32_t m_Size{ 0 };
		uint32_t m_Slot{ 0 };

	};


}