// FOLLOWCAMERA.H

#ifndef __FOLLOWCAMERA_H__
#define __FOLLOWCAMERA_H__

#include "Camera.h"

class FollowCamera : public Camera
{
public:
	FollowCamera( const std::string& name, const Object* target );
	virtual ~FollowCamera();

	void Update( DWORD timeElapsed );

	void SetTarget( const Object* target ) { m_Target = target; }

	void SetOffset( const glm::vec2& offset ) {	m_TargetOffset = offset; }

private:
	const Object* m_Target;
	glm::vec2 m_TargetOffset;
};

#endif /* __FOLLOWCAMERA_H__ */

