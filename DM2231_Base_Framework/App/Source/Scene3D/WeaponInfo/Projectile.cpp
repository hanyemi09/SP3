#include "Projectile.h"

#include <iostream>
using namespace std;

/**
@brief Default Constructor
*/
CProjectile::CProjectile(void)
	: bStatus(false)
	, dLifetime(0.0f)
	, fSpeed(1.0f)
	, pSource(NULL)
{
}

/**
@brief Destructor
*/
CProjectile::~CProjectile(void)
{
	// We set it to NULL only since it was declared somewhere else
	pSource = NULL;
}

/** 
 @brief Set the position and direction of the projectile
 @param vec3Position A const glm::vec3 variable which contains the position of the projectile
 @param vec3Front A const glm::vec3 variable which contains the up direction of the projectile
 @param dLifetime A const float variable which contains the lifetime for this projectile
 @param fSpeed A const float variable which contains the speed of the projectile
 */
bool CProjectile::Init(	glm::vec3 vec3Position,
						glm::vec3 vec3Front,
						const double dLifetime,
						const float fSpeed)
{
	// Check if the shader is ready
	if (!cShader)
	{
		cout << "CProjectile::Init(): The shader is not available for this class instance." << endl;
		return false;
	}

	// Call the parent's Init()
	CEntity3D::Init();

	// Set the type
	SetType(CEntity3D::TYPE::PROJECTILE);

	// Set the attributes for this projectile
	this->vec3Position = vec3Position;
	this->vec3Front = vec3Front;
	this->dLifetime = dLifetime;
	this->fSpeed = fSpeed;
	vec4Colour = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {
		// Rear face
		-0.05f, -0.05f, -0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		-0.05f, 0.05f, -0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		0.05f, 0.05f, -0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		0.05f, 0.05f, -0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		0.05f, -0.05f, -0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		-0.05f, -0.05f, -0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,

		// Front face
		-0.05f, -0.05f, 0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		0.05f, -0.05f, 0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		0.05f, 0.05f, 0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		0.05f, 0.05f, 0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		-0.05f, 0.05f, 0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		-0.05f, -0.05f, 0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,

		// Left face
		-0.05f, 0.05f, 0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		-0.05f, 0.05f, -0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		-0.05f, -0.05f, -0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		-0.05f, -0.05f, -0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		-0.05f, -0.05f, 0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		-0.05f, 0.05f, 0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,

		// Right face
		0.05f, 0.05f, 0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		0.05f, -0.05f, 0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		0.05f, -0.05f, -0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		0.05f, -0.05f, -0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		0.05f, 0.05f, -0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		0.05f, 0.05f, 0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,

		// Bottom face
		-0.05f, -0.05f, -0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		0.05f, -0.05f, -0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		0.05f, -0.05f, 0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		0.05f, -0.05f, 0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		-0.05f, -0.05f, 0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		-0.05f, -0.05f, -0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,

		// Top face
		-0.05f, 0.05f, -0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		-0.05f, 0.05f, 0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		0.05f, 0.05f, 0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		0.05f, 0.05f, 0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		0.05f, 0.05f, -0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		-0.05f, 0.05f, -0.05f, vec4Colour.x, vec4Colour.y, vec4Colour.z
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	return true;
}

/**
 @brief Set model
 @param model A glm::mat4 variable containing the model for this class instance
 */
void CProjectile::SetModel(glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A glm::mat4 variable containing the model for this class instance
 */
void CProjectile::SetView(glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A glm::mat4 variable containing the model for this class instance
 */
void CProjectile::SetProjection(glm::mat4 projection)
{
	this->projection = projection;
}

/**
@brief Activate the CCollider for this class instance
@param cLineShader A Shader* variable which stores a shader which renders lines
*/
void CProjectile::ActivateCollider(Shader* cLineShader)
{
	// Since we are changing the colour, we don't call the parent's ActivateCollider() method.
	// Instead we create it here and insert our colour changing codes
	//CEntity3D::ActivateCollider(cLineShader);

	// Create a new CCollider
	cCollider = new CCollider();
	// Set the bottom left of the CCollider
	cCollider->vec3BottomLeft = glm::vec4(-0.05f, -0.05f, -0.05f, 1.0f);
	// Set the top right of the CCollider
	cCollider->vec3TopRight = glm::vec4(0.05f, 0.05f, 0.05f, 1.0f);
	// Set the colour of the CCollider to Blue
	cCollider->vec4Colour = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	// Initialise the cCollider
	cCollider->Init();
	// Set a shader to it
	cCollider->SetLineShader(cLineShader);
}

/** 
 @brief Activate the projectile. true == active, false == inactive
 @param bStatus A const bool variable which is true if this projectile is active, otherwise false.
 */
void CProjectile::SetStatus(const bool bStatus)
{
	if (bStatus == false)
		dLifetime = 0.0f;
	this->bStatus = bStatus;
}

/** 
 @brief Set the direction of the projectile
 @param vec3Front A const glm::vec3 variable which contains the up direction of the projectile
 */
void CProjectile::SetDirection(glm::vec3 vec3Front)
{
	this->vec3Front = vec3Front;
}

/** 
 @brief Set the lifetime of the projectile
 @param dLifetime A const float variable which contains the lifetime for this projectile
 */
void CProjectile::SetLifetime(const double dLifetime)
{
	this->dLifetime = dLifetime;
}

/** 
 @brief Set the speed of the projectile
 @param fSpeed A const float variable which contains the speed of the projectile
 */
void CProjectile::SetSpeed(const float fSpeed)
{
	this->fSpeed = fSpeed;
}

/** 
 @brief Set the source of the projectile
 @param pSource A CEntity3D* variable which is the CEntity3D who created this projectile
 */
void CProjectile::SetSource(CEntity3D* pSource)
{
	this->pSource = pSource;
}

/** 
 @brief get status of the projectile. true == active, false == inactive
 @return A const bool variable which is true if this projectile is active, otherwise false.
 */
bool CProjectile::GetStatus(void) const
{
	return bStatus;
}

/** 
 @brief Get the direction of the projectile
 @return A const glm::vec3 variable which contains the up direction of the projectile
 */
glm::vec3 CProjectile::GetDirection(void)
{
	return vec3Front;
}

/** 
 @brief Get the lifetime of the projectile
 @return A const float variable which contains the lifetime for this projectile
 */
double CProjectile::GetLifetime(void) const
{
	return dLifetime;
}

/** 
 @brief Get the speed of the projectile
 @return A const float variable which contains the speed of the projectile
 */
float CProjectile::GetSpeed(void) const
{
	return fSpeed;
}

/** 
 @brief Get the source of the projectile
 @return A CEntity3D* variable which is the CEntity3D who created this projectile
 */
CEntity3D* CProjectile::GetSource(void) const
{
	return pSource;
}

/** 
 @brief Update the status of this projectile
 @param dElapsedTime A const double variable which contains the delta time for the realtime loop
 */
void CProjectile::Update(const double dElapsedTime)
{
	if (bStatus == false)
		return;

	// Update TimeLife of projectile. Set to inactive if too long
	dLifetime -= dElapsedTime;
	if (dLifetime < 0.0f)
	{
		SetStatus(false);
		return;
	}

	// Update Position
	vec3Position = vec3Position + vec3Front * (float)dElapsedTime * fSpeed;
}

/**
@brief PreRender Set up the OpenGL display environment before rendering
*/
void CProjectile::PreRender(void)
{
	// Draw this as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
}

/** 
 @brief Render this projectile
 */
void CProjectile::Render(void)
{
	if (bStatus == false)
		return;

	if (dLifetime < 0.0f)
		return;

	// If the shader is in this class, then do not render
	if (!cShader)
	{
		cout << "CProjectile::Render(): The shader is not available for this class instance." << endl;
		return;
	}

	// Activate shader
	cShader->use();

	// Render the projectile

	// create transformations
	model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	//model = glm::rotate(model, (float)glfwGetTime()/10.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, vec3Position);
	model = glm::scale(model, vec3Scale);

	// note: currently we set the projection matrix each frame, but since the projection matrix 
	// rarely changes it's often best practice to set it outside the main loop only once.
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
void CProjectile::PostRender(void)
{
	glDepthFunc(GL_LESS); // set depth function back to default
}