/**
 CPlayer2D
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

// Include Singleton template
#include "DesignPatterns\SingletonTemplate.h"
// Include shader
#include "RenderControl\shader.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include the Map2D as we will use it to check the player's movements and actions
class CMap2D;

// Include Keyboard controller
#include "Inputs\KeyboardController.h"

// Include Settings
#include "GameControl\Settings.h"

// Include Physics2D
#include "Physics2D.h"

// Include AnimatedSprites
#include "AnimatedSprites.h"

// Include SoundController
#include "..\SoundController\SoundController.h"

class CPlayer2D : public CSingletonTemplate<CPlayer2D>
{
	friend CSingletonTemplate<CPlayer2D>;
public:

	// Init
	bool Init(void);

	// Update
	void Update(const double dElapsedTime);

	// PreRender
	void PreRender(void);

	// Render
	void Render(void);

	// PostRender
	void PostRender(void);

	// Set a shader to this class instance
	void SetShader(Shader* cShader);

	// Set the indices of the player
	void Seti32vec2Index(const int iIndex_XAxis, const int iIndex_YAxis);

	// Set the number of microsteps of the player
	void Seti32vec2NumMicroSteps(const int iNumMicroSteps_XAxis, const int iNumMicroSteps_YAxis);

	// Set the UV coordinates of the player
	void Setvec2UVCoordinates(const float fUVCoordinate_XAxis, const float fUVCoordinate_YAxis);

	// Get the indices of the player
	glm::i32vec2 Geti32vec2Index(void) const;

	// Get the number of microsteps of the player
	glm::i32vec2 Geti32vec2NumMicroSteps(void) const;

	// Set the UV coordinates of the player
	glm::vec2 Getvec2UVCoordinates(void) const;

protected:
	enum DIRECTION
	{
		LEFT = 0,
		RIGHT = 1,
		UP = 2,
		DOWN = 3,
		NUM_DIRECTIONS
	};

	// Handler to the Shader Program instance
	Shader* cShader;

	// OpenGL objects
	GLuint VBO, VAO, EBO;

	// The texture ID in OpenGL
	GLuint iTextureID;

	// Handler to the CMap2D instance
	CMap2D* cMap2D;

	// A transformation matrix for controlling where to render the entities
	glm::mat4 transform;

	// The i32vec2 variable which stores the indices of the enemy2D in the Map2D
	glm::i32vec2 i32vec2Index;

	// The i32vec2 variable which stores The number of microsteps from the tile indices for the player. 
	// A tile's width or height is in multiples of these microsteps
	glm::i32vec2 i32vec2NumMicroSteps;

	// The vec2 variable which stores the UV coordinates to render the player
	glm::vec2 vec2UVCoordinate;

	// Keyboard Controller singleton instance
	CKeyboardController* cKeyboardController;

	// Settings
	CSettings* cSettings;

	// Physics
	CPhysics2D cPhysics2D;

	// AnimatedSprites
	CAnimatedSprites cAnimatedSprites;

	// Handler to the CSoundController
	CSoundController* cSoundController;

	// Constructor
	CPlayer2D(void);

	// Destructor
	virtual ~CPlayer2D(void);

	// Load a texture
	bool LoadTexture(const char* filename, GLuint& iTextureID);

	// Constraint the player's position within a boundary
	void Constraint(DIRECTION eDirection = LEFT);

	// Find a feasible position for the player's current position
	void FindFeasiblePosition(const int iOldIndex, DIRECTION eDirection = LEFT);

	// Check if the player is in mid-air
	bool IsMidAir(void);

	// Update Jump or Fall
	void UpdateJumpFall(const double dElapsedTime);

	// Let player interact with the map
	void InteractWithMap(void);
};

