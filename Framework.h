// FRAMEWORK.HPP

#ifndef _FRAMEWORK_HPP_
#define _FRAMEWORK_HPP_

#include <windows.h>
#include "Landscape.h"
#include "Loki.h"
#include <gl\GL.h>
#include "Text2D.h"

class Input;
class FollowCamera;
class CameraManager;

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
	void UpdateCamera(DWORD timeElapsed, const glm::mat4& avatarOrientation );
	void UpdateAvatar( DWORD timeElapsed, glm::mat4& avatarOrientation );

	Input* m_pInput;
	DWORD m_OldFrameTime;
	DWORD m_CurrentFPS;
	DWORD m_OneSecondIntervalAccumulator;
	DWORD m_UpdateAccumulator;
	HDC m_WindowHandleToDeviceContext;

	glm::vec3 m_CameraPosition;
	glm::vec3 m_CameraOrientation;
	glm::vec3 m_PlayerPosition;
	glm::vec4 m_PlayerOrientation;
	float m_PlayerRotationDegrees;
	float m_CameraTargetPitch;
	float m_CameraTargetYaw;
	float m_CameraCurrentPitch;
	float m_CameraCurrentYaw;
	float m_FOVX;
	Text2D m_Text2D;

	Landscape m_Landscape;
	Loki m_Loki;

	struct
	{
		GLint       mvp_matrix;
		GLint       mv_matrix;
		GLint       proj_matrix;
		//GLint       dmap_depth;
		//GLint       enable_fog;
	} uniforms;

	int windowWidth;
	int windowHeight;
	float dmap_depth;
	Shader m_Shader;
	//GLuint tex_displacement;
	GLuint tex_color;

	CameraManager* m_CameraManager;
};

#endif // _FRAMEWORK_HPP_

