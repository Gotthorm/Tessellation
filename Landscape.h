// LANDSCAPE.H

#ifndef __LANDSCAPE_H__
#define __LANDSCAPE_H__

#include <string>
#include <gl/GL.h>
#include <glm/glm.hpp>
#include "Volstagg.h"
#include "HeightMap.h"

class Landscape
{
public:
	Landscape();
	virtual ~Landscape();

	bool Initialize();

	void Render( const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix );

	bool Load( const std::string& filename );

	bool GetHeight( const glm::vec2& position, float& height);

private:
	Volstagg m_Volstagg;
	HeightMap m_HeightMap;
	glm::vec2 m_OriginOffset;
	unsigned short int m_HeightScale;
};

#endif /* __LANDSCAPE_H__ */

