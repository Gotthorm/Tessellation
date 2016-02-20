// CAMERA.H

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <glm/glm.hpp>
#include "Object.h"

class Camera : public Object
{
public:
	Camera(const std::string& name);
	virtual ~Camera();
};

#endif /* __CAMERA_H__ */

