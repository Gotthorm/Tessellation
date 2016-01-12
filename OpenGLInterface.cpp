//#include "Windows.h"
#include "OpenGLInterface.h"
#include <string>
#include <fstream>
#include <streambuf>


PFNGLCREATEPROGRAMPROC OpenGLInterface::CreateProgram = NULL;
PFNGLCREATESHADERPROC OpenGLInterface::CreateShader = NULL;
PFNGLSHADERSOURCEPROC OpenGLInterface::ShaderSource = NULL;
PFNGLCOMPILESHADERPROC OpenGLInterface::CompileShader = NULL;
PFNGLATTACHSHADERPROC OpenGLInterface::AttachShader = NULL;
PFNGLLINKPROGRAMPROC OpenGLInterface::LinkProgram = NULL;
PFNGLGENVERTEXARRAYSPROC OpenGLInterface::GenVertexArrays = NULL;
PFNGLBINDVERTEXARRAYPROC OpenGLInterface::BindVertexArray = NULL;
PFNGLUSEPROGRAMPROC OpenGLInterface::UseProgram = NULL;
PFNGLDELETEVERTEXARRAYSPROC OpenGLInterface::DeleteVertexArrays = NULL;
PFNGLDELETEPROGRAMPROC OpenGLInterface::DeleteProgram = NULL;
PFNGLDELETESHADERPROC OpenGLInterface::DeleteShader = NULL;
PFNGLACTIVETEXTUREPROC OpenGLInterface::ActiveTexture = NULL;
PFNGLBINDTEXTUREUNITPROC OpenGLInterface::BindTexture = NULL;
PFNGLTEXSTORAGE1DPROC OpenGLInterface::TexStorage1D = NULL;
PFNGLTEXSTORAGE2DPROC OpenGLInterface::TexStorage2D = NULL;
PFNGLTEXSTORAGE3DPROC OpenGLInterface::TexStorage3D = NULL;
PFNGLCOMPRESSEDTEXIMAGE2DPROC OpenGLInterface::CompressedTexImage2D = NULL;
PFNGLTEXSUBIMAGE2DEXTPROC OpenGLInterface::TexSubImage2D = NULL;
PFNGLTEXSUBIMAGE3DPROC OpenGLInterface::TexSubImage3D = NULL;
PFNGLGENERATEMIPMAPPROC OpenGLInterface::GenerateMipmap = NULL;
PFNGLVERTEXATTRIB4FVPROC OpenGLInterface::VertexAttrib4fv = NULL;
PFNGLVERTEXATTRIB1FPROC OpenGLInterface::VertexAttrib1f = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC OpenGLInterface::GetObjectParameterivARB = NULL;
PFNGLGETSHADERINFOLOGPROC OpenGLInterface::GetShaderInfoLog = NULL;


PFNGLCLEARBUFFERFVPROC OpenGLInterface::ClearBufferfv = NULL;
PFNGLCREATEBUFFERSPROC OpenGLInterface::CreateBuffers = NULL;
PFNGLNAMEDBUFFERSTORAGEPROC OpenGLInterface::NamedBufferStorage = NULL;
PFNGLBINDBUFFERPROC OpenGLInterface::BindBuffer = NULL;
PFNGLBUFFERSUBDATAPROC OpenGLInterface::BufferSubData = NULL;
PFNGLGENBUFFERSPROC OpenGLInterface::GenBuffers = NULL;
PFNGLVERTEXATTRIBPOINTERPROC OpenGLInterface::VertexAttribPointer = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC OpenGLInterface::EnableVertexAttribArray = NULL;
PFNGLDELETEBUFFERSPROC OpenGLInterface::DeleteBuffers = NULL;
PFNGLGETUNIFORMLOCATIONPROC OpenGLInterface::GetUniformLocation = NULL;
PFNGLUNIFORMMATRIX4FVPROC OpenGLInterface::UniformMatrix4fv = NULL;
PFNGLUNIFORM1FPROC OpenGLInterface::Uniform1f = NULL;
PFNGLUNIFORM1IPROC OpenGLInterface::Uniform1i = NULL;
PFNGLDRAWARRAYSINSTANCEDPROC OpenGLInterface::DrawArraysInstanced = NULL;
PFNGLPATCHPARAMETERIPROC OpenGLInterface::PatchParameteri = NULL;
PFNGLGETSHADERIVPROC OpenGLInterface::GetShaderiv = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC OpenGLInterface::CompressedTexSubImage1D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC OpenGLInterface::CompressedTexSubImage2D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC OpenGLInterface::CompressedTexSubImage3D = NULL;

PFNGLBUFFERDATAPROC OpenGLInterface::BufferData = NULL;
//PFNGLDRAWARRAYSPROC OpenGLInterface::DrawArrays = NULL;

PFNGLGETPROGRAMIVPROC OpenGLInterface::GetProgramiv = NULL;
PFNGLISSHADERPROC OpenGLInterface::IsShader = NULL;
PFNGLISPROGRAMPROC OpenGLInterface::IsProgram = NULL;

bool OpenGLInterface::Initialize()
{
	CreateProgram = (PFNGLCREATEPROGRAMPROC)GetOpenGLFunctionAddress("glCreateProgram");
	CreateShader = (PFNGLCREATESHADERPROC)GetOpenGLFunctionAddress("glCreateShader");
	ShaderSource = (PFNGLSHADERSOURCEPROC)GetOpenGLFunctionAddress("glShaderSource");
	CompileShader = (PFNGLCOMPILESHADERPROC)GetOpenGLFunctionAddress("glCompileShader");
	AttachShader = (PFNGLATTACHSHADERPROC)GetOpenGLFunctionAddress("glAttachShader");
	LinkProgram = (PFNGLLINKPROGRAMPROC)GetOpenGLFunctionAddress("glLinkProgram");
	GenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)GetOpenGLFunctionAddress("glGenVertexArrays");
	BindVertexArray = (PFNGLBINDVERTEXARRAYPROC)GetOpenGLFunctionAddress("glBindVertexArray");
	UseProgram = (PFNGLUSEPROGRAMPROC)GetOpenGLFunctionAddress("glUseProgram");
	DeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)GetOpenGLFunctionAddress("glDeleteVertexArrays");
	DeleteProgram = (PFNGLDELETEPROGRAMPROC)GetOpenGLFunctionAddress("glDeleteProgram");
	DeleteShader = (PFNGLDELETESHADERPROC)GetOpenGLFunctionAddress("glDeleteShader");
	ActiveTexture = (PFNGLACTIVETEXTUREPROC)GetOpenGLFunctionAddress("glActiveTexture");
	BindTexture = (PFNGLBINDTEXTUREUNITPROC)GetOpenGLFunctionAddress("glBindTexture");
	TexStorage1D = (PFNGLTEXSTORAGE1DPROC)GetOpenGLFunctionAddress("glTexStorage1D");
	TexStorage2D = (PFNGLTEXSTORAGE2DPROC)GetOpenGLFunctionAddress("glTexStorage2D");
	TexStorage3D = (PFNGLTEXSTORAGE3DPROC)GetOpenGLFunctionAddress("glTexStorage3D");
	CompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC)GetOpenGLFunctionAddress("glCompressedTexImage2D");
	TexSubImage2D = (PFNGLTEXSUBIMAGE2DEXTPROC)GetOpenGLFunctionAddress("glTexSubImage2D");
	TexSubImage3D = (PFNGLTEXSUBIMAGE3DPROC)GetOpenGLFunctionAddress("glTexSubImage3D");
	GenerateMipmap = (PFNGLGENERATEMIPMAPPROC)GetOpenGLFunctionAddress("glGenerateMipmap");
	VertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC)GetOpenGLFunctionAddress("glVertexAttrib4fv");
	VertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC)GetOpenGLFunctionAddress("glVertexAttrib1f");
	GetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)GetOpenGLFunctionAddress("glGetObjectParameterivARB");
	GetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)GetOpenGLFunctionAddress("glGetShaderInfoLog");

	ClearBufferfv = (PFNGLCLEARBUFFERFVPROC)GetOpenGLFunctionAddress("glClearBufferfv");
	CreateBuffers = (PFNGLCREATEBUFFERSPROC)GetOpenGLFunctionAddress("glCreateBuffers");
	NamedBufferStorage = (PFNGLNAMEDBUFFERSTORAGEPROC)GetOpenGLFunctionAddress("glNamedBufferStorage");
	BindBuffer = (PFNGLBINDBUFFERPROC)GetOpenGLFunctionAddress("glBindBuffer");
	BufferData = (PFNGLBUFFERDATAPROC)GetOpenGLFunctionAddress("glBufferData");
	BufferSubData = (PFNGLBUFFERSUBDATAPROC)GetOpenGLFunctionAddress("glBufferSubData");
	GenBuffers = (PFNGLGENBUFFERSPROC)GetOpenGLFunctionAddress("glGenBuffers");
	VertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)GetOpenGLFunctionAddress("glVertexAttribPointer");
	EnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)GetOpenGLFunctionAddress("glEnableVertexAttribArray");
	DeleteBuffers = (PFNGLDELETEBUFFERSPROC)GetOpenGLFunctionAddress("glDeleteBuffers");
	GetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)GetOpenGLFunctionAddress("glGetUniformLocation");
	UniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)GetOpenGLFunctionAddress("glUniformMatrix4fv");
	Uniform1f = (PFNGLUNIFORM1FPROC)GetOpenGLFunctionAddress( "glUniform1f" );
	Uniform1i = (PFNGLUNIFORM1IPROC)GetOpenGLFunctionAddress( "glUniform1i" );
	DrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC)GetOpenGLFunctionAddress("glDrawArraysInstanced");
	PatchParameteri = (PFNGLPATCHPARAMETERIPROC)GetOpenGLFunctionAddress("glPatchParameteri");
	GetShaderiv = (PFNGLGETSHADERIVPROC)GetOpenGLFunctionAddress("glGetShaderiv" );
	CompressedTexSubImage1D = (PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC)GetOpenGLFunctionAddress("glCompressedTexSubImage1D");
	CompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)GetOpenGLFunctionAddress("glCompressedTexSubImage2D");
	CompressedTexSubImage3D = (PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC)GetOpenGLFunctionAddress("glCompressedTexSubImage3D");

	//DrawArrays = (PFNGLDRAWARRAYSPROC)GetOpenGLFunctionAddress("glDrawArrays");

	GetProgramiv = (PFNGLGETPROGRAMIVPROC)GetOpenGLFunctionAddress("glGetProgramiv");
	IsShader = (PFNGLISSHADERPROC)GetOpenGLFunctionAddress("glIsShader");
	IsProgram = (PFNGLISPROGRAMPROC)GetOpenGLFunctionAddress("glIsProgram");

	return true;
}

GLuint OpenGLInterface::LoadShader(const char* filename, GLenum shader_type, bool check_errors)
{
	GLuint result = 0;

	// Attempt to open the given file by name
	std::ifstream t(filename);

	// Test if the file open step was successful
	if ((t.rdstate() & std::ifstream::failbit) == 0)
	{
		std::string str;

		// Determine the size of the file and preallocate the size of the string we will be reading into
		t.seekg(0, std::ios::end);
		str.reserve((size_t)t.tellg());
		t.seekg(0, std::ios::beg);

		// Read the entire file's contents into the string
		str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

		// Create an empty shader of the given type
		// The valid types are:
		// GL_COMPUTE_SHADER
		// GL_VERTEX_SHADER
		// GL_TESS_CONTROL_SHADER
		// GL_TESS_EVALUATION_SHADER
		// GL_GEOMETRY_SHADER
		// GL_FRAGMENT_SHADER
		// Returns zero unpon failure
		result = CreateShader(shader_type);

		if (result)
		{
			const char* data = str.c_str();

			ShaderSource(result, 1, &data, NULL);

			CompileShader(result);

			if (check_errors)
			{
				GLint status = 0;
				GetObjectParameterivARB(result, GL_COMPILE_STATUS, &status);

				if (!status)
				{
					char buffer[4096];
					GetShaderInfoLog(result, 4096, NULL, buffer);

					OutputDebugStringA(filename);
					OutputDebugStringA(":");
					OutputDebugStringA(buffer);
					OutputDebugStringA("\n");

					DeleteShader(result);
				}
			}
		}
		else
		{
			// Failed shader alloc
			OutputDebugStringA("Failed shader alloc\n");
		}
	}
	else
	{
		// Failed to open shader text file
		OutputDebugStringA("Failed to open shader file\n");
	}
		
	return result;
}

void* OpenGLInterface::GetOpenGLFunctionAddress(const char* name)
{
	void* pointer = (void*)wglGetProcAddress(name);

	// Apparently the returned error codes from wglGetProcAddress can be -1, NULL, 1, 2, or 3
	if (pointer == NULL || (pointer == (void*)0x1) || (pointer == (void*)0x2) || (pointer == (void*)0x3) || (pointer == (void*)-1))
	{
		HMODULE module = LoadLibraryA("opengl32.dll");
		pointer = (void*)GetProcAddress(module, name);
	}

	return pointer;
}