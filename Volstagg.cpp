// VOLSTAGG.CPP

#include <windows.h>
#include "OpenGLInterface.h"
#include "Volstagg.h"
#include <stdio.h>
#include "KTX.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const std::string shaderName( "tessellation" );

Volstagg::Volstagg()
{
}

Volstagg::~Volstagg()
{
}

bool Volstagg::InitializeAllUniformVariables()
{
	if( InitializeUniformVariable( "model_matrix", m_UniformModelMatrix ) && 
		InitializeUniformVariable( "view_matrix", m_UniformViewMatrix ) &&
		InitializeUniformVariable( "projection_matrix", m_UniformProjectionMatrix ) )
	{ 
		// Identity matrix
		SetOrientation( glm::rotate(glm::mat4(), glm::radians(90.0f), glm::vec3(1, 0, 0)) );

		return true;
	}

	return false;
}

void Volstagg::UpdateUniformVariables(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix)
{
	OpenGLInterface::UniformMatrix4fv( m_UniformViewMatrix, 1, GL_FALSE, glm::value_ptr( viewMatrix ) );
	OpenGLInterface::UniformMatrix4fv( m_UniformProjectionMatrix, 1, GL_FALSE, glm::value_ptr( projectionMatrix ) );

	OpenGLInterface::UniformMatrix4fv( m_UniformModelMatrix, 1, GL_FALSE, glm::value_ptr( GetOrientation() ) );
}

bool Volstagg::Load(const std::string& filename)
{
	if (Initialize(shaderName) == true)
	{
		OpenGLInterface::PatchParameteri( GL_PATCH_VERTICES, 4 );

		tex_displacement = OpenGLInterface::LoadTextureFromPNG( filename.c_str() );

		OpenGLInterface::ActiveTexture( GL_TEXTURE1 );

		tex_color = OpenGLInterface::LoadTextureFromPNG( "Media/Textures/Green.png" );

		return true;
	}

	return false;
}

void Volstagg::Draw()
{
	OpenGLInterface::ActiveTexture( GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_2D, tex_displacement);

	// Set OpenGl to use counter clockwise triangles
	glFrontFace(GL_CW);
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	//glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glEnable( GL_CULL_FACE );

	OpenGLInterface::DrawArraysInstanced( GL_PATCHES, 0, 4, 4096 );
}
