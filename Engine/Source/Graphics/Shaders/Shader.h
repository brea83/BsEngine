#pragma once
#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
	//------------------ constructors and properties

	Shader(const char* vertPath, const char* fragPath);
	~Shader();

	unsigned int ShaderProgram{ 0 };

	//--------------- methods

	void Compile(const char* vertPath, const char* fragPath);
	
	void SetUniformBool(const std::string& name, bool value) const;
	void SetUniformInt(const std::string& name, int value) const;
	void SetUniformFloat(const std::string& name, float value) const;
	void SetUniformMat4(const std::string& name, glm::mat4& matrix) const;

	void Use();
	void EndUse();

private:
	std::string LoadFile(const char* aPath); // TODO: move this to asset loading later
	unsigned int CompileShader(int glShaderType, const char* filePath);
	unsigned int LinkShader(unsigned int vertexShader, unsigned int fragmentShader);

};

