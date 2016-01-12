// THOR.H

#ifndef __THOR_H__
#define __THOR_H__

#include "RenderObject.h"

class Thor : public RenderObject
{
public:
	Thor();
    virtual ~Thor();

    bool Load(const std::string& filename);

private:
	virtual bool InitializeAllUniformVariables();
	virtual void UpdateUniformVariables(const Matrix4& projectionMatrix, const Matrix4& viewMatrix);

	GLint m_UniformModelMatrix;
	GLint m_UniformViewMatrix;
	GLint m_UniformProjectionMatrix;
};

#endif /* __THOR_H__ */

