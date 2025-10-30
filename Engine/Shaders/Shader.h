#pragma once
#include <string>


class Shader
{
public:
	//------------------ constructors and properties

	Shader(const char* vertPath, const char* fragPath);
	~Shader();

	unsigned int ShaderProgram;

	//--------------- methods
	//add uniform setters here

	unsigned int LoadVertexShader(const char* aPath);
	unsigned int LoadFragmentShader(const char* aPath);

	void RecompileShader();
	
	void SetUniformBool(const std::string& name, bool value) const;
	void SetUniformInt(const std::string& name, int value) const;
	void SetUniformFloat(const std::string& name, float value) const;

	void Use();
	void EndUse();

private:
	std::string LoadFile(const char* aPath); // TODO: move this to asset loading later

};

