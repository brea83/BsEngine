#pragma once
#include "Resources/Resource.h"
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

namespace Pixie
{
	class TextResource;
		
	struct UniformInfo
	{
		GLint Location{ -1 };
		GLenum Type{ GL_NONE};
		GLsizei ArraySize{ -1 };
		GLint BlockIndex{ -1 };

		bool IsBlockUniform() { return BlockIndex != -1; }
		bool IsInvalid()
		{
			return Location == -1 && 
				Type == GL_NONE &&
				ArraySize == -1 &&
				BlockIndex == -1;
		}
		bool IsValid() { return !IsInvalid(); }

		static std::string TypeEnumToString(GLenum type);
	};


	struct UniformBlockInfo
	{
		GLint Index;
		GLsizei Size;
		GLsizei NumVariables;
		std::vector<std::string> UniformNames;
	};

	class Shader : public Resource
	{
	public:
		//------------------ constructors and properties

		Shader(const std::string& vertPath, const std::string& fragPath);
		~Shader();

		unsigned int ShaderProgram{ 0 };

		//--------------- methods

		void Compile(const std::string& vertPath, const std::string& fragPath);
		void ReCompile(const std::string& barDelineatedPaths);
		bool IsValid() { return ShaderProgram != 0; }

		void SetUniformBool(const std::string& name, bool value) const;
		void SetUniformInt(const std::string& name, int value) const;
		void SetUniformFloat(const std::string& name, float value) const;
		void SetUniformMat4(const std::string& name, glm::mat4& matrix) const;
		void SetUniformVec4(const std::string& name, glm::vec4& vector) const;
		void SetUniformVec3(const std::string& name, glm::vec3& vector) const;

		void Use();
		void EndUse();

		std::string GetName() const;
		// get uniform info for materials that use this shader
		bool HasUniformName(const std::string& name) const;
		const UniformInfo& GetUniformInfoByName(std::string name) const;
		std::unordered_map<std::string, UniformInfo> GetUniforms() { return m_Uniforms; }

	private:
		std::shared_ptr<TextResource> m_VertexSource;
		std::shared_ptr<TextResource> m_FragmentSource;

		GLint m_UniformCount;
		std::unordered_map<std::string, UniformInfo> m_Uniforms;
		GLint m_UniformBlockCount;
		std::unordered_map<std::string, UniformBlockInfo> m_UniformBlocks;

		unsigned int CompileShader(int glShaderType, const std::string& filePath);
		unsigned int LinkShader(unsigned int vertexShader, unsigned int fragmentShader);
		void ExtractUniformMetadata();

		std::string GetUniformNameFromShader(GLint index, GLint nameLength);
		void PrintUniformMetadata(std::string name, UniformInfo uniform) const;
		void PrintMetadata() const;
	};

}