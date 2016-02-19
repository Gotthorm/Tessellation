// OBJECT.H

#pragma once

#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <glm/glm.hpp>

class Object
{
public:
	const glm::mat4& GetOrientation() { return m_Orientation; }

	void SetOrientation( const glm::mat4& orientationMatrix ) { m_Orientation = orientationMatrix; }

	void SetPosition( const glm::vec3& position ) { m_Orientation[ 3 ] = glm::vec4( position, 1 ); }

	glm::vec3 GetUpDirection() const { return glm::vec3( m_Orientation[ 1 ] ); }

	glm::vec3 GetForwardDirection() const { return glm::vec3( m_Orientation[ 2 ] ); }

	glm::vec3 GetPosition() const { return glm::vec3( m_Orientation[ 3 ] ); }

protected:
	Object() : m_Orientation( 1 ) {}
	virtual ~Object() {}

private:
	glm::mat4 m_Orientation;
};

#endif /* __OBJECT_H__ */

