// HEIMDALL.H

#ifndef __VOLSTAGG_H__
#define __VOLSTAGG_H__

#include "RenderObject.h"

class Volstagg : public RenderObject
{
public:
	Volstagg();
    virtual ~Volstagg();

    bool Load(const std::string& filename);

private:
	virtual bool InitializeAllUniformVariables();
	virtual void UpdateUniformVariables(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix);
	virtual void Draw();

	GLint m_UniformModelViewMatrix;
	GLint m_UniformModelViewPointMatrix;
	GLint m_UniformProjectionMatrix;

	GLuint tex_displacement;
	GLuint tex_color;
};

#endif /* __VOLSTAGG_H__ */

