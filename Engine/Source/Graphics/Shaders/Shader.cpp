#include "BsPrecompileHeader.h"
#include "Shader.h"

#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>



Shader::Shader(const char* vertPath, const char* fragPath)
{
	Compile(vertPath, fragPath);
}

Shader::~Shader()
{}

std::string Shader::LoadFile(const char* aPath)
{
	std::ifstream file(aPath);
	if (!file.is_open())
	{
		std::cerr << "Failed to open file: " << aPath << std::endl;
		return "";
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

unsigned int Shader::CompileShader(int glShaderType, const char* filePath)
{
	std::string shaderCodeString = LoadFile(filePath);
	const char* shaderCode = shaderCodeString.c_str();

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
		std::string shaderTypeName = glShaderType == GL_VERTEX_SHADER ? "VERTEX " : "FRAGMENT ";
		std::cout << "ERROR" << shaderTypeName << "SHADER COMPILATION FAILED\n" << infoLog << std::endl;
		return 0;
	}

	return shaderObject;
}

unsigned int Shader::LinkShader(unsigned int vertexShader, unsigned int fragmentShader)
{
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);


	// Clean up shader parts now that they've been linked into a program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//check for linking errors
	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR SHADER PROGRAM LINKING FAILED\n" << infoLog << std::endl;
		return 0;
	}

	return shaderProgram;
}

void Shader::Compile(const char* vertPath, const char* fragPath)
{
	unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertPath);//LoadVertexShader(vertPath);
	if (vertexShader == 0) return;
	unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragPath);//LoadFragmentShader(fragPath);
	if (fragmentShader == 0) return;

	unsigned int linkedProgram = LinkShader(vertexShader, fragmentShader);
	if (linkedProgram == 0) return;
	ShaderProgram = linkedProgram;
	
}

void Shader::SetUniformBool(const std::string & name, bool value) const
{
	glUniform1i(glGetUniformLocation(ShaderProgram, name.c_str()), static_cast<int> (value));
}

void Shader::SetUniformInt(const std::string & name, int value) const
{
	glUniform1i(glGetUniformLocation(ShaderProgram, name.c_str()), value);
}

void Shader::SetUniformFloat(const std::string & name, float value) const
{
	glUniform1f(glGetUniformLocation(ShaderProgram, name.c_str()), value);
}

void Shader::SetUniformMat4(const std::string& name, glm::mat4& matrix) const
{
	glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::Use()
{
	glUseProgram(ShaderProgram);
}

void Shader::EndUse()
{
	glUseProgram(0);
}
