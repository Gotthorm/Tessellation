// FRAMEWORK.CPP

#include <assert.h>
#include <windows.h>
#include <stdio.h>
#include <TCHAR.H>
#include "Framework.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "Common\Vector3.hpp"
#include "Common\Matrix3.hpp"
//#include "Game.h"
#include "Input.h"
//#include "HeightMap.hpp"
//#include "Logger.h"
#include <sstream>
#include <iostream>
//#include "Common\Matrix4.hpp"
#include "vmath.h"
#include "KTX.h"

#pragma comment(lib, "winmm.lib")


const DWORD kOneSecond = 1000;
const DWORD kMaxStringLength = 256;
const float kPlayerStartingX = 500.0f;
const float kPlayerStartingZ = 500.0f;
const float kPlayerRadius = 1.0f;
const float kCameraOffsetDistance = 10.0f;
const float kCameraFollowHeightAngle = 20.0f * Math::DEG_TO_RAD;
const float kCameraMinimumHeightAboveGround = 2.0f;

Framework::Framework() : m_pInput( NULL ), m_OldFrameTime(0), m_OneSecondIntervalAccumulator(0), m_UpdateAccumulator(0), m_CurrentFPS(0), m_WindowHandleToDeviceContext(NULL)
{
}

Framework::~Framework()
{
	assert(m_pInput == NULL);
}

bool Framework::Init( HINSTANCE hInstance, HWND hWindow, const LaunchInfo& launchInfo )
{
	RECT rect;
	if (GetClientRect(hWindow, &rect) == FALSE)
	{
		OutputDebugString(L"Failed to extract the initial window size");
		return false;
	}

	unsigned int width = rect.right - rect.left;
	unsigned int height = rect.bottom - rect.top;

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
		PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
		32,                        //Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                        //Number of bits for the depthbuffer
		8,                        //Number of bits for the stencilbuffer
		0,                        //Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	m_WindowHandleToDeviceContext = GetDC(hWindow);

	if (m_WindowHandleToDeviceContext == NULL)
	{
		return false;
	}

	int letWindowsChooseThisPixelFormat = ChoosePixelFormat(m_WindowHandleToDeviceContext, &pfd);

	if (letWindowsChooseThisPixelFormat == 0)
	{
		return false;
	}

	if (SetPixelFormat(m_WindowHandleToDeviceContext, letWindowsChooseThisPixelFormat, &pfd) == FALSE)
	{
		return false;
	}

	HGLRC ourOpenGLRenderingContext = wglCreateContext(m_WindowHandleToDeviceContext);

	if (ourOpenGLRenderingContext == NULL)
	{
		return false;
	}

	if (wglMakeCurrent(m_WindowHandleToDeviceContext, ourOpenGLRenderingContext) == FALSE)
	{
		return false;
	}

	// Initialize the OpenGL functions we will be using
	// This must be called after we make our OpenGL rendering context above
	OpenGLInterface::Initialize();

#if 1
	// Load data file
	//m_Loki.Load("Media/Objects/dragon.sbm");
	//m_Thor.Load("Media/Landscape.heightmap");

	//GLuint vs = OpenGLInterface::LoadShader("Media/Shaders/render.vs.glsl", GL_VERTEX_SHADER, true);
	//GLuint fs = OpenGLInterface::LoadShader("Media/Shaders/render.fs.glsl", GL_FRAGMENT_SHADER, true);

	//static_program = OpenGLInterface::CreateProgram();
	//OpenGLInterface::AttachShader(static_program, vs);
	//OpenGLInterface::AttachShader(static_program, fs);
	//OpenGLInterface::LinkProgram(static_program);

	//OpenGLInterface::DeleteShader(fs);
	//OpenGLInterface::DeleteShader(vs);

	//uniforms.proj_matrix = OpenGLInterface::GetUniformLocation(static_program, "proj_matrix");
	//uniforms.mv_matrix = OpenGLInterface::GetUniformLocation(static_program, "mv_matrix");
	//uniforms.view_matrix = OpenGLInterface::GetUniformLocation(static_program, "view_matrix");

#endif

	//static float LightPosition[] = { 0.0f, 10000.0f, 3000.0f, 0.0f };
	//static float LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	//static float LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };

#if 1
	// Set OpenGl to use counter clockwise triangles
	//glFrontFace(GL_CCW);

	// Set up the z-buffering comparison test
	//glDepthFunc(GL_LEQUAL);

	//// Turn on the z buffering
	//glEnable(GL_DEPTH_TEST);
#endif

	//	glShadeModel( GL_SMOOTH );							// Enable Smooth Shading

	//glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	//glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	//glEnable(GL_LIGHT1);

	//glEnable(GL_LIGHTING);

	//glClearColor(0.0f, 0.5f, 0.9f, 0.0f);

	//memset(g_aKeyboardState, 0, sizeof(g_aKeyboardState));

	//g_OldTimeStamp = GetTickCount();

	//camera = new Camera(glm::vec3(0.0f, 0.0f, 100.0f));

#if 0
	m_CameraPosition = m_PlayerPosition = Vector3(kPlayerStartingX, m_Land.GetHeight(kPlayerStartingX, kPlayerStartingZ) + kPlayerRadius, kPlayerStartingZ);
#else
	m_CameraPosition = m_PlayerPosition = Vector3::ZERO;
#endif

	m_CameraOrientation = m_PlayerOrientation = -Vector3::UNIT_Z;

	//m_CameraPosition = Vector3(0, 0, 100);
	m_CameraPosition = Vector3( 0.0f, 4.0f, 8.0f );

	m_CameraTargetPitch = m_CameraCurrentPitch = 0.0f;
	m_CameraTargetYaw = 0.0F;
	m_CameraCurrentYaw = 180.0f;

	m_FOVX = 45.0f;

	assert(m_pInput == NULL);
	m_pInput = new Input();
	assert(m_pInput != NULL);

	if (m_pInput->Init() == false)
	{
		return false;
	}

	// Initialize the stats related variables
	m_OldFrameTime = timeGetTime();
	m_OneSecondIntervalAccumulator = 0;
	m_UpdateAccumulator = 0;
	m_CurrentFPS = 0;

	// Initialize the 2D text system
	m_Text2D.init(128, 50);

	ResizeWindow(width, height);

#if 1
	//if( program )
	//	OpenGLInterface::DeleteProgram( program );

	m_Volstagg.Load( "do nothing" );
	//m_Heimdall.Load( "Media/Landscape.heightmap" );
	//m_Shader.Load("landscape");
	//uniforms.mv_matrix = m_Shader.GetUniformVariableLocation( "mv_matrix" );
	//uniforms.mvp_matrix = m_Shader.GetUniformVariableLocation( "mvp_matrix" );
	//uniforms.proj_matrix = m_Shader.GetUniformVariableLocation( "proj_matrix" );
	//OpenGLInterface::PatchParameteri( GL_PATCH_VERTICES, 4 );
	//tex_displacement = KTX::load( "Media/Textures/terragen1.ktx" );
	//OpenGLInterface::ActiveTexture( GL_TEXTURE1 );
	//tex_color = KTX::load( "Media/Textures/terragen_color.ktx" );
#endif

    return true;
}

void Framework::Shutdown()
{
	if (m_pInput)
	{
		m_pInput->Shutdown();
		delete m_pInput;
		m_pInput = NULL;
	}
	m_Text2D.Shutdown();
}

void Framework::Update()
{
	// Get the current time in milliseconds since the computer was turned on
	DWORD newFrameTime = timeGetTime();
	
	// Calculate the amount of milliseconds since the last update
	DWORD timeElapsed = newFrameTime - m_OldFrameTime;

	static float totalTime = 0.0f;
	totalTime += (timeElapsed / 1000.0f);

	// If someone's computer has been running for 49 days, the counter may wrap over
	if (newFrameTime < m_OldFrameTime)
	{
		timeElapsed = newFrameTime + (MAXDWORD - m_OldFrameTime);
	}

	// Update the 1 second accumulator
	m_OneSecondIntervalAccumulator += timeElapsed;

	// Increment the update accumulator;
	++m_UpdateAccumulator;

	if (m_OneSecondIntervalAccumulator >= kOneSecond)
	{
		DWORD secondElapsed = 0;

		while (m_OneSecondIntervalAccumulator >= kOneSecond)
		{
			++secondElapsed;
			m_OneSecondIntervalAccumulator -= kOneSecond;
		}

		m_CurrentFPS = m_UpdateAccumulator / secondElapsed;

		m_UpdateAccumulator = 0;
	}

#if 0
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT, GL_FILL);

	std::stringstream s;
	s.precision(2);
	s << std::fixed << "Position(" << m_CameraPosition[0] << ", " << m_CameraPosition[1] << ", " << m_CameraPosition[2] << ") Orientation(" << m_CameraOrientation[0] << ", " << m_CameraOrientation[1] << ", " << m_CameraOrientation[2] << ")";

	m_Text2D.drawText(s.str().c_str(), 0, 0);
	//m_Text2D.drawText("This is a test", 0, 0);

	m_Text2D.draw();
#endif

	// Update the old time for the next update
	m_OldFrameTime = newFrameTime;

	UpdateCamera(timeElapsed);

#if 0
	// This is from the UpdatePlayer method
	m_PlayerPosition[1] = m_Land.GetHeight(m_PlayerPosition[0], m_PlayerPosition[2]) + kPlayerRadius;

	UpdateCamera(timeElapsed);

	glEnable(GL_CULL_FACE);

	// This is the wireframe mode
	glDisable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT, GL_LINE);
#endif

#if 0


	glPushMatrix();
	{
		glLoadIdentity();

		//if (g_nRequestedCameraMode == FLY)
		{
			gluLookAt(m_CameraPosition[0], m_CameraPosition[1], m_CameraPosition[2],
				m_CameraPosition[0] + m_CameraOrientation[0],
				m_CameraPosition[1] + m_CameraOrientation[1],
				m_CameraPosition[2] + m_CameraOrientation[2],
				0.0, 1.0, 0.0);
		}
		//else
		//{
			//gluLookAt(m_CameraPosition[0], m_CameraPosition[1], m_CameraPosition[2],
			//	m_PlayerPosition[0],
			//	m_PlayerPosition[1],
			//	m_PlayerPosition[2],
			//	0.0, 1.0, 0.0);
		//}

		glColor3f(0.0f, 1.0f, 0.0f);

		//glBindTexture(GL_TEXTURE_2D, g_aTextureID[0]);
		m_Land.Reset(m_CameraPosition, m_CameraOrientation);
		m_Land.Tessellate(m_CameraPosition);
		m_Land.Render();

		//glBegin(GL_TRIANGLES);
		//glNormal3f(0, 1, 0);
		//glVertex3f(0, 0, 0);
		//glVertex3f(0, 0, 0);
		//glVertex3f(0, 0, 0);
		//glEnd();

		glPushMatrix();
		{
			//	glDisable( GL_TEXTURE_2D );
			glColor3f(1.0f, 0.0f, 0.0f);
			//glBindTexture(GL_TEXTURE_2D, g_aTextureID[1]);

			glTranslatef(m_PlayerPosition[0], m_PlayerPosition[1], m_PlayerPosition[2]);

			//glutSolidSphere( 1.0f, 16, 16 );
			// Get a Quadric off the stack
			GLUquadricObj* pObj = gluNewQuadric();
			// Draw the sphere normally
			gluQuadricDrawStyle(pObj, GLU_FILL);
			// This turns on texture coordinates for our Quadric
			gluQuadricTexture(pObj, true);
			// Draw the sphere with a radius of fRadius, nSlices slices, nStacks stacks
			gluSphere(pObj, 1.0f, 16, 16);
			// Free the Quadric
			gluDeleteQuadric(pObj);
		}
		glPopMatrix();
	}
	glPopMatrix();

#endif

#if 1
	static const GLfloat black[] = { 0.85f, 0.95f, 1.0f, 1.0f };
	static const GLfloat one = 1.0f;
	//static bool wireframe = true;
	//static bool enable_fog = true;
	static bool enable_displacement = true;


	//float t = (float)totalTime * 0.03f;
	//float r = sinf( t * 5.37f ) * 15.0f + 16.0f;
	//float h = cosf( t * 4.79f ) * 2.0f + 3.2f;

	//Vector3 Orientation = m_CameraPosition + m_CameraOrientation;
	//vmath::vec3 cameraPosition( m_CameraPosition.X, m_CameraPosition.Y, m_CameraPosition.Z );
	//vmath::vec3 lookAtPosition( Orientation.X, Orientation.Y, Orientation.Z );

	//vmath::mat4 mv_matrix = vmath::lookat( cameraPosition, lookAtPosition, vmath::vec3( 0.0f, 1.0f, 0.0f ) );
	//vmath::mat4 mv_matrix = vmath::lookat( vmath::vec3( 0, 4, 8 ), vmath::vec3( 0.0f ), vmath::vec3( 0.0f, 1.0f, 0.0f ) );

	Matrix4 viewMatrix = Matrix4::LookAt( m_CameraPosition, m_CameraPosition + m_CameraOrientation, Vector3::UNIT_Y );
	//Matrix4 viewMatrix = Matrix4::IDENTITY;

	//float* rawData = mv_matrix;
	//Matrix4 mv_matrix_heimdall(	
	//	rawData[ 0 ],
	//	rawData[ 1 ],
	//	rawData[ 2 ],
	//	rawData[ 3 ],
	//	rawData[ 4 ],
	//	rawData[ 5 ],
	//	rawData[ 6 ],
	//	rawData[ 7 ],
	//	rawData[ 8 ],
	//	rawData[ 9 ],
	//	rawData[ 10 ],
	//	rawData[ 11 ],
	//	rawData[ 12 ],
	//	rawData[ 13 ],
	//	rawData[ 14 ],
	//	rawData[ 15 ]);

	//vmath::mat4 proj_matrix = vmath::perspective( 60.0f, (float)windowWidth / (float)windowHeight, 0.1f, 1000.0f );
	Matrix4 projectionMatrix = Matrix4::Perspective( 60.0f, (float)windowWidth / (float)windowHeight, 0.1f, 10000.0f );

	//vmath::mat4 mvp_matrix = proj_matrix * mv_matrix;
	//Matrix4 mvp_matrix_heimdall = proj_matrix_heimdall * mv_matrix_heimdall;

	glViewport( 0, 0, windowWidth, windowHeight );
	OpenGLInterface::ClearBufferfv( GL_COLOR, 0, black );
	OpenGLInterface::ClearBufferfv( GL_DEPTH, 0, &one );

	//m_Shader.Use();
	//OpenGLInterface::UniformMatrix4fv( uniforms.mv_matrix, 1, GL_FALSE, mv_matrix );
	//OpenGLInterface::UniformMatrix4fv( uniforms.proj_matrix, 1, GL_FALSE, proj_matrix );
	//OpenGLInterface::UniformMatrix4fv( uniforms.mvp_matrix, 1, GL_FALSE, proj_matrix * mv_matrix );
	//glEnable( GL_DEPTH_TEST );
	//glDepthFunc( GL_LEQUAL );
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	//OpenGLInterface::DrawArraysInstanced( GL_PATCHES, 0, 4, 64 * 64 );

	//m_Heimdall.Render( proj_matrix_heimdall, viewMatrix );
	//m_Heimdall.Render( proj_matrix_heimdall, mv_matrix_heimdall );
	m_Volstagg.Render( projectionMatrix, viewMatrix );
	//m_Heimdall.FartyPants( proj_matrix, mv_matrix );
#if 0
	//OpenGLInterface::UseProgram( program );
	m_Shader.Use();

	OpenGLInterface::UniformMatrix4fv( uniforms.mv_matrix, 1, GL_FALSE, mv_matrix );
	OpenGLInterface::UniformMatrix4fv( uniforms.proj_matrix, 1, GL_FALSE, proj_matrix );
	OpenGLInterface::UniformMatrix4fv( uniforms.mvp_matrix, 1, GL_FALSE, proj_matrix * mv_matrix );
	//OpenGLInterface::Uniform1f( uniforms.dmap_depth, enable_displacement ? dmap_depth : 0.0f );
	//OpenGLInterface::Uniform1i( uniforms.enable_fog, enable_fog ? 1 : 0 );

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );

	//if( wireframe )
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	//else
	//	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	OpenGLInterface::DrawArraysInstanced( GL_PATCHES, 0, 4, 64 * 64 );
#endif

#else
	// Clear the colorbuffer
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable( GL_CULL_FACE );
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );

	// Activate our shaders
	//OpenGLInterface::UseProgram(static_program);

	float t = (float)totalTime * 0.03f;
	float r = Math::Sin( t * 5.37f ) * 15.0f + 16.0f;
	float h = Math::Cos( t * 4.79f ) * 2.0f + 3.2f;

	// Create camera matrix
	//Matrix4 view_matrix = Matrix4::LookAt(m_CameraPosition, m_CameraPosition + m_CameraOrientation, Vector3::UNIT_Y);
	//Matrix4 view_matrix = Matrix4::LookAt( Vector3( Math::Sin( t ) * r, h, Math::Cos( t ) * r ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ) );
	//Matrix4 view_matrix = Matrix4::LookAt( Vector3( 0.0f, 4.0f, 8.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ) );
	Matrix4 view_matrix = Matrix4::IDENTITY;

	// Create the projection matrix
	//Matrix4 proj_matrix = Matrix4::Perspective( 50.0f, (float)windowWidth / (float)windowHeight, 0.1f, 10000.0f );
	Matrix4 proj_matrix = Matrix4::Perspective( 60.0f, (float)windowWidth / (float)windowHeight, 0.1f, 10000.0f );

	// Pass the matrices to the shader
	//OpenGLInterface::UniformMatrix4fv(uniforms.view_matrix, 1, GL_FALSE, view_matrix);
	//OpenGLInterface::UniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, proj_matrix);

	m_Heimdall.Render( proj_matrix, view_matrix );
	//m_Thor.Render( proj_matrix, view_matrix );
	//m_Loki.Render(proj_matrix, view_matrix);
#endif

#if 1
	//glDisable(GL_CULL_FACE);
	//glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT, GL_FILL);

	std::stringstream s;
	s.precision(2);
	s << std::fixed << "FPS:" << m_CurrentFPS << " Position(" << m_CameraPosition[0] << ", " << m_CameraPosition[1] << ", " << m_CameraPosition[2] << ") Orientation(" << m_CameraOrientation[0] << ", " << m_CameraOrientation[1] << ", " << m_CameraOrientation[2] << ")";

	m_Text2D.drawText(s.str().c_str(), 0, 0);
	//m_Text2D.drawText("This is a test", 0, 0);

	OutputDebugStringA( s.str().c_str() );
	OutputDebugStringA( "\n" );
	//m_Text2D.draw();
#endif

	SwapBuffers(m_WindowHandleToDeviceContext);

	m_pInput->AdvanceFrame();
}

void Framework::UpdateInput(LPARAM lParam)
{
	assert(m_pInput != NULL);

	if (m_pInput != NULL)
	{
		m_pInput->Update(lParam);
	}
}

void Framework::UpdateCamera(DWORD timeElapsed)
{
	// nTimeDelta is the number of milliseconds that has elapsed since the last frame
#if 1
	if (m_pInput != NULL)
	{
		float deltaTime = timeElapsed / 1000.0f;

		if (m_pInput->GetKey(Input::KEY_MOUSE_RIGHT))
		{
			long deltaX;
			long deltaY;
			m_pInput->GetMouse(deltaX, deltaY);

			//camera->ProcessMouseMovement((float)deltaX, (float)-deltaY);
			m_CameraTargetPitch = (float)-deltaY;
			m_CameraTargetYaw = (float)-deltaX;
		}

		const float mouseSpeed = 0.25f;

		m_CameraCurrentYaw += (m_CameraTargetYaw * mouseSpeed);
		m_CameraCurrentPitch -= (m_CameraTargetPitch * mouseSpeed);

		m_CameraTargetPitch = m_CameraTargetYaw = 0.0f;

		Matrix3 theYawMatrix(Vector3::UNIT_Y, Math::DEG_TO_RAD * m_CameraCurrentYaw);
		Matrix3 thePitchMatrix(Vector3::UNIT_X, Math::DEG_TO_RAD * m_CameraCurrentPitch);
		Vector3 theUpVector = thePitchMatrix * Vector3::UNIT_Y;

		Vector3 orientationMatrix = (theYawMatrix * thePitchMatrix) * Vector3::UNIT_Z;
		m_CameraOrientation[0] = orientationMatrix[0];
		m_CameraOrientation[1] = orientationMatrix[1];
		m_CameraOrientation[2] = orientationMatrix[2];

		Vector3 rightVector = orientationMatrix.Cross(theUpVector);

		// 10 units per second
		float MovementIncrement = 100.0f * (timeElapsed / 1000.0f);

		if (m_pInput->GetKey(Input::KEY_W))
		{
			//camera->ProcessKeyboard(FORWARD, deltaTime);
			m_CameraPosition = m_CameraPosition + (m_CameraOrientation * MovementIncrement);
		}
		else if (m_pInput->GetKey(Input::KEY_S))
		{
			//camera->ProcessKeyboard(BACKWARD, deltaTime);
			m_CameraPosition = m_CameraPosition - (m_CameraOrientation * MovementIncrement);
		}

		if (m_pInput->GetKey(Input::KEY_A))
		{
			//camera->ProcessKeyboard(LEFT, deltaTime);
			m_CameraPosition = m_CameraPosition - (rightVector * MovementIncrement);
		}
		else if (m_pInput->GetKey(Input::KEY_D))
		{
			//camera->ProcessKeyboard(RIGHT, deltaTime);
			m_CameraPosition = m_CameraPosition + (rightVector * MovementIncrement);
		}
	}
#else
	//if (g_nRequestedCameraMode == FLY)
	{
		if (m_pInput->GetKey(Input::KEY_MOUSE_RIGHT))
		{
			long deltaX;
			long deltaY;
			m_pInput->GetMouse(deltaX, deltaY);
			//g_CameraPanActive = true;
			m_CameraTargetPitch = (float)-deltaY;
			m_CameraTargetYaw = (float)-deltaX;
		}

		m_CameraCurrentYaw += (m_CameraTargetYaw * 0.75f);
		m_CameraCurrentPitch -= (m_CameraTargetPitch * 0.75f);

		m_CameraTargetPitch = m_CameraTargetYaw = 0.0f;

		Matrix3 theYawMatrix(Vector3::UNIT_Y, Math::DEG_TO_RAD * m_CameraCurrentYaw);
		Matrix3 thePitchMatrix(Vector3::UNIT_X, Math::DEG_TO_RAD * m_CameraCurrentPitch);
		Vector3 theUpVector = thePitchMatrix * Vector3::UNIT_Y;

		Vector3 orientationMatrix = (theYawMatrix * thePitchMatrix) * Vector3::UNIT_Z;
		m_CameraOrientation[0] = orientationMatrix[0];
		m_CameraOrientation[1] = orientationMatrix[1];
		m_CameraOrientation[2] = orientationMatrix[2];

		Vector3 rightVector = orientationMatrix.Cross(theUpVector);

		// 10 units per second
		float MovementIncrement = 100.0f * (timeElapsed / 1000.0f);

		if (m_pInput != NULL)
		{
			if (m_pInput->GetKey(Input::KEY_W))
			{
				m_CameraPosition = m_CameraPosition + (m_CameraOrientation * MovementIncrement);
			}
			else if (m_pInput->GetKey(Input::KEY_S))
			{
				m_CameraPosition = m_CameraPosition - (m_CameraOrientation * MovementIncrement);
			}

			if (m_pInput->GetKey(Input::KEY_A))
			{
				m_CameraPosition = m_CameraPosition - (rightVector * MovementIncrement);
			}
			else if (m_pInput->GetKey(Input::KEY_D))
			{
				m_CameraPosition = m_CameraPosition + (rightVector * MovementIncrement);
			}
		}
	}
#endif
}

void Framework::Render()
{

}

void Framework::ResizeWindow(int width, int height)
{
	windowWidth = width;
	windowHeight = height;

	//glViewport(0, 0, width, height);

	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();

	//gluPerspective(m_FOVX, (float)width / (float)height, 1.0, 1000000.0);

	//glMatrixMode(GL_MODELVIEW);
}

bool Framework::LoadStaticData(const char* pFileName, float fXZScale, float yScale)
{


	return true;
}