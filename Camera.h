// CAMERA.H

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <string>
//#include <gl\GL.h>
#include <glm/glm.hpp>
#include "Object.h"

class Camera : public Object
{
public:
	Camera();
	virtual ~Camera();

	void SetTarget( const Object* target, const glm::vec2& offset );

	void Update( DWORD timeElapsed );

private:
	glm::vec3 m_Direction;
	const Object* m_Target;
	glm::vec2 m_TargetOffset;
};

#endif /* __CAMERA_H__ */

