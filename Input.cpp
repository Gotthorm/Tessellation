// INPUT.CPP

#include <assert.h>
#include <windows.h>
#include <stdio.h>
#include "Input.h"
#include "TCHAR.H"
#include <map>
#include <string>
//#include "Logger.h"

//std::map<string, bool> myKey;
//bool Input::GetRawKeyboardData(LPARAM lParam)
//{
//	char buffer[sizeof(RAWINPUT)];
//	UINT size = sizeof(RAWINPUT);
//
//	GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, buffer, &size, sizeof(RAWINPUTHEADER));
//
//	RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(buffer);
//	if(raw->header.dwType == RIM_TYPEKEYBOARD)
//	{
//		const RAWKEYBOARD& rawKeyboard = raw->data.keyboard;
//
//		unsigned int scanCode = rawKeyboard.MakeCode;
//		unsigned int flags = rawKeyboard.Flags;
//		const bool E0 = ((flags & RI_KEY_E0) != 0);
//		const bool E1 = ((flags & RI_KEY_E1) != 0);
//		const bool KeyDown = !((flags & RI_KEY_BREAK) != 0);
//
//		UINT key = (scanCode << 16) | (E0 << 24);
//		char stringBuffer[32];
//		GetKeyNameText((LONG)key, stringBuffer, 32);
//		if(KeyDown) // Press
//		{
//			myKey[stringBuffer] = true;
//		}
//		else // Release
//		{
//			myKey[stringBuffer] = false;
//		}
//	}
//	return true;
//}

/*
bool KEYBOARD_INPUT::KeyPressed(string key, int mode)
{
	if(mode == ONLYONCE)
	{
		if(myKey[key] && pressed_onlyonce[key] == false)
		{
			pressed_onlyonce[key] = true;
			return true;
		}
		if(!myKey[key])
		{
			pressed_onlyonce[key] = false;
			return false;
		}
	}

	else if(mode == CONTINUOUS)
	{
		if(myKey[key] == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	else if(mode == TOGGLE)
	{
		if(myKey[key] && pressed_toggle[key] == false)
		{
			pressed_toggle[key] = true;
			released_toggle[key] = !released_toggle[key];
			return released_toggle[key];
		}
		else if(!myKey[key])
		{
			pressed_toggle[key] = false;
			return released_toggle[key];
		}
	}
	return false;
}
*/

Input::Input()
{
}

Input::~Input()
{
}

bool Input::Init()
{
	ZeroMemory(&m_OldKeyStateBuffer, sizeof(m_OldKeyStateBuffer));
	ZeroMemory(&m_NewKeyStateBuffer, sizeof(m_NewKeyStateBuffer));
	ZeroMemory(&m_MouseMovementDeltas, sizeof(m_MouseMovementDeltas));

/*
	RAWINPUTDEVICE Rid[2];

	Rid[0].usUsagePage = 0x01;
	Rid[0].usUsage = 0x02;
	Rid[0].dwFlags = RIDEV_NOLEGACY;   // adds HID mouse and also ignores legacy mouse messages
	Rid[0].hwndTarget = 0;

	Rid[1].usUsagePage = 0x01;
	Rid[1].usUsage = 0x06;
	Rid[1].dwFlags = RIDEV_NOLEGACY;   // adds HID keyboard and also ignores legacy keyboard messages
	Rid[1].hwndTarget = 0;

	if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0])) == FALSE) 
	{
		//registration failed. Call GetLastError for the cause of the error
		return false;
	}
*/

	RAWINPUTDEVICE Rid[2];

	Rid[0].usUsagePage = 0x01;
	Rid[0].usUsage = 0x02;		// Adds HID mouse
	Rid[0].dwFlags = 0;			// Change to RIDEV_NOLEGACY to ignore legacy keyboard messages
	Rid[0].hwndTarget = 0;

	Rid[1].usUsagePage = 0x01;
	Rid[1].usUsage = 0x06;		// Adds HID keyboard
	Rid[1].dwFlags = 0;			// Change to RIDEV_NOLEGACY to ignore legacy keyboard messages
	Rid[1].hwndTarget = 0;

	if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0])) == FALSE)
	{
		//registration failed. Call GetLastError for the cause of the error
		return false;
	}

	return true;
}

void Input::Shutdown()
{
}

// This gets called everytime our winmain loop receives a WM_INPUT message
// It simply caches all of the input for the next game loop update.
void Input::Update(LPARAM lParam)
{
	UINT dwSize;

	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
	LPBYTE lpb = new BYTE[dwSize];
	if (lpb == NULL)
	{
		// TODO: Log a console error
		return;
	}

	if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
	{
		//Logger::GetInstance()->LogError(std::wstring(L"GetRawInputData does not return correct size!"));
	}

	RAWINPUT* raw = (RAWINPUT*)lpb;

	if (raw->header.dwType == RIM_TYPEKEYBOARD)
	{
		m_NewKeyStateBuffer[raw->data.keyboard.VKey] = (raw->data.keyboard.Flags & RI_KEY_BREAK) ? 0 : 1;
		//TCHAR stringBuffer[256];
		//_stprintf_s(stringBuffer, 256, _T(" Kbd: make=%04x Flags:%04x Reserved:%04x ExtraInformation:%08x, msg=%04x VK=%04x \n"),
		//	raw->data.keyboard.MakeCode,
		//	raw->data.keyboard.Flags,
		//	raw->data.keyboard.Reserved,
		//	raw->data.keyboard.ExtraInformation,
		//	raw->data.keyboard.Message,
		//	raw->data.keyboard.VKey);

		//OutputDebugString(stringBuffer);
	}
	else if (raw->header.dwType == RIM_TYPEMOUSE)
	{
		if (raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)
		{
			m_NewKeyStateBuffer[KEY_MOUSE_RIGHT] = 1;
		}
		else if (raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP)
		{
			m_NewKeyStateBuffer[KEY_MOUSE_RIGHT] = 0;
		}

		// Ensure that the last X & Y are deltas and not absolute
		if ((raw->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE) == 0)
		{
			m_MouseMovementDeltas[0] += raw->data.mouse.lLastX;
			m_MouseMovementDeltas[1] += raw->data.mouse.lLastY;
		}

		//TCHAR stringBuffer[256];
		//_stprintf_s(stringBuffer, 256, _T("Mouse: usFlags=%04x ulButtons=%04x usButtonFlags=%04x usButtonData=%04x ulRawButtons=%04x lLastX=%04x lLastY=%04x ulExtraInformation=%04x\r\n"),
		//	raw->data.mouse.usFlags,
		//	raw->data.mouse.ulButtons,
		//	raw->data.mouse.usButtonFlags,
		//	raw->data.mouse.usButtonData,
		//	raw->data.mouse.ulRawButtons,
		//	raw->data.mouse.lLastX,
		//	raw->data.mouse.lLastY,
		//	raw->data.mouse.ulExtraInformation);

		//OutputDebugString(stringBuffer);
	}

	delete[] lpb;
}

void Input::AdvanceFrame()
{
	memcpy(m_OldKeyStateBuffer, m_NewKeyStateBuffer, sizeof(m_NewKeyStateBuffer));
	ZeroMemory(&m_MouseMovementDeltas, sizeof(m_MouseMovementDeltas));
}

bool Input::GetKeyUp(KeyCode key)
{
	return (m_OldKeyStateBuffer[key] != 0 && m_NewKeyStateBuffer[key] == 0);
}

bool Input::GetKeyDown(KeyCode key)
{
	return (m_OldKeyStateBuffer[key] == 0 && m_NewKeyStateBuffer[key] != 0);
}

bool Input::GetKey(KeyCode key)
{
	return (m_NewKeyStateBuffer[key] != 0);
}

//void Input::ClearKeyboard()
//{
//	ZeroMemory( &m_KeyStateBuffer, sizeof( m_KeyStateBuffer ) );
//}
//
//void Input::ClearMouse()
//{
//	ZeroMemory( &m_MouseState, sizeof( m_MouseState ) );
//}

bool Input::GetMouse( long& deltaX, long& deltaY )
{
	deltaX = m_MouseMovementDeltas[0];
	deltaY = m_MouseMovementDeltas[1];

	return true;
}

//void Input::EnableMouseCapture( bool bEnable )
//{
//	if( bEnable == false )
//	{
//		if( m_bMouseCaptureEnabled && m_pDirectInputMouse != NULL)
//		{
//			// Unacquire mouse
//			m_pDirectInputMouse->Unacquire();
//		}
//	}
//
//	m_bMouseCaptureEnabled = bEnable;
//}
