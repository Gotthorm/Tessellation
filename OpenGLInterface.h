#pragma once

#include <Windows.h>
#include <gl\GL.h>
//#include "glcorearb.h"
#include "glext.h"
#include <string>

struct OpenGLInterface
{
	static bool Initialize();
	static GLuint LoadShader(const char * filename, GLenum shader_type, bool check_errors);
	static void* GetOpenGLFunctionAddress(const char* name);
	static GLuint LoadTextureFromPNG(const char* fileName, unsigned int tex = 0);
	static GLuint CreateTextureFromImageData( const unsigned char* imageData, unsigned int height, unsigned int width, bool hasAlpha, unsigned int tex );

	static PFNGLCREATEPROGRAMPROC CreateProgram;
	static PFNGLCREATESHADERPROC CreateShader;
	static PFNGLSHADERSOURCEPROC ShaderSource;
	static PFNGLCOMPILESHADERPROC CompileShader;
	static PFNGLATTACHSHADERPROC AttachShader;
	static PFNGLLINKPROGRAMPROC LinkProgram;
	static PFNGLGENVERTEXARRAYSPROC GenVertexArrays;
	static PFNGLBINDVERTEXARRAYPROC BindVertexArray;
	static PFNGLUSEPROGRAMPROC UseProgram;
	static PFNGLDELETEVERTEXARRAYSPROC DeleteVertexArrays;
	static PFNGLDELETEPROGRAMPROC DeleteProgram;
	static PFNGLDELETESHADERPROC DeleteShader;
	static PFNGLACTIVETEXTUREPROC ActiveTexture;
	static PFNGLBINDTEXTUREUNITPROC BindTextureUnit;
	static PFNGLTEXSTORAGE1DPROC TexStorage1D;
	static PFNGLTEXSTORAGE2DPROC TexStorage2D;
	static PFNGLTEXSTORAGE3DPROC TexStorage3D;
	static PFNGLCOMPRESSEDTEXIMAGE2DPROC CompressedTexImage2D;
	static PFNGLTEXSUBIMAGE2DEXTPROC TexSubImage2D;
	static PFNGLTEXSUBIMAGE3DPROC TexSubImage3D;
	static PFNGLGENERATEMIPMAPPROC GenerateMipmap;
	static PFNGLVERTEXATTRIB4FVPROC VertexAttrib4fv;
	static PFNGLVERTEXATTRIB1FPROC VertexAttrib1f;
	static PFNGLGETOBJECTPARAMETERIVARBPROC GetObjectParameterivARB;
	static PFNGLGETSHADERINFOLOGPROC GetShaderInfoLog;

	//typedef void (APIENTRY* PFNGLCLEARBUFFERFVPROC)(GLenum, GLint, const GLfloat*);
	static PFNGLCLEARBUFFERFVPROC ClearBufferfv;
	static PFNGLCREATEBUFFERSPROC CreateBuffers;
	static PFNGLNAMEDBUFFERSTORAGEPROC NamedBufferStorage;
	static PFNGLBUFFERSUBDATAPROC BufferSubData;
	static PFNGLBINDBUFFERPROC BindBuffer;
	static PFNGLGENBUFFERSPROC GenBuffers;
	static PFNGLVERTEXATTRIBPOINTERPROC VertexAttribPointer;
	static PFNGLENABLEVERTEXATTRIBARRAYPROC EnableVertexAttribArray;
	static PFNGLDELETEBUFFERSPROC DeleteBuffers;
	static PFNGLGETUNIFORMLOCATIONPROC GetUniformLocation;
	static PFNGLUNIFORMMATRIX4FVPROC UniformMatrix4fv;
	static PFNGLUNIFORM3FPROC Uniform3f;
	static PFNGLUNIFORM1FPROC Uniform1f;
	static PFNGLUNIFORM1IPROC Uniform1i;
	static PFNGLDRAWARRAYSINSTANCEDPROC DrawArraysInstanced;
	static PFNGLPATCHPARAMETERIPROC PatchParameteri;
	static PFNGLGETSHADERIVPROC GetShaderiv;
	static PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC CompressedTexSubImage1D;
	static PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC CompressedTexSubImage2D;
	static PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC CompressedTexSubImage3D;

	//typedef void (APIENTRY* PFNGLBUFFERDATAPROC)(GLenum, GLuint, const GLvoid *, GLenum);
	static PFNGLBUFFERDATAPROC BufferData;
	static PFNGLCREATETEXTURESPROC CreateTextures;

	static PFNGLGETPROGRAMIVPROC GetProgramiv;
	static PFNGLISSHADERPROC IsShader;
	static PFNGLISPROGRAMPROC IsProgram;

	//typedef void (APIENTRY* PFNGLDRAWARRAYSPROC)(GLenum, GLint, GLsizei);
	//static PFNGLDRAWARRAYSPROC DrawArrays;

	static std::wstring VersionString;
};
