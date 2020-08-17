/**
 CGroundTile
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "GroundTile.h"

//#include <glm/glm.hpp>
#include <includes\gtc\matrix_transform.hpp>

#include <iostream>
using namespace std;

/**
 @brief Constructor
 */
CGroundTile::CGroundTile(void)
	: vec3TileRearLeft(glm::vec3(0.0f, 0.0f, 0.0f))
	, vec3TileFrontRight(glm::vec3(0.0f, 0.0f, 0.0f))
{
}

/**
 @brief Destructor
 */
CGroundTile::~CGroundTile(void)
{
}

/**
 @brief Initialise this class instance
 @return true if the initialisation is successful, else false
 */
bool CGroundTile::Init(void)
{
	// Check if the shader is ready
	if (!cShader)
	{
		cout << "CGroundTile::Init(): The shader is not available for this class instance." << endl;
		return false;
	}

	// Call the parent's Init()
	CEntity3D::Init();

	// Set the type
	SetType(CEntity3D::TYPE::OTHERS);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = 
	{
		// positions			// texture coords
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,

		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f
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
		std::cout << "Unable to initialise Ground Tile" << std::endl;
		return false;
	}

	return true;
}

/**
 @brief Destroy the data in this class instance
 */
void CGroundTile::Destroy(void)
{
	// Delete the rendering objects in the graphics card
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

/**
 @brief Set model
 @param model A glm::mat4 variable containing the model for this class instance
 */
void CGroundTile::SetModel(glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A glm::mat4 variable containing the model for this class instance
 */
void CGroundTile::SetView(glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A glm::mat4 variable containing the model for this class instance
 */
void CGroundTile::SetProjection(glm::mat4 projection)
{
	this->projection = projection;
}

/**
 @brief ReCalculate
 */
bool CGroundTile::ReCalculate(void)
{
	vec3TileRearLeft = glm::vec3(vec3Position - vec3Scale * 0.5f);

	vec3TileFrontRight = glm::vec3(vec3Position + vec3Scale * 0.5f);

	return true;
}

/**
@brief Update this class instance
*/
void CGroundTile::Update(const double dElapsedTime)
{
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CGroundTile::PreRender(void)
{
	// Draw ground tile as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
}

/**
 @brief Render Render this instance
 @param cShader A Shader* variable which contains the Shader to use in this class instance
 */
void CGroundTile::Render(void)
{
	// If the shader is in this class, then do not render
	if (!cShader)
	{
		cout << "CGroundTile::Render(): The shader is not available for this class instance." << endl;
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
	model = glm::translate(model, glm::vec3(vec3Position.x, vec3Position.y, vec3Position.z));
	model = glm::scale(model, vec3Scale);

	// note: currently we set the projection matrix each frame, but since the projection 
	// matrix rarely changes it's often best practice to set it outside the main loop only once.
	cShader->setMat4("projection", projection);
	cShader->setMat4("view", view);
	cShader->setMat4("model", model);

	// render container
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CGroundTile::PostRender(void)
{
	glDepthFunc(GL_LESS); // set depth function back to default
}

/**
 @brief Get the exact position at a certain position where you only have the x- and z- values.
 @param vec3Position A const glm::vec3 variable which contains the position where only the x- and z- values are available or accurate.
 @return A glm::vec3 variable which store the exact position where the y- value is taken from this class
*/
glm::vec3 CGroundTile::GetExactPosition(glm::vec3 vec3Position)
{
	// If you have a terrain map / height map in this Ground Tile, 
	// Then do the interpolation to find the height at the location of vec3Position
	// Assign this height value to the y-value of vec3Position

	// For the moment, we are having a flat terrain, so the value of y is always 0
	vec3Position.y = 0.5f;
	return vec3Position;
}