// LOKI.H

#ifndef __LOKI_H__
#define __LOKI_H__

#include "RenderObject.h"

class Loki : public RenderObject
{
public:
	Loki();
    virtual ~Loki();

    bool Load(const std::string& filename);

private:
	virtual bool InitializeAllUniformVariables();
	virtual void UpdateUniformVariables(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix);
	virtual void Draw();

	GLint m_UniformModelMatrix;
	GLint m_UniformViewMatrix;
	GLint m_UniformProjectionMatrix;
};

#endif /* __LOKI_H__ */

