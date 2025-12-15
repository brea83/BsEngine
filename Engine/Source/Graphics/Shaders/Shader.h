#pragma once
#include <string>
#include <glm/glm.hpp>
#include "Resources/Resource.h"

namespace Pixie
{
	class TextResource;

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
		void SetUniformMat4(const std::string& name, glm::mat4 matrix) const;
		void SetUniformVec4(const std::string& name, glm::vec4 vector) const;

		void Use();
		void EndUse();

	private:
		std::shared_ptr<TextResource> _vertexSource;
		std::shared_ptr<TextResource> _fragmentSource;

		unsigned int CompileShader(int glShaderType, const std::string& filePath);
		unsigned int LinkShader(unsigned int vertexShader, unsigned int fragmentShader);

	};

}