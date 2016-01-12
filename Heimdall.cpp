// HEIMDALL.CPP

#include <windows.h>
#include "OpenGLInterface.h"
#include "Heimdall.h"
#include <stdio.h>
#include "HeightMap.hpp"
#include "Common/Vector3.hpp"
#include "KTX.h"

const std::string shaderName("landscape");

Heimdall::Heimdall()
{
}

Heimdall::~Heimdall()
{
}

bool Heimdall::InitializeAllUniformVariables()
{
	if( InitializeUniformVariable( "mv_matrix", m_UniformModelViewMatrix ) && 
		InitializeUniformVariable( "mvp_matrix", m_UniformModelViewPointMatrix ) && 
		InitializeUniformVariable( "proj_matrix", m_UniformProjectionMatrix ) )
	{
		return true;
	}

	return false;
}

void Heimdall::UpdateUniformVariables(const Matrix4& projectionMatrix, const Matrix4& viewMatrix)
{
	OpenGLInterface::UniformMatrix4fv(m_UniformModelViewMatrix, 1, GL_FALSE, viewMatrix);
	OpenGLInterface::UniformMatrix4fv(m_UniformProjectionMatrix, 1, GL_FALSE, projectionMatrix);
	//OpenGLInterface::UniformMatrix4fv( m_UniformModelMatrix, 1, GL_FALSE, m_Orientation );

	const float* rawData01 = (const float*)projectionMatrix;
	vmath::mat4 farty01(
		vmath::vec4( rawData01[ 0 ], rawData01[ 1 ], rawData01[ 2 ], rawData01[ 3 ] ),
		vmath::vec4( rawData01[ 4 ], rawData01[ 5 ], rawData01[ 6 ], rawData01[ 7 ] ),
		vmath::vec4( rawData01[ 8 ], rawData01[ 9 ], rawData01[ 10 ], rawData01[ 11 ] ),
		vmath::vec4( rawData01[ 12 ], rawData01[ 13 ], rawData01[ 14 ], rawData01[ 15 ] ) );

	const float* rawData02 = (const float*)viewMatrix;
	vmath::mat4 farty02(
		vmath::vec4( rawData02[ 0 ], rawData02[ 1 ], rawData02[ 2 ], rawData02[ 3 ] ),
		vmath::vec4( rawData02[ 4 ], rawData02[ 5 ], rawData02[ 6 ], rawData02[ 7 ] ),
		vmath::vec4( rawData02[ 8 ], rawData02[ 9 ], rawData02[ 10 ], rawData02[ 11 ] ),
		vmath::vec4( rawData02[ 12 ], rawData02[ 13 ], rawData02[ 14 ], rawData02[ 15 ] ) );

	OpenGLInterface::UniformMatrix4fv( m_UniformModelViewPointMatrix, 1, GL_FALSE, farty01 * farty02 );
	//OpenGLInterface::UniformMatrix4fv( m_UniformModelViewPointMatrix, 1, GL_FALSE, projectionMatrix * viewMatrix );

	// Update the light position
}

bool Heimdall::Load(const std::string& filename)
{
	if (Initialize(shaderName) == true)
	{
#if 0
		HeightMap theHMDFile;

		if (theHMDFile.Load(filename.c_str()) == false)
		{
			return false;
		}

		this->Destroy();

		const float planeScale = 10.0f;
		const float heightScale = 0.1f;

		unsigned int mapWidth = theHMDFile.GetWidthX();
		unsigned int mapHeight = theHMDFile.GetWidthZ();
		unsigned int pointCount = mapWidth * mapHeight;
		unsigned int squareCount = (mapWidth - 1) * (mapHeight - 1);
		unsigned int normalCount = 2 * squareCount * 3;
		unsigned int vertexCount = squareCount * 6;
		unsigned int dataSize = (vertexCount * 4 + normalCount * 3);

		OpenGLInterface::GenVertexArrays(1, &m_VertexArrayObject);
		OpenGLInterface::BindVertexArray(m_VertexArrayObject);
		OpenGLInterface::GenBuffers(1, &m_DataBuffer);
		OpenGLInterface::BindBuffer(GL_ARRAY_BUFFER, m_DataBuffer);
		OpenGLInterface::BufferData(GL_ARRAY_BUFFER, dataSize * sizeof(float), NULL, GL_STATIC_DRAW);

		float* dataBuffer = new float[dataSize];

		float* vertices = dataBuffer;
		float* normals = dataBuffer + (vertexCount * 4);

		const unsigned short* heights = theHMDFile.GetData();

		for (unsigned heightIndex = 0; heightIndex < (mapHeight - 1); ++heightIndex)
		{
			for (unsigned widthIndex = 0; widthIndex < (mapWidth - 1); ++widthIndex)
			{
				unsigned int linearIndex = heightIndex * mapWidth + widthIndex;

				Vector3 v1((widthIndex + 0) * planeScale, heights[linearIndex + 0] * heightScale, (heightIndex + 0) * planeScale);
				Vector3 v2((widthIndex + 1) * planeScale, heights[linearIndex + 1 + mapWidth] * heightScale, (heightIndex + 1) * planeScale);
				Vector3 v3((widthIndex + 1) * planeScale, heights[linearIndex + 1] * heightScale, (heightIndex + 0) * planeScale);
				Vector3 v4((widthIndex + 0) * planeScale, heights[linearIndex + 0 + mapWidth] * heightScale, (heightIndex + 1) * planeScale);

				Vector3 n1 = (v2 - v1).UnitCross(v3 - v1);
				Vector3 n2 = (v4 - v1).UnitCross(v2 - v1);

				(*vertices++) = v1[0];
				(*vertices++) = v1[1];
				(*vertices++) = v1[2];
				(*vertices++) = 1.0f;

				(*vertices++) = v2[0];
				(*vertices++) = v2[1];
				(*vertices++) = v2[2];
				(*vertices++) = 1.0f;

				(*vertices++) = v3[0];
				(*vertices++) = v3[1];
				(*vertices++) = v3[2];
				(*vertices++) = 1.0f;

				(*normals++) = n1[0];
				(*normals++) = n1[1];
				(*normals++) = n1[2];

				(*normals++) = n1[0];
				(*normals++) = n1[1];
				(*normals++) = n1[2];

				(*normals++) = n1[0];
				(*normals++) = n1[1];
				(*normals++) = n1[2];

				(*vertices++) = v1[0];
				(*vertices++) = v1[1];
				(*vertices++) = v1[2];
				(*vertices++) = 1.0f;

				(*vertices++) = v4[0];
				(*vertices++) = v4[1];
				(*vertices++) = v4[2];
				(*vertices++) = 1.0f;

				(*vertices++) = v2[0];
				(*vertices++) = v2[1];
				(*vertices++) = v2[2];
				(*vertices++) = 1.0f;

				(*normals++) = n2[0];
				(*normals++) = n2[1];
				(*normals++) = n2[2];

				(*normals++) = n2[0];
				(*normals++) = n2[1];
				(*normals++) = n2[2];

				(*normals++) = n2[0];
				(*normals++) = n2[1];
				(*normals++) = n2[2];
			}
		}

		OpenGLInterface::BufferSubData(GL_ARRAY_BUFFER, 0, dataSize * sizeof(float), dataBuffer);

		OpenGLInterface::VertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(uintptr_t)0);
		OpenGLInterface::EnableVertexAttribArray(0);

		OpenGLInterface::VertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(uintptr_t)(vertexCount * 4 * sizeof(float)));
		OpenGLInterface::EnableVertexAttribArray(1);

		delete[] dataBuffer;

		OpenGLInterface::BindVertexArray(0);
		OpenGLInterface::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_ObjectCount = vertexCount;
#else
		//OpenGLInterface::GenVertexArrays( 1, &m_VertexArrayObject );
		//OpenGLInterface::BindVertexArray( m_VertexArrayObject );

		OpenGLInterface::PatchParameteri( GL_PATCH_VERTICES, 4 );

		tex_displacement = KTX::load( "Media/Textures/terragen1.ktx" );
		OpenGLInterface::ActiveTexture( GL_TEXTURE1 );
		tex_color = KTX::load( "Media/Textures/terragen_color.ktx" );

		//m_Orientation = Matrix4::LookAt( Vector3( 0.0f, 4.0f, 8.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ) );
#endif
		return true;
	}

	return false;
}

void Heimdall::Draw()
{
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	//glDrawArrays( GL_TRIANGLES, 0, m_ObjectCount );
	//glDrawArrays( GL_PATCHES, 0, 4 );
	OpenGLInterface::DrawArraysInstanced( GL_PATCHES, 0, 4, 64 * 64 );
}

void Heimdall::FartyPants( vmath::mat4& projectionMatrix, vmath::mat4& viewMatrix )
{
	m_Shader.Use();
	OpenGLInterface::UniformMatrix4fv( m_UniformModelViewMatrix, 1, GL_FALSE, viewMatrix );
	OpenGLInterface::UniformMatrix4fv( m_UniformProjectionMatrix, 1, GL_FALSE, projectionMatrix );
	OpenGLInterface::UniformMatrix4fv( m_UniformModelViewPointMatrix, 1, GL_FALSE, projectionMatrix * viewMatrix );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	OpenGLInterface::DrawArraysInstanced( GL_PATCHES, 0, 4, 64 * 64 );
	//glDrawArrays( GL_PATCHES, 0, 4 );
}
