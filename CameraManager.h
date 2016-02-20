// CAMERAMANAGER.H

#ifndef __CAMERAMANAGER_H__
#define __CAMERAMANAGER_H__

#include "Camera.h"
#include <vector>

class CameraManager
{
public:
	enum Mode
	{
		NONE,
		FIXED,
		FOLLOW,
		ORBIT,
		FREE,
	};

	typedef unsigned int CameraId;

	CameraManager();
	virtual ~CameraManager();

	void Update( DWORD timeElapsed );

	// Create a new camera and add to the list
	// Returns an unique identifier ID for the camera
	// Returns 0 if call fails
	CameraId CameraManager::AddCamera( Mode mode, const std::string& name );

	// No assumptions made on the given camera index, GetCamera will handle any incorrect input
	const Camera* GetCurrentCamera() { return GetCamera( m_CurrentCameraIndex ); }

	bool SetCurrentCamera( CameraId cameraId );

	// No assumptions made on the given camera index
	// If the given camera index is valid, return the corresponding camera
	// Otherwise return NULL
	const Camera* GetCamera( CameraId cameraIndex ) { return ( cameraIndex > 0 && cameraIndex <= m_Cameras.size() ) ? m_Cameras[ cameraIndex - 1 ].first : NULL; }

	// If a current camera exists, return the mode
	// Assumption m_CurrentCameraIndex <= m_Cameras.size()
	//Mode GetCurrentMode() { return ( m_CurrentCameraIndex > 0 ) ? m_Cameras[ m_CurrentCameraIndex - 1 ].second : NONE; }

	bool SetCameraTarget( CameraId cameraId, Object* target );

	bool SetCameraPosition( CameraId cameraId, const glm::vec3& position );

	bool SetCameraOffset( CameraId cameraId, const glm::vec3& position );

private:
	Camera* m_CurrentCamera;
	std::vector<std::pair<Camera*, Mode>> m_Cameras;
	CameraId m_CurrentCameraIndex;
};

#endif /* __CAMERAMANAGER_H__ */

