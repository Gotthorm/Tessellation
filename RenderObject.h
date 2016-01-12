// RENDEROBJECT.H

#ifndef __RENDEROBJECT_H__
#define __RENDEROBJECT_H__

#include <string>
#include <gl\GL.h>
#include <glm/glm.hpp>
#include "Shader.h"

class RenderObject
{
public:
	void Render(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix);

	virtual bool Load(const std::string& filename) = 0;

    virtual void Destroy();

protected:
	RenderObject();
	virtual ~RenderObject();

	bool Initialize(const std::string& shaderName);

	// Derived objects should set up all uniform variables that their shader will require
	virtual bool InitializeAllUniformVariables();

	bool InitializeUniformVariable( const char* variableName, GLint& variableId );

	// Derived objects should update the values of all uniform variables here
	virtual void UpdateUniformVariables(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix);

	// Derived objects can override the default GL_TRIANGLES render call with the appropiate functionality
	virtual void Draw();

	GLuint m_DataBuffer;
	GLuint m_ObjectCount;
	GLuint m_VertexArrayObject;

	bool m_Initialized;

	glm::mat4 m_Orientation;
	Shader m_Shader;
};

#endif /* __RENDEROBJECT_H__ */

