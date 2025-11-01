#include "Shader.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>



Shader::Shader(const char* vertPath, const char* fragPath)
{
	unsigned int vertexShader = LoadVertexShader(vertPath);
	unsigned int fragmentShader = LoadFragmentShader(fragPath);

	ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, vertexShader);
	glAttachShader(ShaderProgram, fragmentShader);
	glLinkProgram(ShaderProgram);

	//check for linking errors
	int success;
	char infoLog[512];
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ShaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR SHADER PROGRAM LINKING FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// CLEAN UP 
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
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

unsigned int Shader::LoadVertexShader(const char* aPath)
{
	std::string shaderCodeString = LoadFile(aPath);
	const char* shaderCode = shaderCodeString.c_str();

	unsigned int shaderObject;
	shaderObject = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shaderObject, 1, &shaderCode, NULL);
	glCompileShader(shaderObject);

	//check for compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderObject, 512, NULL, infoLog);
		std::cout << "ERROR VERTEX SHADER COMPILATION FAILED\n" << infoLog << std::endl;
	}

	return shaderObject;
}

unsigned int Shader::LoadFragmentShader(const char* aPath)
{
	std::string shaderCodeString = LoadFile(aPath);
	const char* shaderCode = shaderCodeString.c_str();

	unsigned int shaderObject;
	shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shaderObject, 1, &shaderCode, NULL);
	glCompileShader(shaderObject);

	//check for compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderObject, 512, NULL, infoLog);
		std::cout << "ERROR FRAGMENT SHADER COMPILATION FAILED\n" << infoLog << std::endl;
	}

	return shaderObject;
}

void Shader::RecompileShader()
{}

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
