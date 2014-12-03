/*-----------------------------------------------------------------------------------
File:			pointSprite.h
Author:			Steve Costa
Description:	This class is similar to point sprites as found in Direct3D.  It is
simply a textured quad which faces the viewer at all times.
-----------------------------------------------------------------------------------*/

#ifndef POINT_SPRITE_H_
#define POINT_SPRITE_H_

/*-----------------------------------------------------------------------------------
Include files
-----------------------------------------------------------------------------------*/

#include "vector.h"
using namespace vec;
#include "matrix.h"
using namespace matrix;

/*-----------------------------------------------------------------------------------
This defines the point sprite attributes and methods.  It will draw a textured
quad which always faces the viewer.
-----------------------------------------------------------------------------------*/

class CPointSprite
{
	// Attributes
private:

	GLuint	m_uiTexture;				// OpenGL texture reference
	GLuint	m_uiSpriteDL;				// Particle dispaly list
	static TMatrix orientation;			// Store orientation of modelview matrix

	// Methods
private:

	//-----------------------------------------------------------
	// Load the bitmap from a file
	//-----------------------------------------------------------
	SDL_Surface *LoadBMP(char *filename)	{
		FILE *file = NULL;
		if (!filename) return NULL;
		fopen_s(&file, filename, "r");

		if (file) {
			fclose(file);
			return SDL_LoadBMP(filename);
		}

		// return null if failed
		return NULL;
	}

	//-----------------------------------------------------------
	// Load texture data into memory
	//-----------------------------------------------------------
	int LoadTextures(char* textureFile) {
		int status = false;

		SDL_Surface *TextureImage[1];
		ZeroMemory(TextureImage, sizeof(void *) * 1);

		// load bitmap
		if (TextureImage[0] = LoadBMP(textureFile))
		{
			status = true;
			glGenTextures(1, &m_uiTexture);

			// generate texture 1
			glBindTexture(GL_TEXTURE_2D, m_uiTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->w,
				TextureImage[0]->h, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->pixels);
		}

		// clean up
		if (TextureImage[0])
		{
			SDL_FreeSurface(TextureImage[0]);
		}

		return status;
	}

	//-----------------------------------------------------------
	// Pre render the quad to save on processing
	//-----------------------------------------------------------
	void PreRender(float width, float height) {
		float xExtent = width * 0.5f;
		float yExtent = height * 0.5f;

		glNewList(m_uiSpriteDL, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, m_uiTexture);
		glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2d(1, 1);
		glVertex3f(xExtent, yExtent, 0);	// Top right
		glTexCoord2d(0, 1);
		glVertex3f(-xExtent, yExtent, 0);	// Top left
		glTexCoord2d(1, 0);
		glVertex3f(xExtent, -yExtent, 0);	// Bottom right
		glTexCoord2d(0, 0);
		glVertex3f(-xExtent, -yExtent, 0);	// Bottom left
		glEnd();
		glEndList();
	}

public:

	//-----------------------------------------------------------
	// Standard constructor
	//-----------------------------------------------------------
	CPointSprite() {}

	//-----------------------------------------------------------
	// Standard destructor
	//-----------------------------------------------------------
	~CPointSprite() {
		glDeleteLists(m_uiSpriteDL, 1);
		glDeleteTextures(1, &m_uiTexture);
	}

	//-----------------------------------------------------------
	// Initialize any member variables
	//-----------------------------------------------------------
	void Init(char* textureFile, float width, float height) {
		// Allocate memory for the display list
		m_uiSpriteDL = glGenLists(1);

		// Load the texture bitmap
		LoadTextures(textureFile);

		// Prerender the textured quad
		PreRender(width, height);
	}

	//-----------------------------------------------------------
	// Call this method once before rendering all the point
	// sprites so that they are all facing the viewer.  This
	// will copy the model view matrix into the matrix object
	// which is used in the renderinf method.
	//-----------------------------------------------------------
	static void GetModelView() {
		// Get the current matrix
		glGetFloatv(GL_MODELVIEW_MATRIX, orientation.m);
	}

	//-----------------------------------------------------------
	// Draw the quad to the screen, the user can treat the quad
	// as if it is a point by passing in the x, y, z coordinates
	// only, and the sprite will be drawn to the screen and
	// always face the viewer.
	// temp represents the desired position of the sprite, it
	// is multiplied by the model view matrix so that the correct
	// position is calculated, the current matrix is then set
	// to the identity so that the particle does not get rotated
	// and it is simply placed in the correct position but
	// still faces the viewer
	//-----------------------------------------------------------
	void Render(float x, float y, float z) {
		TVector temp(x, y, z);
		temp *= orientation;
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(temp.x, temp.y, temp.z);
		glCallList(m_uiSpriteDL);
		glPopMatrix();
	}
};

#endif