// LANDSCAPE.CPP

#include <windows.h>
#include "Landscape.h"

Landscape::Landscape()
{

}

Landscape::~Landscape()
{

}

bool Landscape::Initialize()
{
	// Load the height map for the 
	m_Volstagg.Load( "Media/Textures/Island.png" );

	return true;
}

void Landscape::Render( const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix )
{
	m_Volstagg.Render( projectionMatrix, viewMatrix );
}

bool Landscape::GetHeight( const glm::vec2& position, float& height )
{
	height = 0.0f;

	return true;
}