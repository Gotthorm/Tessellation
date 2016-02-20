// CAMERAMANAGER.CPP

#include <windows.h>
#include "CameraManager.h"
#include "FollowCamera.h"

CameraManager::CameraManager() : m_CurrentCamera(NULL), m_CurrentCameraIndex(0)
{
}

CameraManager::~CameraManager()
{
	for( std::vector<std::pair<Camera*, Mode>>::iterator it = m_Cameras.begin(); it != m_Cameras.end(); ++it )
	{
		delete (*it).first;
	}
	m_Cameras.clear();
}

void CameraManager::Update( DWORD timeElapsed )
{
	// Will I update all cameras or just the current camera?

	if( m_CurrentCameraIndex > 0 && m_CurrentCameraIndex <= m_Cameras.size() )
	{
		Mode mode = m_Cameras[ m_CurrentCameraIndex - 1 ].second;

		if( mode == FOLLOW )
		{
			FollowCamera* camera = static_cast<FollowCamera*>( m_Cameras[ m_CurrentCameraIndex - 1 ].first );

			camera->Update( timeElapsed );
		}
		else if( mode == ORBIT )
		{

		}
	}
}

CameraManager::CameraId CameraManager::AddCamera( Mode mode, const std::string& name )
{
	Camera* newCamera(NULL);

	switch( mode )
	{
	case FIXED:
		newCamera = new Camera(name);
		break;
	case FOLLOW:
		newCamera = new FollowCamera( name, NULL );
		break;
	case ORBIT:
		newCamera = new Camera( name );
		break;
	case FREE:
		newCamera = new Camera( name );
		break;
	case NONE:
	default:
		// Do nothing
		return 0;
	}

	m_Cameras.push_back(std::make_pair(newCamera, mode));

	unsigned int CameraId = m_Cameras.size();

	if( m_CurrentCameraIndex == 0 )
	{
		m_CurrentCameraIndex = CameraId;
	}

	return CameraId;
}

bool CameraManager::SetCameraTarget( CameraId cameraId, Object* target )
{
	if( cameraId > 0 && cameraId <= m_Cameras.size() )
	{
		Mode mode = m_Cameras[cameraId - 1].second;

		if( mode == FOLLOW )
		{
			FollowCamera* camera = static_cast<FollowCamera*>(m_Cameras[ cameraId - 1 ].first);

			camera->SetTarget(target);

			return true;
		}
		else if( mode == ORBIT )
		{

		}
	}

	return false;
}

bool CameraManager::SetCameraPosition( CameraId cameraId, const glm::vec3& position )
{
	if( cameraId > 0 && cameraId <= m_Cameras.size() )
	{
		Camera* camera = m_Cameras[ cameraId - 1 ].first;

		camera->SetPosition( position );

		return true;
	}

	return false;
}

bool CameraManager::SetCameraOffset( CameraId cameraId, const glm::vec3& position )
{
	if( cameraId > 0 && cameraId <= m_Cameras.size() )
	{
		Mode mode = m_Cameras[ cameraId - 1 ].second;

		if( mode == FOLLOW )
		{
			FollowCamera* camera = static_cast<FollowCamera*>( m_Cameras[ cameraId - 1 ].first );

			camera->SetOffset( glm::vec2( position ) );

			return true;
		}
		else if( mode == ORBIT )
		{

		}
	}

	return false;
}

bool CameraManager::SetCurrentCamera( CameraId cameraId )
{ 
	if( cameraId > 0 && cameraId <= m_Cameras.size() )
	{
		m_CurrentCameraIndex = cameraId;

		return true;
	}

	return false;
}
