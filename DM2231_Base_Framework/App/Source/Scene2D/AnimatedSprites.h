/**
 CAnimatedSprites
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

// Include GLEW
#ifndef GLEW_STATIC
	#define GLEW_STATIC
	#include <GL/glew.h>
#endif

// Include map storage
#include <map>
using namespace std;

class CAnimatedSprites
{
public:
	enum DIRECTION
	{
		NEUTRAL = 0,
		LEFT = 1,
		RIGHT = 2,
		UP = 3,
		DOWN = 4,
		NUM_DIRECTIONS
	};

	// Constructor
	CAnimatedSprites(void);

	// Destructor
	virtual ~CAnimatedSprites(void);

	// Init
	virtual bool Init(void);

	// Add a Texture ID to this
	virtual void Add(const DIRECTION sDIRECTION, const GLuint iTextureID);

	// Remove a Texture ID from this
	virtual void Remove(const DIRECTION sDIRECTION, const GLuint iTextureID);

	// Get ID
	virtual GLuint Get(const DIRECTION sDIRECTION);

protected:
	// The texture ID for neutral
	GLuint iTextureID_NEUTRAL;

	// Map containing texture IDs for rendering LEFT animation sprites
	map<int, int> MapOfTextureIDs_LEFT;
	// Map containing texture IDs for rendering RIGHT animation sprites
	map<int, int> MapOfTextureIDs_RIGHT;
	// Map containing texture IDs for rendering UP animation sprites
	map<int, int> MapOfTextureIDs_UP;
	// Map containing texture IDs for rendering DOWN animation sprites
	map<int, int> MapOfTextureIDs_DOWN;

	// Current index for each map
	int iIndex_LEFT;
	int iIndex_RIGHT;
	int iIndex_UP;
	int iIndex_DOWN;
};
