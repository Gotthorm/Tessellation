// CAMERA.H

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <glm/glm.hpp>
#include "Object.h"

class Camera : public Object
{
public:
	Camera();
	virtual ~Camera();

	virtual void Update( DWORD timeElapsed ) {}

protected:
	glm::vec3 m_Direction;
};

#endif /* __CAMERA_H__ */

