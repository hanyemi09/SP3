/**
 CPlayer3D
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "Player3D.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CPlayer3D::CPlayer3D(void)
	: vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 1.0f, 0.0f))
	, vec3WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, fYaw(-90.0f)
	, fPitch(0.0f)
	, fMouseSensitivity(0.1f)
	, cCamera(NULL)
	, cGroundMap(NULL)
	, cPrimaryWeapon(NULL)
	, cSecondaryWeapon(NULL)
	, iCurrentWeapon(0)
	, fCameraSwayAngle(0.0f)
	, fCameraSwayDeltaAngle(0.15f)
	, bCameraSwayDirection(false)	// false = left, true = right
	, bCameraSwayActive(true)
{
	// Set the default position so it is above the ground
	vec3Position = glm::vec3(0.0f, 0.5f, 0.0f);

	// Update the player vectors
	UpdatePlayerVectors();
}

/**
 @brief Constructor with vectors
 @param position A const glm::vec3 variable which contains the position of the camera
 @param up A const glm::vec3 variable which contains the up direction of the camera
 @param yaw A const float variable which contains the yaw of the camera
 @param pitch A const float variable which contains the pitch of the camera
 */
CPlayer3D::CPlayer3D(	const glm::vec3 vec3Position,
						const glm::vec3 vec3Front,
						const float fYaw,
						const float fPitch)
	: vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 1.0f, 0.0f))
	, vec3WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, fYaw(fYaw)
	, fPitch(fPitch)
	, fMouseSensitivity(0.1f)
	, cCamera(NULL)
	, cGroundMap(NULL)
	, cPrimaryWeapon(NULL)
	, cSecondaryWeapon(NULL)
	, iCurrentWeapon(0)
	, fCameraSwayAngle(0.0f)
	, fCameraSwayDeltaAngle(0.5f)
	, bCameraSwayDirection(false)	// false = left, true = right
	, bCameraSwayActive(true)
{
	// Set the default position so it is above the ground
	this->vec3Position = glm::vec3(0.0f, 0.5f, 0.0f);

	// Update the player vectors
	UpdatePlayerVectors();
}

/**
 @brief Destructor
 */
CPlayer3D::~CPlayer3D(void)
{
	if (cSecondaryWeapon)
	{
		delete cSecondaryWeapon;
		cSecondaryWeapon = NULL;
	}

	if (cPrimaryWeapon)
	{
		delete cPrimaryWeapon;
		cPrimaryWeapon = NULL;
	}

	if (cGroundMap)
	{
		// We set it to NULL only since it was declared somewhere else
		cGroundMap = NULL;
	}
	
	if (cCamera)
	{
		// We set it to NULL only since it was declared somewhere else
		cCamera = NULL;
	}
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CPlayer3D::Init(void)
{
	// Check if the shader is ready
	if (!cShader)
	{
		cout << "CPlayer3D::Init(): The shader is not available for this class instance." << endl;
		return false;
	}

	// Call the parent's Init()
	CEntity3D::Init();

	// Set the type
	SetType(CEntity3D::TYPE::PLAYER);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {
		// Rear face
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		// Front face
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		// Left face
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		// Right face
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		// Bottom face
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		// Top face
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// load and create a texture 
	iTextureID = LoadTexture("Image/Scene3D_Player.tga");
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/Scene3D_Player.tga" << endl;
		return false;
	}

	// Store the handler to the CGroundMap
	cGroundMap = CGroundMap::GetInstance();

	// Set the Physics to fall status by default
	cPhysics3D.SetStatus(CPhysics3D::STATUS::FALL);

	return true;
}

/**
 @brief Set model
 @param model A glm::mat4 variable containing the model for this class instance
 */
void CPlayer3D::SetModel(glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A glm::mat4 variable containing the model for this class instance
 */
void CPlayer3D::SetView(glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A glm::mat4 variable containing the model for this class instance
 */
void CPlayer3D::SetProjection(glm::mat4 projection)
{
	this->projection = projection;
}

/**
@brief Returns the view matrix calculated using Euler Angles and the LookAt Matrix
@return A glm::mat4 variable which contains the view matrix
*/
glm::mat4 CPlayer3D::GetViewMatrix(void) const
{
	return glm::lookAt(vec3Position, vec3Position + vec3Front, vec3Up);
}

/**
 @brief Attach a camera to this class instance
 @param cCamera A CCamera* variable which contains the camera
 */
void CPlayer3D::AttachCamera(CCamera* cCamera)
{
	// Set the camera to the player
	this->cCamera = cCamera;

	// Update the camera's attributes with the player's attributes
	if (cCamera)
	{
		cCamera->vec3Position = vec3Position;
		cCamera->vec3Front = vec3Front;
		cCamera->vec3Up = vec3Up;
		cCamera->vec3Right = vec3Right;
		cCamera->fYaw = fYaw;
		cCamera->fPitch = fPitch;
	}
}

/**
 @brief Check if a camera ia attached to this class instance
 @return true if a camera is attached, else false
 */
bool CPlayer3D::IsCameraAttached(void)
{
	if (cCamera)
		return true;
	return false;
}

/**
 @brief Set Weapon to this class instance
 @param iSlot A const int variable which contains the weapon info to check for. 0 == Primary, 1 == Secondary
 @param cWeaponInfo A CWeaponInfo* variable which contains the weapon info
 */
void CPlayer3D::SetWeapon(const int iSlot, CWeaponInfo* cWeaponInfo)
{
	if (iSlot == 0)
		cPrimaryWeapon = cWeaponInfo;
	else if (iSlot == 1)
		cSecondaryWeapon = cWeaponInfo;
}

/**
 @brief Get Weapon of this class instance
 @param iSlot A const int variable which contains the weapon info to check for. 0 == Primary, 1 == Secondary
 */
CWeaponInfo* CPlayer3D::GetWeapon(void) const
{
	if (iCurrentWeapon == 0)
		return cPrimaryWeapon;
	else if (iCurrentWeapon == 1)
		return cSecondaryWeapon;

	return NULL;
}

/**
 @brief Set current weapon
 @param iSlot A const int variable which contains the weapon info to check for. 0 == Primary, 1 == Secondary
 */
void CPlayer3D::SetCurrentWeapon(const int iSlot)
{
	iCurrentWeapon = iSlot;
}

/**
 @brief Discharge weapon
 @param iSlot A const int variable which contains the weapon info to check for. 0 == Primary, 1 == Secondary
 */
CProjectile* CPlayer3D::DischargeWeapon(void) const
{
	if ((iCurrentWeapon == 0) && (cPrimaryWeapon))
	{
		return cPrimaryWeapon->Discharge(vec3Position, vec3Front, (CEntity3D*)this);
	}
	else if ((iCurrentWeapon == 1) && (cSecondaryWeapon))
	{
		return cSecondaryWeapon->Discharge(vec3Position, vec3Front, (CEntity3D*)this);
	}
	return NULL;
}

/**
 @brief Set to Jump
 */
void CPlayer3D::SetToJump(void)
{
	if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::IDLE)
	{
		cPhysics3D.SetStatus(CPhysics3D::STATUS::JUMP);
		cPhysics3D.SetInitialVelocity(glm::vec3(0.0f, 10.0f, 0.0f));
	}
}

/**
 @brief Processes input received from any keyboard-like input system as player movements. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
 @param direction A const Player_Movement variable which contains the movement direction of the camera
 @param deltaTime A const float variable which contains the delta time for the realtime loop
 */
void CPlayer3D::ProcessMovement(const Player_Movement direction, const float deltaTime)
{
	float velocity = fMovementSpeed * deltaTime;
	if (direction == FORWARD)
		vec3Position += vec3Front * velocity;
	if (direction == BACKWARD)
		vec3Position -= vec3Front * velocity;
	if (direction == LEFT)
		vec3Position -= vec3Right * velocity;
	if (direction == RIGHT)
		vec3Position += vec3Right * velocity;
	if (direction == SPRINT)
		vec3Position += vec3Front * velocity * glm::vec3(0, 0, 2.5);

	// Indicate that camera sway is to be updated
	if (bCameraSwayActive)
		bUpdateCameraSway = true;
}

/**
 @brief Processes input received from a mouse input system as player rotation. Expects the offset value in both the x and y direction.
 @param xoffset A const float variable which contains the x axis of the mouse movement
 @param yoffset A const float variable which contains the y axis of the mouse movement
 @param constrainPitch A const bool variable which indicates if the pitch will be constrained at upright positions
 */
void CPlayer3D::ProcessRotate(float fXOffset, float fYOffset, const bool constrainPitch)
{
	fXOffset *= fMouseSensitivity;
	fYOffset *= fMouseSensitivity;

	fYaw += fXOffset;
	fPitch += fYOffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (fPitch > 89.0f)
			fPitch = 89.0f;
		if (fPitch < -89.0f)
			fPitch = -89.0f;
	}

	// Update vec3Front, Right and vec3Up Vectors using the updated Euler angles
	UpdatePlayerVectors();
}

/**
 @brief Update this class instance
 @param deltaTime A const float variable which contains the delta time for the realtime loop
 */
void CPlayer3D::Update(const double dElapsedTime)
{
	if (cPrimaryWeapon)
		cPrimaryWeapon->Update(dElapsedTime);
	if (cSecondaryWeapon)
		cSecondaryWeapon->Update(dElapsedTime);

	// Update the Jump/Fall
	UpdateJumpFall(dElapsedTime);

	// If the camera is attached to this player, then update the camera
	if (cCamera)
	{
		cCamera->vec3Position = vec3Position;
		cCamera->vec3Front = vec3Front;
		cCamera->vec3Up = vec3Up;
		cCamera->vec3Right = vec3Right;
		cCamera->fYaw = fYaw + sin(fCameraSwayAngle);
		cCamera->fPitch = fPitch + cos(fCameraSwayAngle);
	}

	// Constraint the player's position
	Constraint();

	// Implement player/camera sway
	if ((bUpdateCameraSway) && (bCameraSwayActive))
	{
		glm::mat4 rotationMat(1); // Creates a identity matrix
		rotationMat = glm::rotate(rotationMat, glm::radians(fCameraSwayAngle), vec3Front);
		vec3Up = glm::vec3(rotationMat * glm::vec4(vec3WorldUp, 1.0f));

		if (bCameraSwayDirection == false)	// Sway to left
		{
			fCameraSwayAngle -= fCameraSwayDeltaAngle;
			if (fCameraSwayAngle < fCameraSwayAngle_LeftLimit)
			{
				fCameraSwayAngle = fCameraSwayAngle_LeftLimit;
				bCameraSwayDirection = !bCameraSwayDirection;
			}
		}
		else if (bCameraSwayDirection == true)	// Sway to right
		{
			fCameraSwayAngle += fCameraSwayDeltaAngle;
			if (fCameraSwayAngle > fCameraSwayAngle_RightLimit)
			{
				fCameraSwayAngle = fCameraSwayAngle_RightLimit;
				bCameraSwayDirection = !bCameraSwayDirection;
			}
		}

		bUpdateCameraSway = false;
	}
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CPlayer3D::PreRender(void)
{
	// Draw ground tile as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
}

/**
 @brief Render Render this instance
 */
void CPlayer3D::Render(void)
{
	// Don't render the player if the camera is attached to it
	if (cCamera)
		return;

	// If the shader is in this class, then do not render
	if (!cShader)
	{
		cout << "CPlayer3D::Render(): The shader is not available for this class instance." << endl;
		return;
	}

	// Activate shader
	cShader->use();

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, iTextureID);

	// create transformations
	model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model = glm::translate(model, glm::vec3(vec3Position.x, vec3Position.y, vec3Position.z));
	model = glm::scale(model, vec3Scale);
	model = glm::rotate(model, glm::radians(fYaw), glm::vec3(0.0f, 1.0f, 0.0f));

	// note: currently we set the projection matrix each frame, but since the projection 
	// matrix rarely changes it's often best practice to set it outside the main loop only once.
	cShader->setMat4("projection", projection);
	cShader->setMat4("view", view);
	cShader->setMat4("model", model);

	// render boxes
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Render the CCollider if needed
	if ((cCollider) && (cCollider->bIsDisplayed))
	{
		cCollider->model = model;
		cCollider->view = view;
		cCollider->projection = projection;
		cCollider->Render();

	}
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CPlayer3D::PostRender(void)
{
	glDepthFunc(GL_LESS); // set depth function back to default
}

/**
 @brief Calculates the front vector from the Camera's (updated) Euler Angles
 */
void CPlayer3D::UpdatePlayerVectors(void)
{
	// Calculate the new vec3Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	front.y = sin(glm::radians(fPitch));
	front.z = sin(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	vec3Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	// Normalize the vectors, because their length gets closer to 0 the more 
	// you look up or down which results in slower movement.
	vec3Right = glm::normalize(glm::cross(vec3Front, vec3WorldUp));  
	if (!bCameraSwayActive)
		vec3Up = glm::normalize(glm::cross(vec3Right, vec3Front));

	// If the camera is attached to this player, then update the camera
	if (cCamera)
	{
		cCamera->vec3Front = vec3Front;
		cCamera->vec3Up = vec3Up;
		cCamera->vec3Right = vec3Right;
		cCamera->fYaw = fYaw;
		cCamera->fPitch = fPitch;
	}
}

/**
 @brief Constraint the player's position
 */
void CPlayer3D::Constraint(void)
{
	if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::IDLE)
	{
		// If the player is not jumping nor falling, then we snap his position to the cGroundMap
		vec3Position = cGroundMap->GetExactPosition(vec3Position);
	}
	else
	{
		// If the player is jumping nor falling, then we only snap his position 
		// if the player's y-coordinate is below the cGroundMap 
		glm::vec3 tempVec3Position = cGroundMap->GetExactPosition(vec3Position);
		vec3Position.x = tempVec3Position.x;
		vec3Position.z = tempVec3Position.z;
		if (vec3Position.y < tempVec3Position.y)
			vec3Position.y = tempVec3Position.y;
	}
}

/**
 @brief Update Jump or Fall
 @param deltaTime A const float variable which contains the delta time for the realtime loop
 */
void CPlayer3D::UpdateJumpFall(const double dElapsedTime)
{
	if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::JUMP)
	{
		// Update the elapsed time to the physics engine
		cPhysics3D.AddElapsedTime((float)dElapsedTime);
		// Call the physics engine update method to calculate the final velocity and displacement
		cPhysics3D.Update();
		// Get the displacement from the physics engine and update the player position
		vec3Position = vec3Position + cPhysics3D.GetDisplacement();

		// If the player is still jumping and the initial velocity has reached zero or below zero, 
		// then it has reach the peak of its jump
		if ((cPhysics3D.GetStatus() == CPhysics3D::STATUS::JUMP) && (cPhysics3D.GetInitialVelocity().y <= 0.0f))
		{
			// Set status to fall
			cPhysics3D.SetStatus(CPhysics3D::STATUS::FALL);
		}
	}
	else if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::FALL)
	{
		// Update the elapsed time to the physics engine
		cPhysics3D.AddElapsedTime(0.1f);
		// Call the physics engine update method to calculate the final velocity and displacement
		cPhysics3D.Update();
		// Get the displacement from the physics engine and update the player position
		vec3Position = vec3Position + cPhysics3D.GetDisplacement();

		// Constaint the player's position to the groundmap
		glm::vec3 vec3CheckPosition = cGroundMap->GetExactPosition(vec3Position);

		// Set the Physics to idle status
		if (vec3CheckPosition.y > vec3Position.y)
		{
			vec3Position = vec3CheckPosition;
			cPhysics3D.SetStatus(CPhysics3D::STATUS::IDLE);
		}
	}
}