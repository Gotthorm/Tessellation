// FRAMEWORK.HPP

#ifndef _FRAMEWORK_HPP_
#define _FRAMEWORK_HPP_

#include <windows.h>
#include "Common\Vector3.hpp"
#include "Landscape.hpp"
#include "Text2D.h"
#include "Loki.h"
#include "Thor.h"
#include "Heimdall.h"
#include "Volstagg.h"
// GLM Mathemtics
//#include "glm/glm/glm.hpp"
//#include "glm/glm/gtc/matrix_transform.hpp"
//#include "glm/glm/gtc/type_ptr.hpp"
//#include "camera.h"

class Input;
class Landscape;
class Vector3;

class Framework
{
public:
	struct LaunchInfo
	{
		LPCTSTR applicationTitle;
		int numberOfThreads;
	};

	//
	Framework();

	//
	~Framework();

	//
	bool Init( HINSTANCE hInstance, HWND hWindow, const LaunchInfo& launchInfo );

	// 
	void Shutdown();

	//
	void Update();

	void UpdateInput(LPARAM lParam);

	//
	void Render();

	//
	//void EnableMouseCapture( bool bEnable );

	//
	void ResizeWindow(int width, int height);

private:
	void UpdateCamera(DWORD timeElapsed);
	bool LoadStaticData(const char* pFileName, float fXZScale, float yScale);

	//Graphics* m_pGraphics;
	//Game* m_pGame;
	Input* m_pInput;
	DWORD m_OldFrameTime;
	DWORD m_CurrentFPS;
	DWORD m_OneSecondIntervalAccumulator;
	DWORD m_UpdateAccumulator;
	HDC m_WindowHandleToDeviceContext;
	//Landscape m_Land;
	Vector3	m_CameraPosition;
	Vector3	m_CameraOrientation;
	Vector3 m_PlayerPosition;
	Vector3 m_PlayerOrientation;
	float m_CameraTargetPitch;
	float m_CameraTargetYaw;
	float m_CameraCurrentPitch;
	float m_CameraCurrentYaw;
	float m_FOVX;
	Text2D m_Text2D;

	Loki m_Loki;
	Thor m_Thor;
	Heimdall m_Heimdall;
	Volstagg m_Volstagg;
	//Camera* camera;

	//GLuint program;
	struct
	{
		GLint       mvp_matrix;
		GLint       mv_matrix;
		GLint       proj_matrix;
		GLint       dmap_depth;
		GLint       enable_fog;
	} uniforms;
	int windowWidth;
	int windowHeight;
	float           dmap_depth;
	Shader m_Shader;
	GLuint          tex_displacement;
	GLuint          tex_color;
};

#endif // _FRAMEWORK_HPP_

