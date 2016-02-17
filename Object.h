// OBJECT.H

#pragma once

#ifndef __OBJECT_H__
#define __OBJECT_H__

//#include <string>
//#include <gl\GL.h>
#include <glm/glm.hpp>

class Object
{
public:

	const glm::mat4& GetOrientation() { return m_Orientation; }

	void SetOrientation( const glm::mat4& orientationMatrix );

	const glm::vec3& GetPosition() const { return m_Position; }

	void SetPosition( const glm::vec3& position );

protected:
	Object();
	virtual ~Object();

private:
	glm::mat4 m_Orientation;
	glm::vec3 m_Position;
};

#endif /* __OBJECT_H__ */

