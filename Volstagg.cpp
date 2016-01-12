// VOLSTAGG.CPP

#include <windows.h>
#include "OpenGLInterface.h"
#include "Volstagg.h"
#include <stdio.h>
#include "HeightMap.hpp"
#include "Common/Vector3.hpp"
#include "KTX.h"

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

void Volstagg::UpdateUniformVariables(const Matrix4& projectionMatrix, const Matrix4& viewMatrix)
{
	OpenGLInterface::UniformMatrix4fv(m_UniformModelViewMatrix, 1, GL_FALSE, viewMatrix);
	OpenGLInterface::UniformMatrix4fv(m_UniformProjectionMatrix, 1, GL_FALSE, projectionMatrix);
}

bool Volstagg::Load(const std::string& filename)
{
	if (Initialize(shaderName) == true)
	{
		//OpenGLInterface::GenVertexArrays( 1, &m_VertexArrayObject );
		//OpenGLInterface::BindVertexArray( m_VertexArrayObject );

		OpenGLInterface::PatchParameteri( GL_PATCH_VERTICES, 4 );

		tex_displacement = KTX::load( "Media/Textures/terragen1.ktx" );
		OpenGLInterface::ActiveTexture( GL_TEXTURE1 );
		tex_color = KTX::load( "Media/Textures/terragen_color.ktx" );

		//m_Orientation = Matrix4::LookAt( Vector3( 0.0f, 4.0f, 8.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ) );
		return true;
	}

	return false;
}

void Volstagg::Draw()
{
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	//glDrawArrays( GL_TRIANGLES, 0, m_ObjectCount );
	//glDrawArrays( GL_PATCHES, 0, 4 );
	OpenGLInterface::DrawArraysInstanced( GL_PATCHES, 0, 4, 64 );
}
