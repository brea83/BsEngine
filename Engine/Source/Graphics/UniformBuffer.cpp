#include "BsPrecompileHeader.h"
#include "UniformBuffer.h"
#include "glm/gtc/type_ptr.hpp"

namespace Pixie
{
	UniformBuffer::UniformBuffer(uint32_t sizeOfData, uint32_t glEnumUsage, uint32_t slot/* const std::string& uniformBlockName, std::vector<uint32_t> shaderIDs*/)
	{
		//for (uint32_t shaderID : shaderIDs)
		//{
			//uint32_t blockIndex = glGetUniformBlockIndex(shaderID, uniformBlockName.c_str());
			//glUniformBlockBinding(shaderID, blockIndex, 0); // TODO: determine whether binding point 0 needs to be cached and changed if using multiple uniform buffer objects
		//}
		m_Slot = slot;
		m_Size = sizeOfData;
		glGenBuffers(1, &m_UniformBufferID);
		Bind();
		glBufferData(GL_UNIFORM_BUFFER, sizeOfData, NULL, glEnumUsage);
		UnBind();
		glBindBufferRange(GL_UNIFORM_BUFFER, slot, m_UniformBufferID, 0, sizeOfData);
	}

	void UniformBuffer::UpdateBool(uint32_t dataOffset, bool data)
	{
		int boolAsInt = data;
		glBufferSubData(GL_UNIFORM_BUFFER, dataOffset, 4, &boolAsInt);
	}

	void UniformBuffer::UpdateInt(uint32_t dataOffset, int data)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, dataOffset, 4, &data);
	}

	void UniformBuffer::UpdateFloat(uint32_t dataOffset, float data)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, dataOffset, 4, &data);
	}

	void UniformBuffer::UpdateVec4(uint32_t dataOffset, glm::vec4 & dataRef)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, dataOffset, sizeof(glm::vec4), glm::value_ptr(dataRef));
	}

	void UniformBuffer::UpdateMat4(uint32_t dataOffset, glm::mat4 & dataRef)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, dataOffset, sizeof(glm::mat4), glm::value_ptr(dataRef));
	}

	void UniformBuffer::UpdateRaw(uint32_t dataOffset, uint32_t sizeOfData, const char* data)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, dataOffset, sizeOfData, data);
	}

	void UniformBuffer::Bind()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferID);
	}

	void UniformBuffer::UnBind()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}