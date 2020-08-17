/**
 CEntity2D
 By: Toh Da Jun
 Date: Mar 2020
 */
#include "Entity2D.h"

#include <iostream>
using namespace std;

// Include GLEW
#include <GL/glew.h>

// Include ImageLoader
#include "System\ImageLoader.h"

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CEntity2D::CEntity2D(void)
	: cShader(NULL)
	, VAO(0)
	, VBO(0)
	, EBO(0)
	, iTextureID(0)
	, cSettings(NULL)
{
	transform = glm::mat4(1.0f);	// make sure to initialize matrix to identity matrix first

	// Initialise vecIndex
	i32vec2Index = glm::i32vec2(0);

	// Initialise vecNumMicroSteps
	i32vec2NumMicroSteps = glm::i32vec2(0);

	// Initialise vec2UVCoordinate
	vec2UVCoordinate = glm::vec2(0.0f);
}

/**
 @brief Destructor This destructor has protected access modifier as this class will be a Singleton
 */
CEntity2D::~CEntity2D(void)
{
	// We won't delete this since it was created elsewhere
	cShader = NULL;

	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

/**
  @brief Initialise this instance
  */
bool CEntity2D::Init(void)
{
	// Get the handler to the CSettings instance
	cSettings = CSettings::GetInstance();

	// set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {	// positions          // texture coords
		(cSettings->TILE_WIDTH / 2.0f), (cSettings->TILE_HEIGHT / 2.0f), 0.0f, 1.0f, 1.0f, // top right
		(cSettings->TILE_WIDTH / 2.0f), -(cSettings->TILE_HEIGHT / 2.0f), 0.0f, 1.0f, 0.0f, // bottom right
		-(cSettings->TILE_WIDTH / 2.0f), -(cSettings->TILE_HEIGHT / 2.0f), 0.0f, 0.0f, 0.0f, // bottom left
		-(cSettings->TILE_WIDTH / 2.0f), (cSettings->TILE_HEIGHT / 2.0f), 0.0f, 0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = { 0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Load the enemy2D texture
	if (LoadTexture("Image/Scene2D_EnemyTile.tga") == false)
	{
		std::cout << "Failed to load enemy2D tile texture" << std::endl;
		return false;
	}

	return true;
}

/**
 @brief Update this instance
 */
void CEntity2D::Update(const double dElapsedTime)
{
}

/**
 @brief Set up the OpenGL display environment before rendering
 */
void CEntity2D::PreRender(void)
{
	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);

	// Activate blending mode
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Activate the shader
	cShader->use();
}

/**
 @brief Render this instance
 */
void CEntity2D::Render(void)
{
	// get matrix's uniform location and set matrix
	unsigned int transformLoc = glGetUniformLocation(cShader->ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	transform = glm::translate(transform, glm::vec3(vec2UVCoordinate.x,
													vec2UVCoordinate.y,
													0.0f));
	// Update the shaders with the latest transform
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	// Get the texture to be rendered
	glBindTexture(GL_TEXTURE_2D, iTextureID);

	glBindVertexArray(VAO);
	// Render the tile
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CEntity2D::PostRender(void)
{
	// Disable blending
	glDisable(GL_BLEND);
}

/**
 @brief Set a shader to this class instance
 @param cShader A Shader variable which stores the Shader instance
 */
void CEntity2D::SetShader(Shader* cShader)
{
	this->cShader = cShader;
}


/**
@brief Set the indices of the enemy2D
@param iIndex_XAxis A const int variable which stores the index in the x-axis
@param iIndex_YAxis A const int variable which stores the index in the y-axis
*/
void CEntity2D::Seti32vec2Index(const int iIndex_XAxis, const int iIndex_YAxis)
{
	this->i32vec2Index.x = iIndex_XAxis;
	this->i32vec2Index.y = iIndex_YAxis;
}

/**
@brief Set the number of microsteps of the enemy2D
@param iNumMicroSteps_XAxis A const int variable storing the current microsteps in the X-axis
@param iNumMicroSteps_YAxis A const int variable storing the current microsteps in the Y-axis
*/
void CEntity2D::Seti32vec2NumMicroSteps(const int iNumMicroSteps_XAxis, const int iNumMicroSteps_YAxis)
{
	this->i32vec2NumMicroSteps.x = iNumMicroSteps_XAxis;
	this->i32vec2NumMicroSteps.y = iNumMicroSteps_YAxis;
}

/**
@brief Set the UV coordinates of the enemy2D
@param fUVCoordinate_XAxis A const float variable storing the UV coordinate of the enemy2D in the X-axis
@param fUVCoordinate_YAxis A const float variable storing the UV coordinate of the enemy2D in the Y-axis
*/
void CEntity2D::Setvec2UVCoordinates(const float fUVCoordinate_XAxis, const float fUVCoordinate_YAxis)
{
	this->vec2UVCoordinate.x = fUVCoordinate_XAxis;
	this->vec2UVCoordinate.y = fUVCoordinate_YAxis;
}

/**
@brief Get the indices of the enemy2D
@return The indices of the enemy2D
*/
glm::i32vec2 CEntity2D::Geti32vec2Index(void) const
{
	return i32vec2Index;
}

/**
@brief Get the number of microsteps of the enemy2D
@return The number of microstep of the enemy2D
*/
glm::i32vec2 CEntity2D::Geti32vec2NumMicroSteps(void) const
{
	return i32vec2NumMicroSteps;
}

/**
@brief Get the UV coordinates of the enemy2D
@return The UV coordinate of the enemy2D
*/
glm::vec2 CEntity2D::Getvec2UVCoordinates(void) const
{
	return vec2UVCoordinate;
}

/**
@brief Load a texture, assign it a code and store it in MapOfTextureIDs.
@param filename A const char* variable which contains the file name of the texture
*/
bool CEntity2D::LoadTexture(const char* filename)
{
	// Variables used in loading the texture
	int width, height, nrChannels;
	
	// texture 1
	// ---------
	glGenTextures(1, &iTextureID);
	glBindTexture(GL_TEXTURE_2D, iTextureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	CImageLoader* cImageLoader = CImageLoader::GetInstance();
	unsigned char *data = cImageLoader->Load(filename, width, height, nrChannels, true);
	if (data)
	{
		if (nrChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else if (nrChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		// Generate mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		return false;
	}
	// Free up the memory of the file data read in
	free(data);

	return true;
}
