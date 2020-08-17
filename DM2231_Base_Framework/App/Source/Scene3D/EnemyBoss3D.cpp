/**
 CEnemy3D
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "EnemyBoss3D.h"

// Allowing loading of LoadOBJ.h
#include "System/LoadOBJ.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CEnemyBoss3D::CEnemyBoss3D(void)
	: vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 1.0f, 0.0f))
	, vec3WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, fYaw(-90.0f)
	, fPitch(0.0f)
	, fRotationSensitivity(0.1f)
	, cCamera(NULL)
	, cPlayer3D(NULL)
	, cGroundMap(NULL)
	, enemyHealth(3)
{
	// Set the default position to the origin
	vec3Position = glm::vec3(0.0f, 0.0f, 0.0f);

	// Update the vectors for this enemy
	UpdateEnemyVectors();
}

/**
 @brief Constructor with vectors
 @param vec3Position A const glm::vec3 variable which contains the position of the camera
 @param vec3Front A const glm::vec3 variable which contains the up direction of the camera
 @param yaw A const float variable which contains the yaw of the camera
 @param pitch A const float variable which contains the pitch of the camera
 */
CEnemyBoss3D::CEnemyBoss3D(	const glm::vec3 vec3Position,
					const glm::vec3 vec3Front,
					const float fYaw,
					const float fPitch)
	: vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 1.0f, 0.0f))
	, vec3WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, fYaw(fYaw)
	, fPitch(fPitch)
	, fRotationSensitivity(0.1f)
	, cCamera(NULL)
	, cPlayer3D(NULL)
	, cGroundMap(NULL)
	, enemyHealth(30)
{
	// Set the default position to the origin
	this->vec3Position = vec3Position;

	// Update the vectors for this enemy
	UpdateEnemyVectors();
}

/**
 @brief Destructor
 */
CEnemyBoss3D::~CEnemyBoss3D(void)
{
	if (cGroundMap)
	{
		// We set it to NULL only since it was declared somewhere else
		cGroundMap = NULL;
	}

	if (cPlayer3D)
	{
		// We set it to NULL only since it was declared somewhere else
		cPlayer3D = NULL;
	}

	if (cCamera)
	{
		// We set it to NULL only since it was declared somewhere else
		cCamera = NULL;
	}

	// Delete the rendering objects in the graphics card
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CEnemyBoss3D::Init(void)
{
	// Check if the shader is ready
	if (!cShader)
	{
		cout << "CEnemy3D::Init(): The shader is not available for this class instance." << endl;
		return false;
	}

	// Call the parent's Init()
	CEntity3D::Init();

	// Set the type
	SetType(CEntity3D::TYPE::NPC);

	// Initialise the cPlayer3D
	cPlayer3D = CPlayer3D::GetInstance();

	std::vector<glm::vec3> vertices;
	std:: vector <glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	vec3Scale = glm::vec3(0.125, 0.125, 0.125);
	vec3ColliderScale = glm::vec3(1.5, 4, 1.5);

	std::string file_path = "OBJ//boss2.obj";
	bool success = LoadOBJ(file_path.c_str(), vertices, uvs, normals);
	if (!success)
		return NULL;
	std::vector <Vertex> vertex_buffer_data;
	std::vector <GLuint> index_buffer_data;
	IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint),&index_buffer_data[0], GL_STATIC_DRAW);
	index_buffer_size = index_buffer_data.size();

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec3)));

	// load and create a texture 
	iTextureID = LoadTexture("Image/boss2.tga");
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/Scene3D_Enemy_01.tga" << endl;
		return false;
	}

	// Store the handler to the CGroundMap
	cGroundMap = CGroundMap::GetInstance();

	// Movement Control
	iCurrentNumMovement = 0;
	iMaxNumMovement = 50;

	return true;
}


/**
 @brief Set model
 @param model A glm::mat4 variable containing the model for this class instance
 */
void CEnemyBoss3D::SetModel(glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A glm::mat4 variable containing the model for this class instance
 */
void CEnemyBoss3D::SetView(glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A glm::mat4 variable containing the model for this class instance
 */
void CEnemyBoss3D::SetProjection(glm::mat4 projection)
{
	this->projection = projection;
}

/**
 @brief Attach a camera to this class instance
 @param cCamera A CCamera* variable which contains the camera
 */
void CEnemyBoss3D::AttachCamera(CCamera* cCamera)
{
	// Set the camera to the player
	this->cCamera = cCamera;

	// Update the camera's attributes with the player's attributes
	if (cCamera)
	{
		cCamera->vec3Position = vec3Position;
		cCamera->vec3Front = vec3Front;
	}
}

/**
 @brief Check if a camera ia attached to this class instance
 @return true if a camera is attached, else false
 */
bool CEnemyBoss3D::IsCameraAttached(void)
{
	if (cCamera)
		return true;
	return false;
}

/**
 @brief Processes input received from any keyboard-like input system as player movements. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
 @param direction A const Player_Movement variable which contains the movement direction of the camera
 @param deltaTime A const float variable which contains the delta time for the realtime loop
 */
void CEnemyBoss3D::ProcessMovement(const Enemy_Movement direction, const float deltaTime)
{
	float velocity = fMovementSpeed * deltaTime * 2.5;
	if (direction == FORWARD)
		vec3Position += vec3Front * velocity;
	if (direction == BACKWARD)
		vec3Position -= vec3Front * velocity;
	if (direction == LEFT)
		vec3Position -= vec3Right * velocity;
	if (direction == RIGHT)
		vec3Position += vec3Right * velocity;

	// If the camera is attached to this player, then update the camera
	if (cCamera)
	{
		cCamera->vec3Position = vec3Position;
	}

	// Constraint the player's position
	Constraint();
}

/**
 @brief Processes input received from a mouse input system as player rotation. Expects the offset value in both the x and y direction.
 @param xoffset A const float variable which contains the x axis of the mouse movement
 @param yoffset A const float variable which contains the y axis of the mouse movement
 @param constrainPitch A const bool variable which indicates if the pitch will be constrained at upright positions
 */
void CEnemyBoss3D::ProcessRotate(const float fXOffset)
{
	// Update the yaw
	fYaw += fXOffset;// *fRotationSensitivity;

	// Update vec3Front, Right and vec3Up Vectors using the updated Euler angles
	UpdateEnemyVectors();
}

/**
@brief Update this class instance
*/
void CEnemyBoss3D::Update(const double dElapsedTime)
{
	// Store the enemy's current position, if rollback is needed.
	StorePositionForRollback();

	if (iCurrentNumMovement < iMaxNumMovement)
	{
		// Process the movement
		ProcessMovement(FORWARD, (float)dElapsedTime);

		// Update the counter
		iCurrentNumMovement++;
	}
	else
	{
		// Randomly choose a new direction up to +30 or -30 degrees to the current direction 
		ProcessRotate(rand() % 60 - 45.0f);
		
		// Reset the counter to 0
		iCurrentNumMovement = 0;
	}
}

/**
@brief Activate the CCollider for this class instance
@param cLineShader A Shader* variable which stores a shader which renders lines
*/
void CEnemyBoss3D::ActivateCollider(Shader* cLineShader)
{
	// Since we are changing the colour, we don't call the parent's ActivateCollider() method.
	// Instead we create it here and insert our colour changing codes
	//CEntity3D::ActivateCollider(cLineShader);

	// Create a new CCollider
	cCollider = new CCollider();
	// Set the colour of the CCollider to Blue
	cCollider->vec4Colour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	// Initialise the cCollider
	cCollider->Init();
	// Set a shader to it
	cCollider->SetLineShader(cLineShader);
}

/**
@brief PreRender Set up the OpenGL display environment before rendering
*/
void CEnemyBoss3D::PreRender(void)
{
	// Draw this as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
}

/**
@brief Render Render this instance
@param cShader A Shader* variable which contains the Shader to use in this class instance
*/
void CEnemyBoss3D::Render(void)
{
	// If the shader is in this class, then do not render
	if (!cShader)
	{
		cout << "CEnemy3D::Render(): The shader is not available for this class instance." << endl;
		return;
	}

	// Activate shader
	cShader->use();

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, iTextureID);

	// create transformations
	model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	//model = glm::rotate(model, (float)glfwGetTime()/10.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, vec3Position);
	model = glm::scale(model, vec3Scale);

	// note: currently we set the projection matrix each frame, but since the projection 
	// matrix rarely changes it's often best practice to set it outside the main loop only once.
	cShader->setMat4("projection", projection);
	cShader->setMat4("view", view);
	cShader->setMat4("model", model);

	// render boxes
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, index_buffer_size, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, index_buffer_size, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	// Render the CCollider if needed
	if ((cCollider) && (cCollider->bIsDisplayed))
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, vec3Position);
		model = glm::scale(model, vec3ColliderScale);

		cCollider->model = model;
		cCollider->view = view;
		cCollider->projection = projection;
		cCollider->Render();
	}
}

/**
@brief PostRender Set up the OpenGL display environment after rendering.
*/
void CEnemyBoss3D::PostRender(void)
{
	glDepthFunc(GL_LESS); // set depth function back to default
}

int CEnemyBoss3D::get_enemyHealth()
{
	return enemyHealth;
}

void CEnemyBoss3D::set_enemyHealth(int x)
{
	enemyHealth = x;
}

/**
 @brief Calculates the front vector from the Camera's (updated) Euler Angles
 */
void CEnemyBoss3D::UpdateEnemyVectors(void)
{
	// Calculate the new vec3Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	front.y = sin(glm::radians(fPitch));
	front.z = sin(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	front = glm::normalize(front);

	// Check if we are too far from the player
	if (cPlayer3D)
	{
	float fDistanceToPlayer = glm::length(cPlayer3D->GetPosition() - vec3Position);
		if (fDistanceToPlayer > 15.0f)
		{
			// Update the direction of the enemy
			front = glm::normalize(glm::vec3(cPlayer3D->GetPosition() - vec3Position));
			
			// Update the yaw and pitch
			fYaw = glm::degrees(glm::atan(front.z, front.x));
			fPitch = glm::degrees(glm::asin(front.y));
		}
	}
	
	vec3Front = front;
	// Also re-calculate the Right and Up vector
	// Normalize the vectors, because their length gets closer to 0 the more 
	// you look up or down which results in slower movement.
	vec3Right = glm::normalize(glm::cross(vec3Front, vec3WorldUp));  
	vec3Up = glm::normalize(glm::cross(vec3Right, vec3Front));

	// If the camera is attached to this player, then update the camera
	if (cCamera)
	{
		cCamera->vec3Front = vec3Front;
		cCamera->vec3Right = vec3Right;
		cCamera->vec3Up = vec3Up;
	}
}

/**
 @brief Constraint the player's position
 */
void CEnemyBoss3D::Constraint(void)
{
	vec3Position = cGroundMap->GetExactPosition(vec3Position);
}