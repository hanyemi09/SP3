/**
 CEnemy3D
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
class CEnemy3D : public CEntity3D
{
public:
	// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
	enum Enemy_Movement {
		FORWARD = 0,
		BACKWARD,
		LEFT,
		RIGHT,
		NUM_MOVEMENTS
	};

	// Default Constructor
	CEnemy3D(void);

	// Constructor with vectors
	CEnemy3D(	const glm::vec3 vec3Position,
				const glm::vec3 vec3Front = glm::vec3(0.0f, 0.0f, -1.0f),
				const float fYaw = -90.0f,
				const float fPitch = 0.0f);

	// Destructor
	virtual ~CEnemy3D(void);

	// Initialise this class instance
	bool Init(void);

	// Set model
	virtual void SetModel(glm::mat4 model);
	// Set view
	virtual void SetView(glm::mat4 view);
	// Set projection
	virtual void SetProjection(glm::mat4 projection);

	// Attach a camera to this class instance
	void AttachCamera(CCamera* cCamera = NULL);
	// Check if a camera ia attached to this class instance
	bool IsCameraAttached(void);

    // Processes input received from any keyboard-like input system as Enemy movements. 
	// Accepts input parameter in the form of Enemy defined ENUM (to abstract it from windowing systems)
	void ProcessMovement(const Enemy_Movement direction, const float fDeltaTime);

    // Processes input received from a mouse input system as Enemy rotation. 
	// Expects the offset value in both the x and y direction.
	void ProcessRotate(const float fXOffset);

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

	int get_enemyHealth();
	void set_enemyHealth(int x);

protected:
	// Enemy Attributes
	glm::vec3 vec3Up;
	glm::vec3 vec3Right;
	glm::vec3 vec3WorldUp;
	// Euler Angles
	float fYaw;
	float fPitch;
	// Enemy options
	float fRotationSensitivity;

	// Movement Control
	int iCurrentNumMovement;
	int iMaxNumMovement;

	int enemyHealth;

	// The handle to the CCamera class instance
	CCamera* cCamera;

	// The handle to the CPlayer3D class instance
	CPlayer3D* cPlayer3D;

	// The handle to the CGroundMap class instance
	CGroundMap* cGroundMap;

    // Calculates the front vector from the Enemy's (updated) Euler Angles
	void UpdateEnemyVectors(void);

	// Constraint the Enemy's position
	void Constraint(void);
};
