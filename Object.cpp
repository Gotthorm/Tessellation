// OBJECT.CPP

#include <windows.h>
#include "Object.h"

Object::Object() : m_Position(0), m_Orientation(1)
{
}

Object::~Object()
{
}

void Object::SetOrientation( const glm::mat4& orientationMatrix )
{
	m_Orientation = orientationMatrix;

	// Extract a copy of the position and cache it
	m_Position = glm::vec3( m_Orientation[3] );
}