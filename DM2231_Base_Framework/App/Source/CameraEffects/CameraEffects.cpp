#include "CameraEffects.h"
#include "GL\glew.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CCameraEffects::CCameraEffects(void)
	: bStatus_BloodScreen(false)
	, fCountdownTimer_BloodScreen(0.1f)
{
}

/**
 @brief Destructor
 */
CCameraEffects::~CCameraEffects(void)
{
	// Delete the rendering objects in the graphics card
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// We set this to NULL, since it was created elsewhere so we don't delete it here
	cShader = NULL;
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CCameraEffects::Init(void)
{
	// Check if the shader is ready
	if (!cShader)
	{
		cout << "CCameraEffects::Init(): The shader is not available for this class instance." << endl;
		return false;
	}

	// Call the parent's Init()
	CEntity3D::Init();

	// Set the type
	SetType(CEntity3D::TYPE::OTHERS);

	transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

	// set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {
		// positions			// texture coords
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,

		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f
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

	if ((VAO == 0) || (VBO == 0))
	{
		std::cout << "Unable to initialise CameraEffect" << std::endl;
		return false;
	}

	// load and create a texture 
	iTextureID = LoadTexture("Image/GUI/CameraEffects_Blood.tga");
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/GUI/CameraEffects_Blood.tga" << endl;
		return false;
	}

	return true;
}

/**
 @brief Set model
 @param model A glm::mat4 variable containing the model for this class instance
 */
void CCameraEffects::SetModel(glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A glm::mat4 variable containing the model for this class instance
 */
void CCameraEffects::SetView(glm::mat4 view)
{
	this->view = glm::mat4(glm::mat3(view)); // remove translation from the view matrix
}

/**
 @brief Set projection
 @param projection A glm::mat4 variable containing the model for this class instance
 */
void CCameraEffects::SetProjection(glm::mat4 projection)
{
	this->projection = projection;
}

/** 
 @briefActivate BloodScreen
 */
void CCameraEffects::Activate_BloodScreen(void)
{
	bStatus_BloodScreen = true;
}

/** 
 @briefSet Countdown Timer for BloodScreen
 @param fCountdownTimer_BloodScreen A const float variable containing the new countdown timer for the blood screen
 */
bool CCameraEffects::SetTimer_BloodScreen(const float fCountdownTimer_BloodScreen)
{
	this->fCountdownTimer_BloodScreen = fCountdownTimer_BloodScreen;
	return true;
}

/** 
 @briefGet Countdown Timer for BloodScreen
 */
float CCameraEffects::GetTimer_BloodScreen(void) const
{
	return fCountdownTimer_BloodScreen;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CCameraEffects::Update(const double dElapsedTime)
{
	if (bStatus_BloodScreen)
	{
		fCountdownTimer_BloodScreen -= (float)dElapsedTime;
		if (fCountdownTimer_BloodScreen <= 0.0f)
		{
			// Set the bool flag for Render BloodScreen to false
			bStatus_BloodScreen = false;
			// Reset the countdown timer to the default value
			fCountdownTimer_BloodScreen = 2.0f;
		}
	}
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CCameraEffects::PreRender(void)
{
	if (!bStatus_BloodScreen)
		return;

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);

	// Activate blending mode
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/**
 @brief Render Render this instance
 @param cShader A Shader* variable which contains the Shader to use in this class instance
 */
void CCameraEffects::Render(void)
{
	// If the shader is in this class, then do not render
	if (!cShader)
	{
		cout << "CCameraEffects::Render(): The shader is not available for this class instance." << endl;
		return;
	}

	// Activate shader
	cShader->use();

	if (!bStatus_BloodScreen)
		return;

	unsigned int transformLoc;
	// get matrix's uniform location and set matrix
	transformLoc = glGetUniformLocation(cShader->ID, "transform");

	// Update the shaders with the latest transform
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	// Get the texture to be rendered
	glBindTexture(GL_TEXTURE_2D, iTextureID);

	// Render the quad
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Reset to default
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CCameraEffects::PostRender(void)
{
	if (!bStatus_BloodScreen)
		return;

	// Disable blending
	glDisable(GL_BLEND);
}
