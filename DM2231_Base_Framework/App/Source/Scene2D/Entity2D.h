/**
 CEntity2D
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

// Include shader
#include "RenderControl\shader.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include Settings
#include "GameControl\Settings.h"

class CEntity2D
{
public:
	// Constructor
	CEntity2D(void);

	// Destructor
	virtual ~CEntity2D(void);

	// Init
	virtual bool Init(void);

	// Update
	virtual void Update(const double dElapsedTime);

	// PreRender
	virtual void PreRender(void);

	// Render
	virtual void Render(void);

	// PostRender
	virtual void PostRender(void);

	// Set a shader to this class instance
	virtual void SetShader(Shader* cShader);

	// Set the indices of the enemy2D
	void Seti32vec2Index(const int iIndex_XAxis, const int iIndex_YAxis);

	// Set the number of microsteps of the enemy2D
	void Seti32vec2NumMicroSteps(const int iNumMicroSteps_XAxis, const int iNumMicroSteps_YAxis);

	// Set the UV coordinates of the enemy2D
	void Setvec2UVCoordinates(const float fUVCoordinate_XAxis, const float fUVCoordinate_YAxis);

	// Get the indices of the enemy2D
	glm::i32vec2 Geti32vec2Index(void) const;

	// Get the number of microsteps of the enemy2D
	glm::i32vec2 Geti32vec2NumMicroSteps(void) const;

	// Set the UV coordinates of the enemy2D
	glm::vec2 Getvec2UVCoordinates(void) const;

protected:
	// Handler to the Shader Program instance
	Shader* cShader;

	// OpenGL objects
	unsigned int VBO, VAO, EBO;

	// The texture ID in OpenGL
	unsigned int iTextureID;

	// A transformation matrix for controlling where to render the entities
	glm::mat4 transform;

	// The i32vec2 which stores the indices of the enemy2D in the Map2D
	glm::i32vec2 i32vec2Index;

	// The i32vec2 variable which stores The number of microsteps from the tile indices for the enemy2D. 
	// A tile's width or height is in multiples of these microsteps
	glm::i32vec2 i32vec2NumMicroSteps;

	// The vec2 variable which stores the UV coordinates to render the enemy2D
	glm::vec2 vec2UVCoordinate;

	// Load a texture
	virtual bool LoadTexture(const char* filename);

	// Settings
	CSettings* cSettings;
};
