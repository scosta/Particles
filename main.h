/*-----------------------------------------------------------------------------------
File:			main.h
Author:			Steve Costa
Description:	Define the windows class and the necessary constants
-----------------------------------------------------------------------------------*/

#ifndef MAIN_H_
#define MAIN_H_

/*-----------------------------------------------------------------------------------
Header files
-----------------------------------------------------------------------------------*/

// Include the following in order to detect memory leaks
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "commonUtil.h"					// Common Macros, and headers
#include "game.h"						// Game header file

/*-----------------------------------------------------------------------------------
Constants
-----------------------------------------------------------------------------------*/

#define WINDOW_CLASS_NAME		"Particles"		// Class name

/*-----------------------------------------------------------------------------------
Define the main window class for the game.
-----------------------------------------------------------------------------------*/

class CWin
{
	// ATTRIBUTES
private:
	WNDCLASSEX						m_winClass;			// Window class
	DEVMODE							m_screenSettings;	// Screen settings
public:
	static HWND						m_sWinHandle;			// Window handle
	static HINSTANCE				m_sMainInstance;		// Program instance
	static HGLRC					m_sRenderContext;		// Rendering context
	static HDC						m_sDeviceContext;		// GDI device context
	static PIXELFORMATDESCRIPTOR	m_sPfd;					// Format desired for pixels
	
	// METHODS
public:
	int Init(WNDPROC windowProc, HINSTANCE hInstance);		// Class constructor
	int Shutdown();											// Class destructor
};


#endif