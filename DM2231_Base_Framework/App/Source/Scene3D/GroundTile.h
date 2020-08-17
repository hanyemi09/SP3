/**
 CGroundTile
 By: Toh Da Jun
 Date: Apr 2020
 */
#pragma once

// Include Entity3D
#include <Primitives/Entity3D.h>

// Include GLEW
#include <GL/glew.h>
// Include GLM
#include <includes\glm.hpp>

// Include shader
#include <RenderControl\shader.h>

class CGroundTile : public CEntity3D
{
public:
	// Constructor
	CGroundTile(void);

	// Destructor
	~CGroundTile(void);

	// Initialise this class instance
	bool Init(void);

	// Destroy the data in this class instance
	void Destroy(void);

	// Set model
	virtual void SetModel(glm::mat4 model);
	// Set view
	virtual void SetView(glm::mat4 view);
	// Set projection
	virtual void SetProjection(glm::mat4 projection);

	// ReCalculate
	bool ReCalculate(void);

	// Update this class instance
	virtual void Update(const double dElapsedTime);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

	// Get the exact position at a certain position where you only have the x- and z- values.
	glm::vec3 GetExactPosition(glm::vec3 vec3Position);

protected:
	// The coordinate of the rear-left corner of the tile in X-, Z-axes
	glm::vec3 vec3TileRearLeft;
	// The coordinate of the front-right corner of the tile in X-, Z-axes
	glm::vec3 vec3TileFrontRight;

};
