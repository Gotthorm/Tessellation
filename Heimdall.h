// HEIMDALL.H

#ifndef __HEIMDALL_H__
#define __HEIMDALL_H__

#include "RenderObject.h"
#include "vmath.h"

class Heimdall : public RenderObject
{
public:
	Heimdall();
    virtual ~Heimdall();

    bool Load(const std::string& filename);

	void FartyPants( vmath::mat4& projectionMatrix, vmath::mat4& viewMatrix );

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

	//GLuint          tex_displacement;
	GLuint          tex_color;
};

#endif /* __HEIMDALL_H__ */

