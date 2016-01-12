// LOKI.CPP

#include <windows.h>
#include "OpenGLInterface.h"
#include "Loki.h"
#include <stdio.h>
#include "Common/Vector3.hpp"

#define SB6M_FOURCC(a,b,c,d)            ( ((unsigned int)(a) << 0) | ((unsigned int)(b) << 8) | ((unsigned int)(c) << 16) | ((unsigned int)(d) << 24) )
#define SB6M_MAGIC                      SB6M_FOURCC('S','B','6','M')

typedef enum SB6M_CHUNK_TYPE_t
{
	SB6M_CHUNK_TYPE_INDEX_DATA = SB6M_FOURCC('I', 'N', 'D', 'X'),
	SB6M_CHUNK_TYPE_VERTEX_DATA = SB6M_FOURCC('V', 'R', 'T', 'X'),
	SB6M_CHUNK_TYPE_VERTEX_ATTRIBS = SB6M_FOURCC('A', 'T', 'R', 'B'),
	SB6M_CHUNK_TYPE_SUB_OBJECT_LIST = SB6M_FOURCC('O', 'L', 'S', 'T'),
	SB6M_CHUNK_TYPE_COMMENT = SB6M_FOURCC('C', 'M', 'N', 'T'),
	SB6M_CHUNK_TYPE_DATA = SB6M_FOURCC('D', 'A', 'T', 'A')
} SB6M_CHUNK_TYPE;

typedef struct SB6M_HEADER_t
{
	union
	{
		unsigned int    magic;
		char            magic_name[4];
	};
	unsigned int        size;
	unsigned int        num_chunks;
	unsigned int        flags;
} SB6M_HEADER;

typedef struct SB6M_CHUNK_HEADER_t
{
	union
	{
		unsigned int    chunk_type;
		char            chunk_name[4];
	};
	unsigned int        size;
} SB6M_CHUNK_HEADER;

typedef struct SB6M_CHUNK_INDEX_DATA_t
{
	SB6M_CHUNK_HEADER   header;
	unsigned int        index_type;
	unsigned int        index_count;
	unsigned int        index_data_offset;
} SB6M_CHUNK_INDEX_DATA;

typedef struct SB6M_CHUNK_VERTEX_DATA_t
{
	SB6M_CHUNK_HEADER   header;
	unsigned int        data_size;
	unsigned int        data_offset;
	unsigned int        total_vertices;
} SB6M_CHUNK_VERTEX_DATA;

typedef struct SB6M_VERTEX_ATTRIB_DECL_t
{
	char                name[64];
	unsigned int        size;
	unsigned int        type;
	unsigned int        stride;
	unsigned int        flags;
	unsigned int        data_offset;
} SB6M_VERTEX_ATTRIB_DECL;

#define SB6M_VERTEX_ATTRIB_FLAG_NORMALIZED      0x00000001
#define SB6M_VERTEX_ATTRIB_FLAG_INTEGER         0x00000002

typedef struct SB6M_VERTEX_ATTRIB_CHUNK_t
{
	SB6M_CHUNK_HEADER           header;
	unsigned int                attrib_count;
	SB6M_VERTEX_ATTRIB_DECL     attrib_data[1];
} SB6M_VERTEX_ATTRIB_CHUNK;

typedef enum SB6M_DATA_ENCODING_t
{
	SB6M_DATA_ENCODING_RAW = 0
} SB6M_DATA_ENCODING;

typedef struct SB6M_DATA_CHUNK_t
{
	SB6M_CHUNK_HEADER           header;
	unsigned int                encoding;
	unsigned int                data_offset;
	unsigned int                data_length;
} SB6M_DATA_CHUNK;

typedef struct SB6M_SUB_OBJECT_DECL_t
{
	unsigned int                first;
	unsigned int                count;
} SB6M_SUB_OBJECT_DECL;

typedef struct SB6M_CHUNK_SUB_OBJECT_LIST_t
{
	SB6M_CHUNK_HEADER           header;
	unsigned int                count;
	SB6M_SUB_OBJECT_DECL        sub_object[1];
} SB6M_CHUNK_SUB_OBJECT_LIST;

typedef struct SB6M_CHUNK_COMMENT_t
{
	SB6M_CHUNK_HEADER           header;
	char                        comment[1];
} SB6M_CHUNK_COMMENT;

const std::string shaderName("render");

Loki::Loki()
{
}

Loki::~Loki()
{
}

bool Loki::InitializeUniformVariables()
{
	m_UniformProjectionMatrix = m_Shader.GetUniformVariableLocation("projection_matrix");
	m_UniformViewMatrix = m_Shader.GetUniformVariableLocation("view_matrix");
	m_UniformModelMatrix = m_Shader.GetUniformVariableLocation("model_matrix");

	return (m_UniformProjectionMatrix != 0) && (m_UniformViewMatrix != 0) && (m_UniformModelMatrix != 0);
}

void Loki::UpdateUniformVariables(const Matrix4& projectionMatrix, const Matrix4& viewMatrix)
{
	OpenGLInterface::UniformMatrix4fv(m_UniformViewMatrix, 1, GL_FALSE, viewMatrix);
	OpenGLInterface::UniformMatrix4fv(m_UniformProjectionMatrix, 1, GL_FALSE, projectionMatrix);
	OpenGLInterface::UniformMatrix4fv(m_UniformModelMatrix, 1, GL_FALSE, m_Orientation);
}

bool Loki::Load(const std::string& filename)
{
	if (Initialize(shaderName) == true)
	{
		FILE * infile = fopen(filename.c_str(), "rb");
		size_t filesize;
		char * data;

		this->Destroy();

		fseek(infile, 0, SEEK_END);
		filesize = ftell(infile);
		fseek(infile, 0, SEEK_SET);

		data = new char[filesize];

		fread(data, filesize, 1, infile);

		char * ptr = data;
		SB6M_HEADER * header = (SB6M_HEADER *)ptr;
		ptr += header->size;

		SB6M_VERTEX_ATTRIB_CHUNK * vertex_attrib_chunk = NULL;
		SB6M_CHUNK_VERTEX_DATA * vertex_data_chunk = NULL;
		SB6M_CHUNK_INDEX_DATA * index_data_chunk = NULL;
		SB6M_CHUNK_SUB_OBJECT_LIST * sub_object_chunk = NULL;
		SB6M_DATA_CHUNK * data_chunk = NULL;

		unsigned int i;
		for (i = 0; i < header->num_chunks; i++)
		{
			SB6M_CHUNK_HEADER * chunk = (SB6M_CHUNK_HEADER *)ptr;
			ptr += chunk->size;
			switch (chunk->chunk_type)
			{
			case SB6M_CHUNK_TYPE_VERTEX_ATTRIBS:
				vertex_attrib_chunk = (SB6M_VERTEX_ATTRIB_CHUNK *)chunk;
				break;
			case SB6M_CHUNK_TYPE_VERTEX_DATA:
				vertex_data_chunk = (SB6M_CHUNK_VERTEX_DATA *)chunk;
				break;
			default:
				break; // goto failed;
			}
		}

		// failed:

		OpenGLInterface::GenVertexArrays(1, &m_VertexArrayObject);
		OpenGLInterface::BindVertexArray(m_VertexArrayObject);

		unsigned int data_size = vertex_data_chunk->data_size;

		OpenGLInterface::GenBuffers(1, &m_DataBuffer);
		OpenGLInterface::BindBuffer(GL_ARRAY_BUFFER, m_DataBuffer);
		OpenGLInterface::BufferData(GL_ARRAY_BUFFER, data_size, NULL, GL_STATIC_DRAW);

		OpenGLInterface::BufferSubData(GL_ARRAY_BUFFER, 0, data_size, data + vertex_data_chunk->data_offset);

		//for (i = 0; i < vertex_attrib_chunk->attrib_count; i++)
		{
			OpenGLInterface::VertexAttribPointer(0,
				4,
				GL_FLOAT,
				GL_FALSE,
				0,
				(GLvoid *)(uintptr_t)0);
			OpenGLInterface::EnableVertexAttribArray(0);
		}
		{
			OpenGLInterface::VertexAttribPointer(1,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				(GLvoid *)(uintptr_t)4800000);
			OpenGLInterface::EnableVertexAttribArray(1);
		}

		m_ObjectCount = vertex_data_chunk->total_vertices;

		delete[] data;

		fclose(infile);

		OpenGLInterface::BindVertexArray(0);
		OpenGLInterface::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		return true;
	}

	return false;
}