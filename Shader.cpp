// SHADER.CPP

#include <windows.h>
#include "OpenGLInterface.h"
#include "Shader.h"

std::string Shader::m_Path = "Media/Shaders/";

Shader::Shader() : m_Program(0)
{
}

Shader::~Shader()
{
}

// Static
void Shader::SetPath(const char* shaderPath)
{
	m_Path = shaderPath;
}

bool Shader::Load(const std::string& shaderName)
{
	GLuint program = OpenGLInterface::CreateProgram();

	if (program != 0)
	{
		std::string shaderPath(m_Path);
		shaderPath += shaderName;

		if (TryLoadingShader(program, shaderPath, GL_VERTEX_SHADER))
		{
			// Log info
			LogShaderInfo();
		}

		if( TryLoadingShader( program, shaderPath, GL_TESS_CONTROL_SHADER ) )
		{
			// Log info
			LogShaderInfo();
		}

		if( TryLoadingShader( program, shaderPath, GL_TESS_EVALUATION_SHADER ) )
		{
			// Log info
			LogShaderInfo();
		}

		if (TryLoadingShader(program, shaderPath, GL_FRAGMENT_SHADER))
		{
			// Log info
			LogShaderInfo();
		}

		// Attempt to link the program
		OpenGLInterface::LinkProgram(program);

		// Delete all of the shaders that were successfuly loaded
		DeleteShaders();

		// Retrieve the results of the program linking step
		GLint params;
		OpenGLInterface::GetProgramiv(program, GL_LINK_STATUS, &params);

		if (params == GL_TRUE)
		{
			m_Program = program;

			return true;
		}
	}

	return false;
}

bool Shader::TryLoadingShader(GLuint program, std::string shaderPath, unsigned int shaderType)
{
	std::string filePath(shaderPath);

	switch (shaderType)
	{
	case GL_VERTEX_SHADER:
		filePath += ".vs";
		break;
	case GL_TESS_CONTROL_SHADER:
		filePath += ".tcs";
		break;
	case GL_TESS_EVALUATION_SHADER:
		filePath += ".tes";
		break;
	case GL_FRAGMENT_SHADER:
		filePath += ".fs";
		break;
	default:
		return false;
	}

	filePath += ".glsl";

	GLuint shaderId = OpenGLInterface::LoadShader(filePath.c_str(), shaderType, true);

	if (shaderId != 0)
	{
		if (OpenGLInterface::IsShader(shaderId) && OpenGLInterface::IsProgram(program))
		{
			OpenGLInterface::AttachShader(program, shaderId);

			// Confirm the previous attach call succeeded
			if (glGetError() == GL_NO_ERROR)
			{
				m_LoadedShaderList.push_back(shaderId);

				return true;
			}
		}
	}

	return false;
}

void Shader::DeleteShaders()
{
	for (std::vector<GLuint>::const_iterator it = m_LoadedShaderList.begin(); it != m_LoadedShaderList.end(); ++it)
	{
		OpenGLInterface::DeleteShader(*it);
	}
}

bool Shader::Use()
{
	if (m_Program != 0)
	{
		OpenGLInterface::UseProgram(m_Program);

		GLenum results = glGetError();

		if( results == GL_NO_ERROR )
		{
			return true;
		}
	}

	return false;
}

GLuint Shader::GetUniformVariableLocation(const std::string& variableName)
{
	GLuint variableLocation = 0;

	if (m_Program != 0)
	{
		variableLocation = OpenGLInterface::GetUniformLocation(m_Program, variableName.c_str());
	}

	return variableLocation;
}

void Shader::LogShaderInfo()
{
	GLint bufferLength;
	GLuint shaderId = m_LoadedShaderList.back();

	OpenGLInterface::GetShaderiv( shaderId, GL_INFO_LOG_LENGTH, &bufferLength );

	if( bufferLength > 0 )
	{
		GLchar* infoBuffer = new GLchar[ bufferLength ];

		GLsizei infoLength;
		OpenGLInterface::GetShaderInfoLog( shaderId, bufferLength, &infoLength, infoBuffer );

		// TODO: Change to message
		OutputDebugStringA( infoBuffer );
		OutputDebugStringA( "\n" );

		delete[] infoBuffer;
	}
}