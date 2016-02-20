// FOLLOWCAMERA.CPP

#include <windows.h>
#include "FollowCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/compatibility.hpp>

FollowCamera::FollowCamera( const std::string& name, const Object* target ) : Camera(name), m_Target( target ), m_TargetOffset( 0.0f )
{
}

FollowCamera::~FollowCamera()
{
}

void FollowCamera::Update( DWORD timeElapsed )
{
	if( m_Target != NULL )
	{
		// Get the normalized direction vector of the target scaled by the correct follow distance
		glm::vec3 directionVector = m_Target->GetForwardDirection() * m_TargetOffset.x;

		// Create the base follow target position
		glm::vec3 targetPosition = m_Target->GetPosition() - directionVector;

		// Now adjust for the desired height
		targetPosition += ( m_Target->GetUpDirection() * m_TargetOffset.y );

		// Update the orientation so that we are looking at the target
		glm::mat4 newMatrix = lookAt( targetPosition, m_Target->GetPosition(), m_Target->GetUpDirection() );

		SetOrientation( newMatrix );
	}
}