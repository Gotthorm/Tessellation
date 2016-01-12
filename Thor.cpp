// THOR.CPP

#include <windows.h>
#include "OpenGLInterface.h"
#include "Thor.h"
#include <stdio.h>
#include "HeightMap.hpp"
#include "Common/Vector3.hpp"

const std::string shaderName("default");

Thor::Thor()
{
}

Thor::~Thor()
{
}

bool Thor::InitializeAllUniformVariables()
{
	if( InitializeUniformVariable( "model_matrix", m_UniformModelMatrix ) &&
		InitializeUniformVariable( "view_matrix", m_UniformViewMatrix ) &&
		InitializeUniformVariable( "projection_matrix", m_UniformProjectionMatrix ) )
	{
		return true;
	}

	return false;
}

void Thor::UpdateUniformVariables(const Matrix4& projectionMatrix, const Matrix4& viewMatrix)
{
	OpenGLInterface::UniformMatrix4fv(m_UniformViewMatrix, 1, GL_FALSE, viewMatrix);
	OpenGLInterface::UniformMatrix4fv(m_UniformProjectionMatrix, 1, GL_FALSE, projectionMatrix);
	OpenGLInterface::UniformMatrix4fv(m_UniformModelMatrix, 1, GL_FALSE, m_Orientation);

	// Update the light position
}

bool Thor::Load(const std::string& filename)
{
	if (Initialize(shaderName) == true)
	{
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

		return true;
	}

	return false;
}

