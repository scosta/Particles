/*-----------------------------------------------------------------------------------
File:			game.cpp
Authors:		Steve Costa
Description:	Header file defining the main game class.
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
Header files
-----------------------------------------------------------------------------------*/

#include "game.h"							// Class header file
#include "main.h"

/*-----------------------------------------------------------------------------------
Initialize colours
-----------------------------------------------------------------------------------*/

GLfloat CGame::colors[NUM_COLORS][3] =
{
	{ 1.0f, 0.5f, 0.5f }, { 1.0f, 0.75f, 0.5f }, { 1.0f, 1.0f, 0.5f }, { 0.75f, 1.0f, 0.5f },
	{ 0.5f, 1.0f, 0.5f }, { 0.5f, 1.0f, 0.75f }, { 0.5f, 1.0f, 1.0f }, { 0.5f, 0.75f, 1.0f },
	{ 0.5f, 0.5f, 1.0f }, { 0.75f, 0.5f, 1.0f }, { 1.0f, 0.5f, 1.0f }, { 1.0f, 0.5f, 0.75f }
};

/*-----------------------------------------------------------------------------------
Declare static orientation matrix for the point sprite class
-----------------------------------------------------------------------------------*/

TMatrix CPointSprite::orientation;

/*-----------------------------------------------------------------------------------
Initialize particle gravity
-----------------------------------------------------------------------------------*/

float CParticle::m_sfGravity = 9.8f;

/*-----------------------------------------------------------------------------------
Set any initial values for class state variables.
-----------------------------------------------------------------------------------*/

CGame::CGame()
{
	// Set y rotation to 0
	m_RotY = 0.0f;
}

/*-----------------------------------------------------------------------------------
Initialize the class
-----------------------------------------------------------------------------------*/

int CGame::Init()
{
	//----------------------------------------------------------------------
	// Set the viewport to the dimensions of the window
	//----------------------------------------------------------------------
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	//----------------------------------------------------------------------
	// Reset the coordinate system
	//----------------------------------------------------------------------
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//----------------------------------------------------------------------
	// Calculate aspect ration of the window
	//----------------------------------------------------------------------
	gluPerspective(45.0f, (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 200.0f);

	//----------------------------------------------------------------------
	// Change to the model view matrix
	//----------------------------------------------------------------------
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//----------------------------------------------------------------------
	// No need for z-buffering
	//----------------------------------------------------------------------
	glDisable(GL_DEPTH_TEST);

	//----------------------------------------------------------------------
	// Set the clear color
	//----------------------------------------------------------------------
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);

	//----------------------------------------------------------------------
	// Set smooth shading
	//----------------------------------------------------------------------
	glShadeModel(GL_SMOOTH);

	//----------------------------------------------------------------------
	// Enable blending for transparency
	//----------------------------------------------------------------------
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	//----------------------------------------------------------------------
	// Specify how we want perspective correction and point smoothing
	//----------------------------------------------------------------------
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	//----------------------------------------------------------------------
	// Enable texturing
	//----------------------------------------------------------------------
	glEnable(GL_TEXTURE_2D);

	//----------------------------------------------------------------------
	// Start the lights
	//----------------------------------------------------------------------
	SetupLights();

	//----------------------------------------------------------------------
	// Initialize the point sprite object
	//----------------------------------------------------------------------
	m_pointSprite.Init(TEXTURE_FILE, 1.0f, 1.0f);

	//----------------------------------------------------------------------
	// Set up the timing variable
	//----------------------------------------------------------------------
	m_currentTime = GetTickCount();

	return RETURN_SUCCESS;
}

/*-----------------------------------------------------------------------------------
Initialize the light
-----------------------------------------------------------------------------------*/

int CGame::SetupLights()
{
	GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f }; 	// Ambient Light Values
	GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };		// Diffuse Light Values
	GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };	// Light Position

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);

	glEnable(GL_LIGHT1);

	return RETURN_SUCCESS;
}

/*-----------------------------------------------------------------------------------
Handle user input commands which control the camera and exit the program.
-----------------------------------------------------------------------------------*/

void CGame::GetInput()
{
	// Exit program if escape pressed
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
		SendMessage(CWin::m_sWinHandle, WM_CLOSE, 0, 0);
	}
}

/*-----------------------------------------------------------------------------------
Translate and rotate the scene according to the user's preferences.
-----------------------------------------------------------------------------------*/

int CGame::Main()
{
	int i;

	//----------------------------------------------------------------------
	// Keep track of elapsed time since last frame
	//----------------------------------------------------------------------
	m_elapsedTime = GetTickCount() - m_currentTime;
	m_currentTime += m_elapsedTime;
	m_elapsedSecs = m_elapsedTime * 0.001f;

	//----------------------------------------------------------------------
	// Clear the buffer and load identity matrix
	//----------------------------------------------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//----------------------------------------------------------------------
	// Get user input
	//----------------------------------------------------------------------
	GetInput();

	//----------------------------------------------------------------------
	// Change positioning and orientation of camera
	//----------------------------------------------------------------------
	glTranslatef(0.0f, 0.0f, -25.0f);
	glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(m_RotY, 0.0f, 1.0f, 0.0f);
	m_RotY += 0.5f;

	//----------------------------------------------------------------------
	// Update the particles
	//----------------------------------------------------------------------
	for (i = 0; i < NUM_PARTICLES; i++)
	{
		// Restart any particles that have died
		if (!m_particles[i].IsAlive()) {
			m_particles[i].ReStart();

			// Set position to origin
			m_particles[i].m_fPosX = 0.0f;
			m_particles[i].m_fPosY = 0.0f;
			m_particles[i].m_fPosZ = 0.0f;

			// Set a random velocity
			m_particles[i].m_fVelX = 1.0f + float((rand() % 5) - 2.5f);
			m_particles[i].m_fVelY = 1.0f + float((rand() % 15));
			m_particles[i].m_fVelZ = float((rand() % 5) - 2.5f);

			// Set the acceleration to 0
			m_particles[i].m_fAccelX = 0.0f;
			m_particles[i].m_fAccelY = 0.0f;
			m_particles[i].m_fAccelZ = 0.0f;

			// Set a colour
			int randCol = rand() % NUM_COLORS;
			m_particles[i].m_fColR = colors[randCol][0];
			m_particles[i].m_fColG = colors[randCol][1];
			m_particles[i].m_fColB = colors[randCol][2];
		}

		// Update the particle positions
		m_particles[i].Update(m_elapsedSecs);
	}

	//----------------------------------------------------------------------
	// Draw the particles
	//----------------------------------------------------------------------
	m_pointSprite.GetModelView();
	for (i = 0; i < NUM_PARTICLES; i++)
	{
		glColor4f(m_particles[i].m_fColR, m_particles[i].m_fColG,
			m_particles[i].m_fColB, m_particles[i].GetLifeValue());
		m_pointSprite.Render(m_particles[i].m_fPosX,
			m_particles[i].m_fPosY,
			m_particles[i].m_fPosZ);
	}

	//----------------------------------------------------------------------
	// Ensure that we keep a constant frame rate
	//----------------------------------------------------------------------
	m_timeLeft = GetTickCount() - m_currentTime;
	m_timeLeft = (m_timeLeft > FRAME_INTERVAL) ? 0 : FRAME_INTERVAL - m_timeLeft;
	Sleep(m_timeLeft);

	return 0;
}

/*-----------------------------------------------------------------------------------
When shutting down the program clear the dynamic arrays
-----------------------------------------------------------------------------------*/

int CGame::Shutdown()
{
	return 0;
}
