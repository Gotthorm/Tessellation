// LANDSCAPE.CPP

#include <windows.h>
#include "Landscape.h"
#include "PNG.h"

Landscape::Landscape()
{

}

Landscape::~Landscape()
{

}

bool Landscape::Initialize()
{
	// Load the height map
	PNG pngFile;

	m_OriginOffset = glm::vec2(512,512);

	if( pngFile.Load( "Media/Textures/Island.png" ) )
	{
		if( m_HeightMap.Import( pngFile.GetData(), pngFile.GetWidth(), pngFile.GetHeight(), pngFile.GetHasAlpha() ? HeightMap::RGBA : HeightMap::RGB ) )
		{
			m_Volstagg.Load( "Media/Textures/Island.png" );

			return true;
		}
	}

	return false;
}

void Landscape::Render( const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix )
{
	m_Volstagg.Render( projectionMatrix, viewMatrix );
}

bool Landscape::GetHeight( const glm::vec2& position, float& height )
{
	// Current scale of landscape is 16
	unsigned int posX = (unsigned int)( ( ( position[ 0 ] + m_OriginOffset[ 0 ] ) / 16 ) + 0.5f ) * 16;
	unsigned int posY = (unsigned int)( ( ( position[ 1 ] + m_OriginOffset[ 1 ] ) / 16 ) + 0.5f ) * 16;

	unsigned int dataIndex = posX * m_HeightMap.GetWidth() + posY;

	height = (float)m_HeightMap.GetData()[dataIndex];

	return true;
}