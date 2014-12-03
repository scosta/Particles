/*-----------------------------------------------------------------------------------
File:			commonUtil.h
Author:			Steve Costa
Description:	File is used as a source of common libraries, constants, and macros
used throughout the whole program.
-----------------------------------------------------------------------------------*/

#ifndef COMMON_UTIL_H
#define COMMON_UTIL_H

/*-----------------------------------------------------------------------------------
External Libraries
-----------------------------------------------------------------------------------*/

#include <windows.h>
#include <cstdio>
#include <gl\gl.h>							// OpenGL Library files
#include <gl\glu.h>
#include <SDL.h>


/*-----------------------------------------------------------------------------------
Constants
-----------------------------------------------------------------------------------*/

#define SCREEN_WIDTH			1024					// Horizontal resolution
#define SCREEN_HEIGHT			768						// Vertical resolution
#define SCREEN_BPP				32						// Color depth
#define	FRAME_INTERVAL			20						// 50 FPS

#define ONE_EIGTH_WIDTH			(SCREEN_WIDTH >> 3)
#define ONE_EIGTH_HEIGHT		(SCREEN_HEIGHT >> 3)

/*-----------------------------------------------------------------------------------
Return values
-----------------------------------------------------------------------------------*/

#define	RETURN_SUCCESS			1
#define	RETURN_FAILURE			-1

/*-----------------------------------------------------------------------------------
Macros
-----------------------------------------------------------------------------------*/

// PI
#define PI		3.14159265f
#define PI2		6.28318531f

// Get 3 dimensional index array
#ifndef GET3DINDEX
#define GET3DINDEX(i, j, k, sizeI, sizeJ) (((((i) * (sizeI)) + (j)) * (sizeJ)) + (k))
#endif

// Square macro
#ifndef SQR
#define SQR(a)	((a) * (a))
#endif

// Cube macro
#ifndef CUBE
#define CUBE(a)	((a) * (a) * (a))
#endif

// Min macro
#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif 

// Max macro
#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif 

// Swap macro
#ifndef SWAP
#define SWAP(a, b, t) { t = a; a = b; b = t; }
#endif

// Absolute macro
#ifndef ABS
#define ABS(a) ((a) < 0 ? -(a) : (a))
#endif

#endif