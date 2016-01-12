// RENDEROBJECT.CPP

#include <windows.h>
#include "OpenGLInterface.h"
#include "RenderObject.h"

RenderObject::RenderObject() : m_DataBuffer(0), m_VertexArrayObject(0), m_ObjectCount(0), m_Initialized(false)
{
	m_Orientation = Matrix4::IDENTITY;
}

RenderObject::~RenderObject()
{

}

bool RenderObject::Initialize(const std::string& shaderName)
{
	if (m_Initialized == false)
	{
		if (m_Shader.Load(shaderName))
		{
			if (InitializeAllUniformVariables())
			{
				m_Initialized = true;
			}
		}
	}

	return m_Initialized;
}

bool RenderObject::InitializeAllUniformVariables()
{
	return true;
}

bool RenderObject::InitializeUniformVariable( const char* variableName, GLint& variableId )
{
	GLint result = m_Shader.GetUniformVariableLocation( variableName );

	if( result != GL_INVALID_OPERATION && result != GL_INVALID_VALUE && result != -1 )
	{
		variableId = result;

		return true;
	}

	return false;
}

void RenderObject::UpdateUniformVariables(const Matrix4& projectionMatrix, const Matrix4& viewMatrix)
{
}

void RenderObject::Destroy()
{
	OpenGLInterface::DeleteVertexArrays(1, &m_VertexArrayObject);
	OpenGLInterface::DeleteBuffers(1, &m_DataBuffer);

	m_VertexArrayObject = 0;
	m_DataBuffer = 0;
}

void RenderObject::Render(const Matrix4& projectionMatrix, const Matrix4& viewMatrix)
{
	if (m_Initialized && m_Shader.Use())
	{
		UpdateUniformVariables(projectionMatrix, viewMatrix);

		Draw();
	}
}

// The default rendering of data.
// Derived objects should overload this with any necessary changes
void RenderObject::Draw()
{
	OpenGLInterface::BindVertexArray( m_VertexArrayObject );

	glDrawArrays(GL_TRIANGLES, 0, m_ObjectCount);

	OpenGLInterface::BindVertexArray( 0 );
}
