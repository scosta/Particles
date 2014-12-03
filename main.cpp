/*-----------------------------------------------------------------------------------
File:			main.cpp
Author:			Steve Costa
Description:	This is a generic windows shell allowing for OpenGL programming
in full-screen mode.
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
Header files
-----------------------------------------------------------------------------------*/

#include "main.h"							// Header file for this class
#include "game.h"

/*-----------------------------------------------------------------------------------
Declare static variables of the CWindow class.
-----------------------------------------------------------------------------------*/

HINSTANCE				CWin::m_sMainInstance;			// Program instance
HWND					CWin::m_sWinHandle = NULL;		// Window handle
HGLRC					CWin::m_sRenderContext = NULL;	// Rendering context
HDC						CWin::m_sDeviceContext = NULL;	// GDI device context
PIXELFORMATDESCRIPTOR	CWin::m_sPfd;					// Format desired for pixels

/*-----------------------------------------------------------------------------------
Window class method which initialises the window and the OpenGL graphics
rendering context.
Return values:		0 = Success
1 = Failed to register class
2 = Failed to create window
3 = Failed to change display settings
4 = Failed to get device context
5 = Failed to get compatible pixel format
6 = Failed to set chosen pixel format
7 = Failed to get a rendering context
8 = Failed to activate the rendering context
-----------------------------------------------------------------------------------*/

int CWin::Init(WNDPROC windowProc, HINSTANCE hInstance)
{
	GLuint	pixelFormat;								// Pixel format id

	// Assign the instance variable to our static variable
	m_sMainInstance = hInstance;

	// Fill the class structure
	m_winClass.cbSize = sizeof(WNDCLASSEX);
	m_winClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	m_winClass.lpfnWndProc = windowProc;
	m_winClass.cbClsExtra = 0;
	m_winClass.cbWndExtra = 0;
	m_winClass.hInstance = m_sMainInstance;
	m_winClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	m_winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_winClass.hbrBackground = NULL;
	m_winClass.lpszMenuName = NULL;
	m_winClass.lpszClassName = WINDOW_CLASS_NAME;
	m_winClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	// Register the window class
	if (!RegisterClassEx(&m_winClass))
		return 1;

	// Set up the OpenGL context
	ZeroMemory(&m_screenSettings, sizeof(DEVMODE));			// Clear device memory
	m_screenSettings.dmSize = sizeof(DEVMODE);				// Set size of structure
	m_screenSettings.dmPelsWidth = SCREEN_WIDTH;			// Set screen width
	m_screenSettings.dmPelsHeight = SCREEN_HEIGHT;			// Set screen hight
	m_screenSettings.dmBitsPerPel = SCREEN_BPP;				// Set bits per pixel
	m_screenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	// Create the window
	if (!(m_sWinHandle = CreateWindowEx(WS_EX_APPWINDOW, WINDOW_CLASS_NAME, "Particle Animation",
		WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, m_sMainInstance, NULL)))
		return 2;

	// Change the display settings
	if (ChangeDisplaySettings(&m_screenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		return 3;

	// Get the device context
	if (!(m_sDeviceContext = GetDC(m_sWinHandle)))
		return 4;

	// Set up the descriptor according to how we want the pixels
	ZeroMemory(&m_sPfd, sizeof(PIXELFORMATDESCRIPTOR));			// Clear descriptor memory
	m_sPfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);				// Set size
	m_sPfd.nVersion = 1;										// Version num
	m_sPfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL
		| PFD_DOUBLEBUFFER;
	m_sPfd.iPixelType = PFD_TYPE_RGBA;							// RGBA setting
	m_sPfd.cColorBits = SCREEN_BPP;								// Set the bit depth
	m_sPfd.cDepthBits = 16;										// z-buffer	
	m_sPfd.iLayerType = PFD_MAIN_PLANE;							// Main canvas

	// Query for closest matching pixel format
	if (!(pixelFormat = ChoosePixelFormat(m_sDeviceContext, &m_sPfd)))
		return 5;

	// Set the pixel format
	if (!(SetPixelFormat(m_sDeviceContext, pixelFormat, &m_sPfd)))
		return 6;

	// Now we can get a rendering context from our device context
	if (!(m_sRenderContext = wglCreateContext(m_sDeviceContext)))
		return 7;

	// Activate the rendering context
	if (!(wglMakeCurrent(m_sDeviceContext, m_sRenderContext)))
		return 8;

	return 0;
} // End CWin::CWin

/*-----------------------------------------------------------------------------------
Shutdown method which frees all resources.
-----------------------------------------------------------------------------------*/

int CWin::Shutdown()
{
	// Change back to original display settings
	ChangeDisplaySettings(NULL, 0);

	// Release the rendering context
	if (!(wglMakeCurrent(NULL, NULL)))
		MessageBox(NULL, "Failed to release rendering context", "ERROR", MB_OK | MB_ICONINFORMATION);

	// Free the memory allocated
	if (!(wglDeleteContext(m_sRenderContext)))
		MessageBox(NULL, "Failed to delete rendering context", "ERROR", MB_OK | MB_ICONINFORMATION);
	m_sRenderContext = NULL;

	// Unregister this window class
	if (!(UnregisterClass(WINDOW_CLASS_NAME, m_sMainInstance)))
		MessageBox(NULL, "Failed to unregister the windows class", "ERROR", MB_OK | MB_ICONINFORMATION);
	m_sMainInstance = NULL;

	return 0;
} // End CWin::~CWin

/*-----------------------------------------------------------------------------------
Windows message handling procedure
-----------------------------------------------------------------------------------*/

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// Proceess messages
	switch (msg)
	{
	case WM_SETCURSOR:					// Invisible mouse cursor
		SetCursor(NULL);
		return 0;
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
} // End WindowProc

/*-----------------------------------------------------------------------------------
Program entry point
-----------------------------------------------------------------------------------*/

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance,
	LPSTR lpcmdline, int ncmdshow)
{
	MSG			msg;
	CWin		*p_window;					// Window object pointer
	CGame		*p_game;					// Game object pointer

	// Detect memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	p_window = new CWin();					// Allocate memory for new window
	p_window->Init(WindowProc, hinstance);	// Initialise window

	p_game = new CGame();					// Allocate memory for game object
	p_game->Init();							// Initialise game

	// Program loop
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0))
				break;
			DispatchMessage(&msg);
		}
		else
		{
			p_game->Main();
			SwapBuffers(CWin::m_sDeviceContext);
		}
	}

	// Clean up when done
	p_game->Shutdown();
	delete(p_game);
	p_game = NULL;

	p_window->Shutdown();
	delete(p_window);
	p_window = NULL;

	return 0;
} // End WinMain