/**
 CEnemy2D
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

#include "Entity2D.h"

// Include shader
#include "RenderControl\shader.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include the Map2D as we will use it to check the player's movements and actions
class CMap2D;

// Include Settings
#include "GameControl\Settings.h"

// Include Physics2D
#include "Physics2D.h"

// Include Player2D
#include "Player2D.h"

class CEnemy2D : public CEntity2D
{
public:
	// Constructor
	CEnemy2D(void);

	// Destructor
	virtual ~CEnemy2D(void);

	// Init
	bool Init(void);

	// Update
	void Update(const double dElapsedTime);

	// PreRender
	//	void PreRender(void);

	// Render
	void Render(void);

	// PostRender
	//	void PostRender(void);

	// Set a shader to this class instance
	//	void SetShader(Shader* cShader);

	//// Set the indices of the enemy2D
	//void Seti32vec2Index(const int iIndex_XAxis, const int iIndex_YAxis);

	//// Set the number of microsteps of the enemy2D
	//void Seti32vec2NumMicroSteps(const int iNumMicroSteps_XAxis, const int iNumMicroSteps_YAxis);

	//// Set the UV coordinates of the enemy2D
	//void Setvec2UVCoordinates(const float fUVCoordinate_XAxis, const float fUVCoordinate_YAxis);

	//// Get the indices of the enemy2D
	//glm::i32vec2 Geti32vec2Index(void) const;

	//// Get the number of microsteps of the enemy2D
	//glm::i32vec2 Geti32vec2NumMicroSteps(void) const;

	//// Set the UV coordinates of the enemy2D
	//glm::vec2 Getvec2UVCoordinates(void) const;

	// Set the handle to cPlayer to this class instance
	void SetPlayer2D(CPlayer2D* cPlayer2D);

protected:
	enum DIRECTION
	{
		LEFT = 0,
		RIGHT = 1,
		UP = 2,
		DOWN = 3,
		NUM_DIRECTIONS
	};

	enum FSM
	{
		IDLE = 0,
		PATROL = 1,
		ATTACK = 2,
		NUM_FSM
	};

	// Handler to the Shader Program instance
	//	Shader* cShader;

	// OpenGL objects
	//	unsigned int VBO, VAO, EBO;

	// The texture ID in OpenGL
	//	unsigned int iTextureID;

	// Handler to the CMap2D instance
	CMap2D* cMap2D;

	// A transformation matrix for controlling where to render the entities
	//	glm::mat4 transform;

	// The i32vec2 which stores the indices of the enemy2D in the Map2D
	//	glm::i32vec2 i32vec2Index;

	// The i32vec2 variable which stores The number of microsteps from the tile indices for the enemy2D. 
	// A tile's width or height is in multiples of these microsteps
	//	glm::i32vec2 i32vec2NumMicroSteps;

	// The vec2 variable which stores the UV coordinates to render the enemy2D
	glm::vec2 vec2UVCoordinate;

	// The i32vec2 which stores the indices of the destination for enemy2D in the Map2D
	glm::i32vec2 i32vec2Destination;
	// The i32vec2 which stores the direction for enemy2D movement in the Map2D
	glm::i32vec2 i32vec2Direction;

	// Settings
//	CSettings* cSettings;

	// Physics
	CPhysics2D cPhysics2D;

	// Handle to the CPlayer2D
	CPlayer2D* cPlayer2D;

	// Current FSM
	FSM sCurrentFSM;

	// FSM counter - count how many frames it has been in this FSM
	int iFSMCounter;

	// Max Idle
	const int iMaxFSMCounter = 60;

	// Real-Time Loop Control
	// Total elapsed time
	double dTotalElapsedTime;
	// We set the minimum elapsed time to 4 times per second or 0.25Hz
	const double dMinimumElapsedTime = 0.25;

	// Load a texture
	//	bool LoadTexture(const char* filename);

	// Constraint the enemy2D's position within a boundary
	void Constraint(DIRECTION eDirection = LEFT);

	// Find a feasible position for the enemy2D's current position
	bool FindFeasiblePosition(const int iOldIndex, DIRECTION eDirection = LEFT);

	// Check if the enemy2D is in mid-air
	bool IsMidAir(void);

	// Update Jump or Fall
	void UpdateJumpFall(const double dElapsedTime);

	// Let enemy2D interact with the player
	bool InteractWithPlayer(void);

	// Update direction
	void UpdateDirection(void);

	// Flip horizontal direction. For patrol use only
	void FlipHorizontalDirection(void);

	// Update position
	void UpdatePosition(void);
};

