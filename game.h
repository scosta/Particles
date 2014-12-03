/*-----------------------------------------------------------------------------------
File:			game.h
Author:		    Steve Costa
Description:	Header file defining the main game class.
-----------------------------------------------------------------------------------*/

#ifndef GAME_H_
#define GAME_H_

/*-----------------------------------------------------------------------------------
Header files
-----------------------------------------------------------------------------------*/

#include "commonUtil.h"						// Common Macros, and headers
#include "pointSprite.h"					// Point sprite object
#include "particle.h"						// Particle object

/*-----------------------------------------------------------------------------------
Constants
-----------------------------------------------------------------------------------*/

#define	TEXTURE_FILE		"particle.bmp"
#define	NUM_PARTICLES		300
#define NUM_COLORS			12

/*-----------------------------------------------------------------------------------
Game class definition
-----------------------------------------------------------------------------------*/

class CGame
{
	// Attributes
private:

	CPointSprite m_pointSprite;				// Point sprite to draw particles
	CParticle m_particles[NUM_PARTICLES];	// Array of particles
	static GLfloat colors[NUM_COLORS][3];	// Colours to use in game

	float m_RotY;							// Scene rotation

	DWORD	m_currentTime,
		m_timeLeft,
		m_elapsedTime;					// Timing variables
	float	m_elapsedSecs;					// Elapsed time in seconds

	// Methods
private:

	void GetInput();							// Get user input
	int SetupLights();							// Enable the OpenGL lights

public:

	CGame();
	int Init();
	int Main();
	int Shutdown();
};


#endif GAME_H