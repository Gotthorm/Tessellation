// SHADER.H

#ifndef __SHADER_H__
#define __SHADER_H__

#include <vector>
#include <string>

class Shader
{
public:
	Shader();
    virtual ~Shader();

	static void SetPath(const char * shaderPath);

    bool Load(const std::string& shaderName);

	bool Use();

	GLuint GetUniformVariableLocation(const std::string& variableName);

private:
	bool TryLoadingShader(GLuint program, std::string shaderPath, unsigned int shaderType);

	void DeleteShaders();

	void LogShaderInfo();

	GLuint m_Program;

	std::vector<GLuint> m_LoadedShaderList;

	static std::string m_Path;
};

#endif /* __SHADER_H__ */

