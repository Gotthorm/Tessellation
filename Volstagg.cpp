// VOLSTAGG.CPP

#include <windows.h>
#include "OpenGLInterface.h"
#include "Volstagg.h"
#include <stdio.h>
#include "KTX.h"
#include "glm/gtc/type_ptr.hpp"

const std::string shaderName( "tessellation" );

Volstagg::Volstagg()
{
}

Volstagg::~Volstagg()
{
}

bool Volstagg::InitializeAllUniformVariables()
{
	if( InitializeUniformVariable( "mv_matrix", m_UniformModelViewMatrix ) && InitializeUniformVariable( "proj_matrix", m_UniformProjectionMatrix ) )
	{ 
		return true;
	}

	return false;
}

void Volstagg::UpdateUniformVariables(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix)
{
	OpenGLInterface::UniformMatrix4fv(m_UniformModelViewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	OpenGLInterface::UniformMatrix4fv(m_UniformProjectionMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

bool Volstagg::Load(const std::string& filename)
{
	if (Initialize(shaderName) == true)
	{
		OpenGLInterface::PatchParameteri( GL_PATCH_VERTICES, 4 );

		tex_displacement = KTX::load( "Media/Textures/terragen1.ktx" );
		OpenGLInterface::ActiveTexture( GL_TEXTURE1 );
		tex_color = KTX::load( "Media/Textures/terragen_color.ktx" );

		return true;
	}

	return false;
}

void Volstagg::Draw()
{
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	OpenGLInterface::DrawArraysInstanced( GL_PATCHES, 0, 4, 64 );
}
