// HEIMDALL.H

#ifndef __VOLSTAGG_H__
#define __VOLSTAGG_H__

#include "RenderObject.h"
#include "vmath.h"

class Volstagg : public RenderObject
{
public:
	Volstagg();
    virtual ~Volstagg();

    bool Load(const std::string& filename);

private:
	virtual bool InitializeAllUniformVariables();
	virtual void UpdateUniformVariables(const Matrix4& projectionMatrix, const Matrix4& viewMatrix);
	virtual void Draw();

	//GLint m_UniformModelMatrix;
	//GLint m_UniformViewMatrix;
	GLint m_UniformModelViewMatrix;
	GLint m_UniformModelViewPointMatrix;
	GLint m_UniformProjectionMatrix;
	//GLint m_FinalMatrix;

	GLuint          tex_displacement;
	GLuint          tex_color;
};

#endif /* __VOLSTAGG_H__ */

