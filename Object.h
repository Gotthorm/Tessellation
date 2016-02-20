// OBJECT.H

#pragma once

#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <glm/glm.hpp>
#include <string>

class Object
{
public:
	const std::string& GetName() const { return m_Name; }

	const glm::mat4& GetOrientation() const { return m_Orientation; }

	void SetOrientation( const glm::mat4& orientationMatrix ) { m_Orientation = orientationMatrix; }

	void SetPosition( const glm::vec3& position ) { m_Orientation[ 3 ] = glm::vec4( position, 1 ); }

	glm::vec3 GetUpDirection() const { return glm::vec3( m_Orientation[ 1 ] ); }

	glm::vec3 GetForwardDirection() const { return glm::vec3( m_Orientation[ 2 ] ); }

	glm::vec3 GetPosition() const { return glm::vec3( m_Orientation[ 3 ] ); }

protected:
	Object(const std::string& name) : m_Orientation( 1 ), m_Name(name) {}
	virtual ~Object() {}

private:
	glm::mat4 m_Orientation;
	std::string m_Name;
};

#endif /* __OBJECT_H__ */

