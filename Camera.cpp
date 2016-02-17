// CAMERA.CPP

#include <windows.h>
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/compatibility.hpp>

Camera::Camera() : m_Direction( 0.0f, 0.0f, 1.0f ), m_Target( NULL ), m_TargetOffset( 0.0f )
{
}

Camera::~Camera()
{
}

void Camera::SetTarget( const Object* target, const glm::vec2& offset )
{
	m_Target = target;
	m_TargetOffset = offset;
}

void Camera::Update( DWORD timeElapsed )
{
	if( m_Target != NULL )
	{
		// Create a reverse vector from the target to this camera
		glm::vec3 reverseVector = GetPosition() - m_Target->GetPosition();

		// Flatten the vector to the XZ plane based on the target's Y height
		reverseVector.y = 0.0f;

		// Normalize the reverse vector, scale by the x offset, and assign as the target position
		glm::vec3 targetPosition = glm::normalize(reverseVector) * m_TargetOffset.x;

		// Correct the height to the proper Y offset
		targetPosition.y += m_TargetOffset.y;

		// Convert it from an offset to an actual position
		targetPosition += m_Target->GetPosition();

		// Lerp towards the target position
		glm::vec3 newPosition = glm::lerp( GetPosition(), targetPosition, 1.0f );

		// Update the orientation so that we are looking at the target
		glm::mat4 newMatrix = lookAt( newPosition, m_Target->GetPosition(), glm::vec3(0,1,0) );

		SetOrientation( newMatrix );
	}
}