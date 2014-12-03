/*-----------------------------------------------------------------------------------
File:			particle.h
Author:			Steve Costa
Description:	This class contains the attributes and methods required for a
particle.  Creating an array of these particles and animating
them will result in particle effects.  Some static variables
exist so that all the particles undergo the same physical effects
such as gravity.
-----------------------------------------------------------------------------------*/

#ifndef PARTICLE_H_
#define PARTICLE_H_

/*-----------------------------------------------------------------------------------
Define particle attributes and methods
-----------------------------------------------------------------------------------*/

class CParticle
{
	// Attributes
private:
	float m_fLife;							// Life span of particle
	float m_fFadeRate;						// How fast particle fades out

public:
	static float m_sfGravity;				// Acceleration due to gravity

	float m_fColR, m_fColG, m_fColB;		// Colour variables
	float m_fPosX, m_fPosY, m_fPosZ;		// Position variables
	float m_fVelX, m_fVelY, m_fVelZ;		// Velocity variables
	float m_fAccelX, m_fAccelY, m_fAccelZ;	// Acceleration variables

	// Methods
public:

	//-----------------------------------------------------------
	// Standard constructor
	//-----------------------------------------------------------
	CParticle() {
		m_fLife = 0.0f;
		m_fFadeRate = 0.0f;
		m_fColR = 0.0f; m_fColG = 0.0f; m_fColB = 0.0f;
		m_fPosX = 0.0f; m_fPosY = 0.0f; m_fPosZ = 0.0f;
		m_fVelX = 0.0f; m_fVelY = 0.0f; m_fVelZ = 0.0f;
		m_fAccelX = 0.0f; m_fAccelY = 0.0f; m_fAccelZ = 0.0f;
	}

	//-----------------------------------------------------------
	// Standard destructor
	//-----------------------------------------------------------
	~CParticle() { }

	//-----------------------------------------------------------
	// Set the particle to alive and set its fading rate
	//-----------------------------------------------------------
	void ReStart() {
		// Life is set to 1 so that it can be used as the alpha
		// value for the colour, when it dies it will fade out
		m_fLife = 1.0f;
		m_fFadeRate = float(rand() % 100) * 0.001f + 0.003f;
	}

	//-----------------------------------------------------------
	// Translate the particle according to its physical state
	// variables and the amount of time that has elapsed.
	//-----------------------------------------------------------
	void Update(float dt) {
		// Update the velocity vector
		m_fVelX += dt * m_fAccelX;
		m_fVelY += dt * (m_fAccelY - m_sfGravity);
		m_fVelZ += dt * m_fAccelZ;

		// Update the positon vector
		m_fPosX += dt * m_fVelX;
		m_fPosY += dt * m_fVelY;
		m_fPosZ += dt * m_fVelZ;

		// Simmulate a floor by making the particles bounce
		// when they reach 0 along the y-axis
		if (m_fPosY < 0.0f)
			m_fVelY *= -0.75f;

		// Particle fades
		m_fLife -= m_fFadeRate;
	}

	//-----------------------------------------------------------
	// Check to see if particle is still alive
	//-----------------------------------------------------------
	bool IsAlive() {
		return (m_fLife > 0.0f);
	}

	//-----------------------------------------------------------
	// Return the life value of particle (used for alpha value)
	//-----------------------------------------------------------
	float GetLifeValue() {
		return m_fLife;
	}
};

#endif