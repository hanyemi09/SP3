/**
 CStructure3D
 By: Toh Da Jun
 Date: Apr 2020
 */
#pragma once

// Include Entity3D
#include <Primitives/Entity3D.h>

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include Camera
#include "Camera.h"

// Include Player3D
#include "Player3D.h"

// Include GroundMap
#include "GroundMap.h"

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class CStructure2_3D : public CEntity3D
{
public:
	// Default Constructor
	CStructure2_3D(void);

	// Constructor with vectors
	CStructure2_3D(	const glm::vec3 vec3Position,
				const glm::vec3 vec3Front = glm::vec3(0.0f, 0.0f, 1.0f),
				const float fYaw = 0.0f,
				const float fPitch = 0.0f);

	// Destructor
	virtual ~CStructure2_3D(void);

	// Initialise this class instance
	bool Init(void);

	// Set model
	virtual void SetModel(glm::mat4 model);
	// Set view
	virtual void SetView(glm::mat4 view);
	// Set projection
	virtual void SetProjection(glm::mat4 projection);

	// Update this class instance
	virtual void Update(const double dElapsedTime);

	// Activate the CCollider for this class instance
	virtual void ActivateCollider(Shader* cLineShader = NULL);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

protected:
	// The handle to the CGroundMap class instance
	CGroundMap* cGroundMap;
};
