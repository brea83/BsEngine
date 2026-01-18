#include "BsPrecompileHeader.h"
#include "Shader.h"
#include "Resources/AssetLoader.h"
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

namespace Pixie
{
	Shader::Shader(const std::string& vertPath, const std::string& fragPath, const std::string& geometryPath)
		: Resource(ResourceType::Shader)
	{
		Compile(vertPath, fragPath, geometryPath);
	}

	Shader::~Shader()
	{}

	unsigned int Shader::CompileShader(int glShaderType, const std::string& filePath)
	{
		const char* shaderCode;
		if (glShaderType == GL_FRAGMENT_SHADER)
		{
			m_FragmentSource = AssetLoader::LoadTextFile(filePath);
			shaderCode = m_FragmentSource->Text.c_str();
		}
		else if (glShaderType == GL_VERTEX_SHADER)
		{
			m_VertexSource = AssetLoader::LoadTextFile(filePath);
			shaderCode = m_VertexSource->Text.c_str();
		}
		else if (glShaderType == GL_GEOMETRY_SHADER)
		{
			m_GeometrySource = AssetLoader::LoadTextFile(filePath);
			shaderCode = m_GeometrySource->Text.c_str();
		}
		else
		{
			Logger::Log(LOG_ERROR, "ERROR, TRIED TO COMPILE A SHADER WITH INVALID GL TYPE");
			return 0;
		}

		unsigned int shaderObject;
		shaderObject = glCreateShader(glShaderType);
		glShaderSource(shaderObject, 1, &shaderCode, NULL);
		glCompileShader(shaderObject);

		//check for compile errors
		int success;
		char infoLog[512];
		glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shaderObject, 512, NULL, infoLog);
			std::string shaderTypeName = "";
			if (glShaderType == GL_FRAGMENT_SHADER)
				shaderTypeName = "FRAGMENT";
			else if (glShaderType == GL_VERTEX_SHADER)
				shaderTypeName = "VERTEX";
			else if (glShaderType == GL_GEOMETRY_SHADER)
				shaderTypeName = "GEOMETRY";
			else
				shaderTypeName = "UNKNOWN TYPE";

			Logger::Log(LOG_ERROR, "ERROR, {} SHADER COMPILATION FAILED", shaderTypeName);
			Logger::Log(LOG_ERROR, "{}", infoLog);
			return 0;
		}

		return shaderObject;
	}

	unsigned int Shader::LinkShader(unsigned int vertexShader, unsigned int geometryShader, unsigned int fragmentShader)
	{
		unsigned int shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		if (geometryShader)
			glAttachShader(shaderProgram, geometryShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);


		// Clean up shader parts now that they've been linked into a program
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		if (geometryShader)
			glDeleteShader(geometryShader);

		//check for linking errors
		int success;
		char infoLog[512];
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			Logger::Log(LOG_ERROR, "ERROR SHADER PROGRAM LINKING FAILED");
			Logger::Log(LOG_ERROR, "{}", infoLog);
			return 0;
		}

		return shaderProgram;
	}

	void Shader::Compile(const std::string& vertPath, const std::string& fragPath, const std::string& geometryPath)
	{
		unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertPath);//LoadVertexShader(vertPath);
		if (vertexShader == 0) return;

		unsigned int geometryShader = 0;
		if (!geometryPath.empty())
		{
			geometryShader = CompileShader(GL_GEOMETRY_SHADER, geometryPath);//LoadVertexShader(vertPath);
			if (geometryShader == 0) return;
		}
		unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragPath);//LoadFragmentShader(fragPath);
		if (fragmentShader == 0) return;

		unsigned int linkedProgram = LinkShader(vertexShader, geometryShader, fragmentShader);
		if (linkedProgram == 0) return;
		ShaderProgram = linkedProgram;

		Logger::Log(LOG_TRACE, "Successfully Compiled {}", GetName());
	}

	void Shader::ReCompile(const std::string& barDelineatedPaths)
	{
		std::stringstream ss(barDelineatedPaths);

		std::string segment;
		std::vector<std::string> segmentList;
		char delimiter = '|';
		while (std::getline(ss, segment, delimiter))
		{
			segmentList.emplace_back(segment);
		}

		if (segmentList.size() < 2) return;

		if (!AssetLoader::ReLoadTextFile(segmentList[0])) return;
		if (!AssetLoader::ReLoadTextFile(segmentList[1])) return;
		if (segmentList.size() == 3)
		{
			if (!AssetLoader::ReLoadTextFile(segmentList[2])) return;
			Logger::Log(LOG_TRACE, "Recompiling {}", barDelineatedPaths);
			Compile(segmentList[0], segmentList[1], segmentList[2]);
		}
		else
		{
			Logger::Log(LOG_TRACE, "Recompiling {}", barDelineatedPaths);
			Compile(segmentList[0], segmentList[1]);
		}

	}

	void Shader::SetUniformBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ShaderProgram, name.c_str()), static_cast<int> (value));
	}

	void Shader::SetUniformInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ShaderProgram, name.c_str()), value);
	}

	void Shader::SetUniformFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ShaderProgram, name.c_str()), value);
	}

	void Shader::SetUniformMat4(const std::string& name, glm::mat4& matrix) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::SetUniformVec4(const std::string& name, glm::vec4& vector) const
	{
		glUniform4fv(glGetUniformLocation(ShaderProgram, name.c_str()), 1, glm::value_ptr(vector));
	}

	void Shader::SetUniformVec3(const std::string& name, glm::vec3& vector) const
	{
		glUniform3fv(glGetUniformLocation(ShaderProgram, name.c_str()), 1, glm::value_ptr(vector));
	}

	void Shader::Use()
	{
		glUseProgram(ShaderProgram);
	}

	void Shader::EndUse()
	{
		glUseProgram(0);
	}
	std::string Shader::GetName()
	{
		std::string name = m_VertexSource->GetNameString() + "|";
		if (m_GeometrySource != nullptr)
			name += m_GeometrySource->GetNameString() + "|";
			
		name += m_FragmentSource->GetNameString();

		return name;

		//return std::string(m_VertexSource->FileName.string() + "|" + m_FragmentSource->FileName.string());
	}
}