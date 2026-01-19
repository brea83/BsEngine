#include "BsPrecompileHeader.h"
#include "Shader.h"
#include "Resources/AssetLoader.h"
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

namespace Pixie
{
	Shader::Shader(const std::string& vertPath, const std::string& fragPath)
		: Resource(ResourceType::Shader)
	{
		Compile(vertPath, fragPath);
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
			std::string shaderTypeName = glShaderType == GL_VERTEX_SHADER ? "VERTEX " : "FRAGMENT ";

			Logger::Log(LOG_ERROR, "ERROR, {} SHADER COMPILATION FAILED", shaderTypeName);
			Logger::Log(LOG_ERROR, "{}", infoLog);
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
			Logger::Log(LOG_ERROR, "ERROR SHADER PROGRAM LINKING FAILED");
			Logger::Log(LOG_ERROR, "{}", infoLog);
			return 0;
		}

		return shaderProgram;
	}

	void Shader::ExtractUniformMetadata()
	{
		// clear containers
		if (!m_Uniforms.empty())
		{
			m_Uniforms.clear();
			m_UniformCount = 0;
		}

		if (!m_UniformBlocks.empty())
		{
			m_UniformBlockCount = 0;
			m_UniformBlocks.clear();
		}

		// uniform blocks

		glGetProgramInterfaceiv(ShaderProgram, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &m_UniformBlockCount);
		const GLenum blockProperties[4] = { GL_BUFFER_BINDING, GL_BUFFER_DATA_SIZE, GL_NAME_LENGTH, GL_NUM_ACTIVE_VARIABLES };
		const GLenum activeUnifProp[1] = { GL_ACTIVE_VARIABLES };
		const GLenum uniformProperties[4] = { GL_NAME_LENGTH, GL_TYPE, GL_OFFSET, GL_ARRAY_SIZE };

		for (int i = 0; i < m_UniformBlockCount; i++)
		{
			GLint properties[4];
			glGetProgramResourceiv(ShaderProgram, GL_UNIFORM_BLOCK, i, 4, blockProperties, 4, NULL, properties);

			GLint numActiveUniforms = properties[3];
			if (!numActiveUniforms) continue;


			UniformBlockInfo block;
			block.Index = properties[0];
			block.Size = properties[1];
			block.NumVariables = numActiveUniforms;
			block.UniformNames.resize(numActiveUniforms);

			std::vector<char> nameData(properties[2]);
			glGetProgramResourceName(ShaderProgram, GL_UNIFORM_BLOCK, i, nameData.size(), NULL, &nameData[0]);
			std::string name(nameData.begin(), nameData.end() - 1);

			//Logger::Log(LOG_INFO, "Found uniform block index {}, named: {}", block.Index, name);
			//Logger::Log(LOG_INFO, "                    Size: {}", block.Size);
			//Logger::Log(LOG_INFO, "     Num Active Uniforms: {}", block.NumVariables);

			// get a list of the active uniforms
			std::vector<GLint> blockUniforms(block.NumVariables);
			glGetProgramResourceiv(ShaderProgram, GL_UNIFORM_BLOCK, i, 1, activeUnifProp, block.NumVariables, NULL, &blockUniforms[0]);

			for (int j = 0; j < block.NumVariables; j++)
			{
				GLint values[4];
				glGetProgramResourceiv(ShaderProgram, GL_UNIFORM, blockUniforms[j], 4, uniformProperties, 4, NULL, values);

				std::string uniformName = GetUniformNameFromShader(blockUniforms[j], values[0]);
				UniformInfo uniform;
				uniform.Location = values[2];
				uniform.Type = values[1];
				uniform.ArraySize = values[3];
				uniform.BlockIndex = properties[0];
				m_Uniforms[uniformName] = uniform;

				block.UniformNames[j] = uniformName;
				//Logger::Log(LOG_INFO, "Block uniform {} named: {}", UniformInfo::TypeEnumToString(values[1]), uniformName);
				//Logger::Log(LOG_INFO, "     Array length: {}", uniform.ArraySize);
			}
			m_UniformBlocks[name] = block;
		}


		// free floating uniforms 
		glGetProgramInterfaceiv(ShaderProgram, GL_UNIFORM, GL_ACTIVE_RESOURCES, &m_UniformCount);
		const GLenum properties[5] = { GL_BLOCK_INDEX, GL_TYPE, GL_NAME_LENGTH, GL_LOCATION, GL_ARRAY_SIZE };

		if (m_UniformCount == 0) return;

		for (int i = 0; i < m_UniformCount; i++)
		{
			GLint values[5];
			glGetProgramResourceiv(ShaderProgram, GL_UNIFORM, i, 5, properties, 5, NULL, values);

			//skip uniforms that are in a block
			if (values[0] != -1)
			{
				//Logger::Log(LOG_TRACE, "skipping uniform in block number: {}", values[0]);
				continue;
			}

			std::string name = GetUniformNameFromShader(i, values[2]);

			//Logger::Log(LOG_INFO, "Found uniform {} named: {}", UniformInfo::TypeEnumToString(values[1]), name);
			//Logger::Log(LOG_INFO, "     Array length: {}", values[4]);

			UniformInfo uniform;
			uniform.Location = values[3];
			uniform.Type = values[1];
			uniform.ArraySize = values[4];
			uniform.BlockIndex = values[0];

			m_Uniforms[name] = uniform;
		}

		
	}

	std::string Shader::GetUniformNameFromShader(GLint index, GLint nameLength)
	{
		std::vector<char> nameData(nameLength);
		glGetProgramResourceName(ShaderProgram, GL_UNIFORM, index, nameLength, NULL, &nameData[0]);

		std::string name(nameData.begin(), nameData.end() - 1);
		return name;
	}

	void Shader::PrintUniformMetadata(std::string name, UniformInfo uniform) const
	{
		Logger::Log(LOG_TRACE, "{}", name);
		Logger::Log(LOG_TRACE, "........................");
		if(uniform.IsBlockUniform())
			Logger::Log(LOG_TRACE, "     Offset: {}", uniform.Location);
		else
			Logger::Log(LOG_TRACE, "   Location: {}", uniform.Location);

		Logger::Log(LOG_TRACE, "       Type: {}", UniformInfo::TypeEnumToString(uniform.Type));
		Logger::Log(LOG_TRACE, " Array Size: {}", uniform.ArraySize);
		Logger::Log(LOG_TRACE, "Block Index: {}", uniform.BlockIndex);
		Logger::Log(LOG_TRACE, "........................");
	}

	const UniformInfo& Shader::GetUniformInfoByName(std::string name) const
	{
		if (m_Uniforms.find(name) != m_Uniforms.end())
		{
			return m_Uniforms.at(name);
		}
		else
		{
			return UniformInfo();
		}
	}

	void Shader::PrintMetadata() const
	{
		Logger::Log(LOG_TRACE, "========================");
		std::string shaderName = GetName();
		Logger::Log(LOG_TRACE, "Shader Program: {}", shaderName);
		Logger::Log(LOG_TRACE, "ID: {}", ShaderProgram);
		Logger::Log(LOG_TRACE, "________________________");
		Logger::Log(LOG_TRACE, "________________________");

		for (auto pair : m_UniformBlocks)
		{
			std::string name = pair.first;
			UniformBlockInfo block = pair.second;
			Logger::Log(LOG_TRACE, "Block: {}", name);
			Logger::Log(LOG_TRACE, "Index: {}", block.Index);
			Logger::Log(LOG_TRACE, "________________________");

			Logger::Log(LOG_TRACE, "Uniforms");
			Logger::Log(LOG_TRACE, "------------------------");

			for (std::string uniformName : block.UniformNames)
			{
				UniformInfo uniform = GetUniformInfoByName(uniformName);
				if (uniform.IsValid())
				{
					PrintUniformMetadata(uniformName, uniform);
				}
				else
				{
					Logger::Log(LOG_TRACE, "Could not find Uniform named: {}", name);
				}
			}

			Logger::Log(LOG_TRACE, "________________________");
			Logger::Log(LOG_TRACE, "________________________");
		}

		Logger::Log(LOG_TRACE, "Other Uniforms");
		Logger::Log(LOG_TRACE, "------------------------");
		for (auto pair : m_Uniforms)
		{
			std::string name = pair.first;
			UniformInfo uniform = pair.second;

			if (uniform.IsBlockUniform()) continue;
			PrintUniformMetadata(name, uniform);
		}
		Logger::Log(LOG_TRACE, "========================");
	}

	void Shader::Compile(const std::string& vertPath, const std::string& fragPath)
	{
		std::string name = GetName() == "" ? vertPath : GetName();

		Logger::Log(LOG_TRACE, "Compiling shader {}", name);
		unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertPath);//LoadVertexShader(vertPath);
		if (vertexShader == 0) return;
		unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragPath);//LoadFragmentShader(fragPath);
		if (fragmentShader == 0) return;
		Logger::Log(LOG_TRACE, "Linking...");
		unsigned int linkedProgram = LinkShader(vertexShader, fragmentShader);
		if (linkedProgram == 0) return;
		ShaderProgram = linkedProgram;

		Logger::Log(LOG_TRACE, "Getting Uniform Metadata...");
		ExtractUniformMetadata();
		//PrintMetadata();
		Logger::Log(LOG_TRACE, "Compile Completed Successfully");
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

		Compile(segmentList[0], segmentList[1]);
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
	std::string Shader::GetName() const
	{
		if (m_VertexSource == nullptr || m_FragmentSource == nullptr)	return "";
		return std::string(m_VertexSource->FileName.string() + "|" + m_FragmentSource->FileName.string());
	}

	std::string UniformInfo::TypeEnumToString(GLenum type)
	{
		std::string name = "UNKNOWN";
		switch (type)
		{
			case GL_FLOAT								://		float
				return "float";
			case GL_FLOAT_VEC2							://		vec2
				return "vec2";
			case GL_FLOAT_VEC3							://		vec3
				return "vec3";
			case GL_FLOAT_VEC4							://		vec4
				return "vec4";
			//case GL_DOUBLE								://		double
			//case GL_DOUBLE_VEC2							://		dvec2
			//case GL_DOUBLE_VEC3							://		dvec3
			//case GL_DOUBLE_VEC4							://		dvec4
			case GL_INT									://		int
				return "int";
			//case GL_INT_VEC2							://		ivec2
			//case GL_INT_VEC3							://		ivec3
			//case GL_INT_VEC4							://		ivec4
			//case GL_UNSIGNED_INT						://		unsigned int
			//case GL_UNSIGNED_INT_VEC2					://		uvec2
			//case GL_UNSIGNED_INT_VEC3					://		uvec3
			//case GL_UNSIGNED_INT_VEC4					://		uvec4
			case GL_BOOL								://		bool
				return "bool";
			//case GL_BOOL_VEC2							://		bvec2
			//case GL_BOOL_VEC3							://		bvec3
			//case GL_BOOL_VEC4							://		bvec4
			case GL_FLOAT_MAT2							://		mat2
				return "mat2";
			case GL_FLOAT_MAT3							://		mat3
				return "mat3";
			case GL_FLOAT_MAT4							://		mat4
				return "mat4";
			case GL_FLOAT_MAT2x3						://		mat2x3
				return "mat2x3";
			case GL_FLOAT_MAT2x4						://		mat2x4
				return "mat2x4";
			case GL_FLOAT_MAT3x2						://		mat3x2
				return "mat3x2";
			case GL_FLOAT_MAT3x4						://		mat3x4
				return "mat3x4";
			case GL_FLOAT_MAT4x2						://		mat4x2
				return "mat4x2";
			case GL_FLOAT_MAT4x3						://		mat4x3
				return "mat4x3";
			//case GL_DOUBLE_MAT2							://		dmat2
			//case GL_DOUBLE_MAT3							://		dmat3
			//case GL_DOUBLE_MAT4							://		dmat4
			//case GL_DOUBLE_MAT2x3						://		dmat2x3
			//case GL_DOUBLE_MAT2x4						://		dmat2x4
			//case GL_DOUBLE_MAT3x2						://		dmat3x2
			//case GL_DOUBLE_MAT3x4						://		dmat3x4
			//case GL_DOUBLE_MAT4x2						://		dmat4x2
			//case GL_DOUBLE_MAT4x3						://		dmat4x3
			case GL_SAMPLER_1D							://		sampler1D
				return "sampler1D";
			case GL_SAMPLER_2D							://		sampler2D
				return "sampler2D";
			case GL_SAMPLER_3D							://		sampler3D
				return "sampler3D";
			case GL_SAMPLER_CUBE						://		samplerCube
				return "samplerCube";
			//case GL_SAMPLER_1D_SHADOW					://		sampler1DShadow
			//case GL_SAMPLER_2D_SHADOW					://		sampler2DShadow
			case GL_SAMPLER_1D_ARRAY					://		sampler1DArray
				return "sampler1DArray";
			case GL_SAMPLER_2D_ARRAY					://		sampler2DArray
				return "sampler2DArray";
			//case GL_SAMPLER_1D_ARRAY_SHADOW				://		sampler1DArrayShadow
			//case GL_SAMPLER_2D_ARRAY_SHADOW				://		sampler2DArrayShadow
			case GL_SAMPLER_2D_MULTISAMPLE				://		sampler2DMS
				return "sampler2DMS";
			case GL_SAMPLER_2D_MULTISAMPLE_ARRAY		://		sampler2DMSArray
				return "sampler2DMSArray";
			//case GL_SAMPLER_CUBE_SHADOW					://		samplerCubeShadow
			//case GL_SAMPLER_BUFFER						://		samplerBuffer
			//case GL_SAMPLER_2D_RECT						://		sampler2DRect
			//case GL_SAMPLER_2D_RECT_SHADOW				://		sampler2DRectShadow
			//case GL_INT_SAMPLER_1D						://		isampler1D
			//case GL_INT_SAMPLER_2D						://		isampler2D
			//case GL_INT_SAMPLER_3D						://		isampler3D
			//case GL_INT_SAMPLER_CUBE					://		isamplerCube
			//case GL_INT_SAMPLER_1D_ARRAY				://		isampler1DArray
			//case GL_INT_SAMPLER_2D_ARRAY				://		isampler2DArray
			//case GL_INT_SAMPLER_2D_MULTISAMPLE			://		isampler2DMS
			//case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY	://		isampler2DMSArray
			//case GL_INT_SAMPLER_BUFFER					://		isamplerBuffer
			//case GL_INT_SAMPLER_2D_RECT					://		isampler2DRect
			//case GL_UNSIGNED_INT_SAMPLER_1D				://		usampler1D
			//case GL_UNSIGNED_INT_SAMPLER_2D				://		usampler2D
			//case GL_UNSIGNED_INT_SAMPLER_3D				://		usampler3D
			//case GL_UNSIGNED_INT_SAMPLER_CUBE			://		usamplerCube
			//case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY		://		usampler1DArray
			//case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY		://		usampler2DArray
			//case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE	://		usampler2DMS
			//case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY://	usampler2DMSArray
			//case GL_UNSIGNED_INT_SAMPLER_BUFFER			://		usamplerBuffer
			//case GL_UNSIGNED_INT_SAMPLER_2D_RECT		://		usampler2DRect
			default:
				return name;
		}
			
	}
}