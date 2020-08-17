/**
 CGroundMap
 By: Toh Da Jun
 Date: Apr 2020
 */
#pragma once

// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

// Include GLEW
#include <GL/glew.h>
// Include GLM
#include <includes\glm.hpp>

// Include shader
#include <RenderControl\shader.h>

// Include Settings
#include "GameControl\Settings.h"

// Include CGroundTile
#include "GroundTile.h"

// Include vector to store all the ground tile
#include <vector>
using namespace std;

class CGroundMap : public CSingletonTemplate<CGroundMap>
{
	friend CSingletonTemplate<CGroundMap>;
public:
	// Initialise this class instance
	bool Init(	glm::vec3 vec3MapSize = glm::vec3(50.0f, 1.0f, 50.0f), 
				glm::i32vec3 i32vec3NumTiles = glm::i32vec3(10, 1, 10));

	// Set model
	virtual void SetModel(glm::mat4 model);
	// Set view
	virtual void SetView(glm::mat4 view);
	// Set projection
	virtual void SetProjection(glm::mat4 projection);

	// Set a shader to this class instance
	void SetShader(Shader* cShader);

	// Set map size
	bool SetMapSize(glm::vec3 vec3MapSize);
	// Set num tiles
	bool SetNumTiles(glm::i32vec3 i32vec3NumTiles);

	// ReCalculate
	bool ReCalculate(void);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

	// Get the exact position at a certain position where you only have the x- and z- values.
	glm::vec3 GetExactPosition(glm::vec3 vec3Position);

protected:
	// Handler to the Shader Program instance
	Shader* cShader;

	CSettings* cSettings;

	// The texture ID in OpenGL
	GLuint iTextureID_01, iTextureID_02;

	// Render Settings
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	// Map Settings
	// The size of the map in X-, Z-axes
	glm::vec3 vec3MapSize;
	// The size of each tile in X-, Z-axes
	glm::vec3 vec3TileSize;
	// The number of the tiles in X-, Z-axes
	glm::i32vec3 i32vec3NumTiles;

	// The coordinate of the rear-left corner of the map in X-, Z-axes
	glm::vec3 vec3MapRearLeft;
	// The coordinate of the front-right corner of the map in X-, Z-axes
	glm::vec3 vec3MapFrontRight;

	// Vectors of Vectors containing the Ground Tiles
	vector<vector<CGroundTile>> vGroundTiles;

	// Constructor
	CGroundMap(void);

	// Destructor
	~CGroundMap(void);

	// Load a texture
	bool LoadTexture(const char* filename, GLuint& iTextureID);
};
