// FRAMEWORK.CPP

#include <assert.h>
#include <windows.h>
#include <stdio.h>
#include <TCHAR.H>
#include "Framework.h"
#include "Input.h"
#include <sstream>
#include <iostream>
//#include "KTX.h"
#include "OpenGLInterface.h"
#include <glm/gtc/matrix_transform.hpp>

#pragma comment(lib, "winmm.lib")


const DWORD kOneSecond = 1000;
const DWORD kMaxStringLength = 256;
const float kPlayerStartingX = 500.0f;
const float kPlayerStartingZ = 500.0f;
const float kPlayerRadius = 1.0f;
const float kCameraOffsetDistance = 10.0f;
const float kCameraFollowHeightAngle = glm::radians(20.0f);
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

	m_CameraPosition = m_PlayerPosition = glm::vec3();

	m_CameraOrientation = m_PlayerOrientation = glm::vec3(0,0,1);

	//m_CameraPosition = Vector3(0, 0, 100);
	m_CameraPosition = glm::vec3( 0.0f, 30.f, 0.0f );

	m_CameraTargetPitch = m_CameraCurrentPitch = 0.0f;
	m_CameraTargetYaw = 0.0f;
	m_CameraCurrentYaw = 0.0f;

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

	ResizeWindow(width, height);

	m_Volstagg.Load( "do nothing" );
	m_Loki.Load( "Media/Objects/cube.sbm" );

	// Initialize the 2D text system
	//m_Text2D.init( 128, 50 );
	
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
	//m_Text2D.Shutdown();
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

	// Update the old time for the next update
	m_OldFrameTime = newFrameTime;

	UpdateCamera(timeElapsed);

	static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static const GLfloat one = 1.0f;
	//static bool wireframe = true;
	//static bool enable_fog = true;
	static bool enable_displacement = true;

	glm::mat4 viewMatrix = glm::lookAt( m_CameraPosition, m_CameraPosition + m_CameraOrientation, glm::vec3(0,1,0) );
	//glm::mat4 viewMatrix = glm::lookAtLH( m_CameraPosition, m_CameraPosition + m_CameraOrientation, glm::vec3( 0, -1, 0 ) );
	//glm::mat4 viewMatrix = glm::lookAtLH( m_CameraPosition, glm::vec3( 0, 0, 0 ), glm::vec3( 0, -1, 0 ) );

	glm::mat4 projectionMatrix = glm::perspective( 45.0f, (float)windowWidth / (float)windowHeight, 0.1f, 10000.0f );
	//glm::mat4 projectionMatrix = glm::perspectiveLH( 50.0f, (float)windowWidth / (float)windowHeight, 0.1f, 10000.0f );

	glViewport( 0, 0, windowWidth, windowHeight );
	OpenGLInterface::ClearBufferfv( GL_COLOR, 0, black );
	OpenGLInterface::ClearBufferfv( GL_DEPTH, 0, &one );

	m_Volstagg.Render( projectionMatrix, viewMatrix );
	m_Loki.Render( projectionMatrix, viewMatrix );

#if 0
	glPolygonMode(GL_FRONT, GL_FILL);

	std::stringstream s;
	s.precision(2);
	s << std::fixed << "FPS:" << m_CurrentFPS << " Position(" << m_CameraPosition[0] << ", " << m_CameraPosition[1] << ", " << m_CameraPosition[2] << ") Orientation(" << m_CameraOrientation[0] << ", " << m_CameraOrientation[1] << ", " << m_CameraOrientation[2] << ")";

	m_Text2D.drawText(s.str().c_str(), 0, 0);

	m_Text2D.draw();
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

		glm::mat4 theYawMatrix = glm::rotate( glm::mat4(), glm::radians( m_CameraCurrentYaw ), glm::vec3( 0, 1.0, 0 ) );
		glm::mat4 thePitchMatrix = glm::rotate( glm::mat4(), glm::radians( m_CameraCurrentPitch ), glm::vec3( 1.0, 0, 0 ) );
		glm::vec4 theUpVector = thePitchMatrix * glm::vec4(0,1,0,1);
		//glm::mat3 theYawMatrix = glm::mat3();
		//glm::mat3 thePitchMatrix = glm::mat3();
		//glm::vec3 theUpVector = glm::vec3(0,1,0);

		glm::vec4 orientationVector = (theYawMatrix * thePitchMatrix) * glm::vec4(0,0,1,1);

		m_CameraOrientation.x = orientationVector.x;
		m_CameraOrientation.y = orientationVector.y;
		m_CameraOrientation.z = orientationVector.z;

		glm::vec3 rightVector = glm::cross( m_CameraOrientation, glm::vec3( theUpVector.x, theUpVector.y, theUpVector.z ) );

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