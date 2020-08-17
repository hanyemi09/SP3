/**
 Player2D
 By: Toh Da Jun
 Date: Mar 2020
 */
#include "Player2D.h"

#include <iostream>
using namespace std;

// Include GLEW
#include <GL/glew.h>

// Include ImageLoader
#include "System\ImageLoader.h"

// Include the Map2D as we will use it to check the player's movements and actions
#include "Map2D.h"

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CPlayer2D::CPlayer2D(void)
	: cShader(NULL)
	, VAO(0)
	, VBO(0)
	, EBO(0)
	, iTextureID(0)
	, cMap2D(NULL)
	, cKeyboardController(NULL)
	, cSettings(NULL)
	, cSoundController(NULL)
{
	transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

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
CPlayer2D::~CPlayer2D(void)
{
	// We won't delete this since it was created elsewhere
	cSoundController = NULL;

	// We won't delete this since it was created elsewhere
	cKeyboardController = NULL;

	// We won't delete this since it was created elsewhere
	cMap2D = NULL;

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
bool CPlayer2D::Init(void)
{
	// Store the keyboard controller singleton instance here
	cKeyboardController = CKeyboardController::GetInstance();

	// Get the handler to the CSettings instance
	cSettings = CSettings::GetInstance();

	// Get the handler to the CMap2D instance
	cMap2D = CMap2D::GetInstance();

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

	// Load the player texture
	if (LoadTexture("Image/Scene2D_PlayerTile.tga", iTextureID) == false)
	{
		std::cout << "Failed to load player tile texture" << std::endl;
		return false;
	}
	cAnimatedSprites.Add(CAnimatedSprites::NEUTRAL, iTextureID);

	// Load the player texture
	if (LoadTexture("Image/Scene2D_PlayerTile_R1.tga", iTextureID) == false)
	{
		std::cout << "Failed to load player tile texture 01" << std::endl;
		return false;
	}
	cAnimatedSprites.Add(CAnimatedSprites::RIGHT, iTextureID);

	// Load the player texture
	if (LoadTexture("Image/Scene2D_PlayerTile_R2.tga", iTextureID) == false)
	{
		std::cout << "Failed to load player tile texture 02" << std::endl;
		return false;
	}
	cAnimatedSprites.Add(CAnimatedSprites::RIGHT, iTextureID);

	// Load the player texture
	if (LoadTexture("Image/Scene2D_PlayerTile_L1.tga", iTextureID) == false)
	{
		std::cout << "Failed to load player tile texture 01" << std::endl;
		return false;
	}
	cAnimatedSprites.Add(CAnimatedSprites::LEFT, iTextureID);

	// Load the player texture
	if (LoadTexture("Image/Scene2D_PlayerTile_L2.tga", iTextureID) == false)
	{
		std::cout << "Failed to load player tile texture 02" << std::endl;
		return false;
	}
	cAnimatedSprites.Add(CAnimatedSprites::LEFT, iTextureID);

	// Update the animated sprites
	iTextureID = cAnimatedSprites.Get(CAnimatedSprites::NEUTRAL);

	// Set the Physics to fall status by default
	cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);

	// Get the handler to the CSoundController
	cSoundController = CSoundController::GetInstance();

	return true;
}

/**
 @brief Update this instance
 */
void CPlayer2D::Update(const double dElapsedTime)
{
	// Get keyboard updates
	if (cKeyboardController->IsKeyDown(GLFW_KEY_LEFT))
	{
		const int iOldIndex = i32vec2Index.x;
		if (i32vec2Index.x >= 0)
		{
			i32vec2NumMicroSteps.x--;
			if (i32vec2NumMicroSteps.x < 0)
			{
				i32vec2NumMicroSteps.x = ((int)cSettings->NUM_STEPS_PER_TILE_XAXIS) - 1;
				i32vec2Index.x--;
			}
		}

		// Constraint the player's position within the screen boundary
		Constraint(LEFT);

		// Find a feasible position for the player's current position
		FindFeasiblePosition(iOldIndex, LEFT);

		// Check if player is in mid-air, such as walking off a platform
		if (IsMidAir() == true)
		{
			cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);
		}

		// Interact with the Map
		InteractWithMap();

		// Update the animated sprites
		iTextureID = cAnimatedSprites.Get(CAnimatedSprites::LEFT);
	}
	if (cKeyboardController->IsKeyDown(GLFW_KEY_RIGHT))
	{
		const int iOldIndex = i32vec2Index.x;
		if (i32vec2Index.x < (int)cSettings->NUM_TILES_XAXIS)
		{
			i32vec2NumMicroSteps.x++;

			if (i32vec2NumMicroSteps.x >= cSettings->NUM_STEPS_PER_TILE_XAXIS)
			{
				i32vec2NumMicroSteps.x = 0;
				i32vec2Index.x++;
			}
		}

		// Constraint the player's position within the screen boundary
		Constraint(RIGHT);

		// Find a feasible position for the player's current position
		FindFeasiblePosition(iOldIndex, RIGHT);
		
		// Check if player is in mid-air, such as walking off a platform
		if (IsMidAir() == true)
		{
			cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);
		}

		// Interact with the Map
		InteractWithMap();

		// Update the animated sprites
		iTextureID = cAnimatedSprites.Get(CAnimatedSprites::RIGHT);
	}
	if (cKeyboardController->IsKeyDown(GLFW_KEY_UP))
	{
		const int iOldIndex = i32vec2Index.y;

		i32vec2NumMicroSteps.y++;

		if (i32vec2NumMicroSteps.y > cSettings->NUM_STEPS_PER_TILE_YAXIS)
		{
			i32vec2NumMicroSteps.y = 0;
			i32vec2Index.y++;
		}

		// Constraint the player's position within the screen boundary
		Constraint(UP);

		// Find a feasible position for the player's current position
		FindFeasiblePosition(iOldIndex, UP);

		// Interact with the Map
		InteractWithMap();

		// Update the animated sprites
		iTextureID = cAnimatedSprites.Get(CAnimatedSprites::UP);
	}
	if (cKeyboardController->IsKeyDown(GLFW_KEY_DOWN))
	{
		const int iOldIndex = i32vec2Index.y;
		if (i32vec2Index.y >= 0)
		{
			i32vec2NumMicroSteps.y--;
			if (i32vec2NumMicroSteps.y < 0)
			{
				i32vec2NumMicroSteps.y = ((int)cSettings->NUM_STEPS_PER_TILE_YAXIS) - 1;
				i32vec2Index.y--;
			}
		}

		// Constraint the player's position within the screen boundary
		Constraint(DOWN);

		// Find a feasible position for the player's current position
		FindFeasiblePosition(iOldIndex, DOWN);

		// Interact with the Map
		InteractWithMap();

		// Update the animated sprites
		iTextureID = cAnimatedSprites.Get(CAnimatedSprites::DOWN);
	}
	if (cKeyboardController->IsKeyDown(GLFW_KEY_SPACE))
	{
		if (cPhysics2D.GetStatus() == CPhysics2D::STATUS::IDLE)
		{
			cPhysics2D.SetStatus(CPhysics2D::STATUS::JUMP);
			cPhysics2D.SetInitialVelocity(glm::vec2(0.0f, 10.0f));
		}
		cSoundController->PlaySoundByID(3);
	}

	// Update Jump or Fall
	UpdateJumpFall(dElapsedTime);

	// Update the UV Coordinates
	vec2UVCoordinate.x = cSettings->ConvertIndexToUVSpace(cSettings->x, i32vec2Index.x, false, i32vec2NumMicroSteps.x*cSettings->MICRO_STEP_XAXIS);
	vec2UVCoordinate.y = cSettings->ConvertIndexToUVSpace(cSettings->y, i32vec2Index.y, false, i32vec2NumMicroSteps.y*cSettings->MICRO_STEP_YAXIS);
}

/**
 @brief Set up the OpenGL display environment before rendering
 */
void CPlayer2D::PreRender(void)
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
void CPlayer2D::Render(void)
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
void CPlayer2D::PostRender(void)
{
	// Disable blending
	glDisable(GL_BLEND);
}

/**
 @brief Set a shader to this class instance
 @param cShader A Shader variable which stores the Shader instance
 */
void CPlayer2D::SetShader(Shader* cShader)
{
	this->cShader = cShader;
}

/**
 @brief Set the indices of the player
 @param iIndex_XAxis A const int variable which stores the index in the x-axis
 @param iIndex_YAxis A const int variable which stores the index in the y-axis
 */
void CPlayer2D::Seti32vec2Index(const int iIndex_XAxis, const int iIndex_YAxis)
{
	i32vec2Index.x = iIndex_XAxis;
	i32vec2Index.y = iIndex_YAxis;
}

/**
 @brief Set the number of microsteps of the player
 @param iNumMicroSteps_XAxis A const int variable storing the current microsteps in the X-axis
 @param iNumMicroSteps_YAxis A const int variable storing the current microsteps in the Y-axis
 */
void CPlayer2D::Seti32vec2NumMicroSteps(const int iNumMicroSteps_XAxis, const int iNumMicroSteps_YAxis)
{
	i32vec2NumMicroSteps.x = iNumMicroSteps_XAxis;
	i32vec2NumMicroSteps.y = iNumMicroSteps_YAxis;
}

/**
 @brief Set the UV coordinates of the player
 @param fUVCoordinate_XAxis A const float variable storing the UV coordinate of the player in the X-axis
 @param fUVCoordinate_YAxis A const float variable storing the UV coordinate of the player in the Y-axis
 */
void CPlayer2D::Setvec2UVCoordinates(const float fUVCoordinate_XAxis, const float fUVCoordinate_YAxis)
{
	vec2UVCoordinate.x = fUVCoordinate_XAxis;
	vec2UVCoordinate.y = fUVCoordinate_YAxis;
}

/**
 @brief Get the indices of the player
 @return The indices of the player
 */
glm::i32vec2 CPlayer2D::Geti32vec2Index(void) const
{
	return i32vec2Index;
}

/**
 @brief Get the number of microsteps of the player
 @return The number of microstep of the player
 */
glm::i32vec2 CPlayer2D::Geti32vec2NumMicroSteps(void) const
{
	return i32vec2NumMicroSteps;
}

/**
 @brief Get the UV coordinates of the player
 @return The UV coordinate of the player
 */ 
glm::vec2 CPlayer2D::Getvec2UVCoordinates(void) const
{
	return vec2UVCoordinate;
}

/**
@brief Load a texture, assign it a code and store it in MapOfTextureIDs.
@param filename A const char* variable which contains the file name of the texture
*/
bool CPlayer2D::LoadTexture(const char* filename, GLuint& iTextureID)
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

/**
 @brief Constraint the player's position within a boundary
 @param eDirection A DIRECTION enumerated data type which indicates the direction to check
 */
void CPlayer2D::Constraint(DIRECTION eDirection)
{
	if (eDirection == LEFT)
	{
		if (i32vec2Index.x < 0)
		{
			i32vec2Index.x = 0;
			i32vec2NumMicroSteps.x = 0;
		}
	}
	else if (eDirection == RIGHT)
	{
		if (i32vec2Index.x >= ((int)cSettings->NUM_TILES_XAXIS) - 1)
		{
			i32vec2Index.x = ((int)cSettings->NUM_TILES_XAXIS) - 1;
			i32vec2NumMicroSteps.x = 0;
		}
	}
	else if (eDirection == UP)
	{
		if (i32vec2Index.y >= ((int)cSettings->NUM_TILES_YAXIS) - 1)
		{
			i32vec2Index.y = ((int)cSettings->NUM_TILES_YAXIS) - 1;
			i32vec2NumMicroSteps.y = 0;
		}
	}
	else if (eDirection == DOWN)
	{
		if (i32vec2Index.y < 0)
		{
			i32vec2Index.y = 0;
			i32vec2NumMicroSteps.y = 0;
		}
	}
	else
	{
		cout << "CPlayer2D::Constraint: Unknown direction." << endl;
	}
}

/**
 @brief Find a feasible position for the player's current position
 @param iOldIndex A const int variable which stores the old index of the player.
 @param eDirection A DIRECTION enumerated data type which indicates the direction to check
 */
void CPlayer2D::FindFeasiblePosition(const int iOldIndex, DIRECTION eDirection)
{
	if (eDirection == LEFT)
	{
		// If the player within the screen boundary, then check further
		if ((i32vec2NumMicroSteps.x != 0) && (i32vec2Index.x >= 0))
		{
			// Check if this new position is not accessible, then go back to old position
			if (cMap2D->GetMapInfo(i32vec2Index.y, i32vec2Index.x) == 1)
			{
				// Snap to the previous index
				i32vec2Index.x = iOldIndex;
				// Reset the i32vec2NumMicroSteps.x so that the player is aligned to the column
				i32vec2NumMicroSteps.x = 0;
			}
			// If the new position is between 2 rows, then check the other row as well
			if (i32vec2NumMicroSteps.y != 0)
			{
				// If the new position is not feasible, then go back to the old position
				if (cMap2D->GetMapInfo(i32vec2Index.y + 1, i32vec2Index.x) == 1)
				{
					// Snap to the previous index
					i32vec2Index.x = iOldIndex;
					// Reset the i32vec2NumMicroSteps.x so that the player is aligned to the column
					i32vec2NumMicroSteps.x = 0;
				}
			}
		}
	}
	else if (eDirection == RIGHT)
	{
		// If the player within the screen boundary, then check further
		if ((i32vec2NumMicroSteps.x != 0) && (i32vec2Index.x < ((int)cSettings->NUM_TILES_XAXIS) - 1))
		{
			// Check if this new position is not accessible, then go back to old position
			if (cMap2D->GetMapInfo(i32vec2Index.y, i32vec2Index.x + 1) == 1)
			{
				// Snap to the previous index
				i32vec2Index.x = iOldIndex;
				// Reset the i32vec2NumMicroSteps.x so that the player is aligned to the column
				i32vec2NumMicroSteps.x = 0;
			}
			// If the new position is between 2 rows, then check the other row as well
			if (i32vec2NumMicroSteps.y != 0)
			{
				// If the new position is not feasible, then go back to the old position
				if (cMap2D->GetMapInfo(i32vec2Index.y + 1, i32vec2Index.x + 1) == 1)
				{
					// Snap to the previous index
					i32vec2Index.x = iOldIndex;
					// Reset the i32vec2NumMicroSteps.x so that the player is aligned to the column
					i32vec2NumMicroSteps.x = 0;
				}
			}
		}
	}
	else if (eDirection == UP)
	{
		// If the player within the screen boundary, then check further
		if ((i32vec2NumMicroSteps.y != 0) && (i32vec2Index.y < ((int)cSettings->NUM_TILES_YAXIS) - 1))
		{
			// Check if this new position is not accessible, then go back to old position
			if (cMap2D->GetMapInfo(i32vec2Index.y + 1, i32vec2Index.x) == 1)
			{
				// Snap to the previous index
				i32vec2Index.y = iOldIndex;
				// Reset the i32vec2NumMicroSteps.y so that the player is aligned to the row
				i32vec2NumMicroSteps.y = 0;
			}
			// If the new position is between 2 columns, then check the other column as well
			if (i32vec2NumMicroSteps.x != 0)
			{
				// If the new position is not feasible, then go back to the old position
				if (cMap2D->GetMapInfo(i32vec2Index.y + 1, i32vec2Index.x + 1) == 1)
				{
					// Snap to the previous index
					i32vec2Index.y = iOldIndex;
					// Reset the i32vec2NumMicroSteps.y so that the player is aligned to the row
					i32vec2NumMicroSteps.y = 0;
				}
			}
		}
	}
	else if (eDirection == DOWN)
	{
		// If the player within the screen boundary, then check further
		if ((i32vec2NumMicroSteps.y != 0) && (i32vec2Index.y >= 0))
		{
			// Check if this new position is not accessible, then go back to old position
			if (cMap2D->GetMapInfo(i32vec2Index.y, i32vec2Index.x) == 1)
			{
				// Snap to the previous index
				i32vec2Index.y = iOldIndex;
				// Reset the i32vec2NumMicroSteps.y so that the player is aligned to the row
				i32vec2NumMicroSteps.y = 0;
			}
			// If the new position is between 2 columns, then check the other column as well
			if (i32vec2NumMicroSteps.x != 0)
			{
				// If the new position is not feasible, then go back to the old position
				if (cMap2D->GetMapInfo(i32vec2Index.y, i32vec2Index.x + 1) == 1)
				{
					// Snap to the previous index
					i32vec2Index.y = iOldIndex;
					// Reset the i32vec2NumMicroSteps.y so that the player is aligned to the row
					i32vec2NumMicroSteps.y = 0;
				}
			}
		}
	}
	else
	{
		cout << "CPlayer2D::FindFeasiblePosition: Unknown direction." << endl;
	}
}

// Check if the player is in mid-air
bool CPlayer2D::IsMidAir(void)
{
	// if the player is at the bottom row, then he is not in mid-air for sure
	if (i32vec2Index.y == 0)
		return false;

	// Check if the tile below the player's current position is empty
	if ((i32vec2NumMicroSteps.x == 0) && (cMap2D->GetMapInfo(i32vec2Index.y-1, i32vec2Index.x) == 0))
	{
		return true;
	}

	return false;
}

// Update Jump or Fall
void CPlayer2D::UpdateJumpFall(const double dElapsedTime)
{
	if (cPhysics2D.GetStatus() == CPhysics2D::STATUS::JUMP)
	{
		// Update the elapsed time to the physics engine
		cPhysics2D.AddElapsedTime((float)dElapsedTime);
		// Call the physics engine update method to calculate the final velocity and displacement
		cPhysics2D.Update();
		// Get the displacement from the physics engine
		glm::vec2 v2Displacement = cPhysics2D.GetDisplacement();

		// Store the current i32vec2Index.y
		int iIndex_YAxis_OLD = i32vec2Index.y;

		// Add in the change in i32vec2Index.y
		i32vec2Index.y += (int)v2Displacement.y;
		// Add in the change in i32vec2NumMicroSteps.y
		i32vec2NumMicroSteps.y += (int)((v2Displacement.y - (long)v2Displacement.y) * cSettings->NUM_STEPS_PER_TILE_YAXIS);
		
		// Update i32vec2Index.y if i32vec2NumMicroSteps.y is more than 0
		if (i32vec2NumMicroSteps.y > 0)
		{
			i32vec2Index.y += 1;
			i32vec2NumMicroSteps.y = 0;
		}

		// Constraint the player's position within the screen boundary
		Constraint(UP);

		// if the player has hit a solid tile then stop its jump
		for (int i = iIndex_YAxis_OLD; i <= i32vec2Index.y; i++)
		{
			// If the player is within a column, then check only one column.
			// If the player is betweem two columns, then check both columns.
			if (((i32vec2NumMicroSteps.x == 0) && (cMap2D->GetMapInfo(i, i32vec2Index.x) == 1)) ||
				((i32vec2NumMicroSteps.x != 0) && ((cMap2D->GetMapInfo(i, i32vec2Index.x) == 1) ||
				(cMap2D->GetMapInfo(i, i32vec2Index.x + 1) == 1))))
			{
				// Since the player i reached a solid tile, then assign this tile index to i32vec2Index.y
				i32vec2Index.y = i - 1;
				// Since the player has stopped at a tile position, then reset i32vec2NumMicroSteps.y to 0
				i32vec2NumMicroSteps.y = 0;
				// Set the Physics to fall status
				cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);
				break;
			}
		}

		// Interact with the Map
		InteractWithMap();

		// If the player is still jumping and the initial velocity has reached zero or below zero, 
		// then it has reach the peak of its jump
		if ((cPhysics2D.GetStatus() == CPhysics2D::STATUS::JUMP) && (cPhysics2D.GetInitialVelocity().y <= 0.0f))
		{
			// Set status to fall
			cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);
		}
	}
	else if (cPhysics2D.GetStatus() == CPhysics2D::STATUS::FALL)
	{
		// Update the elapsed time to the physics engine
		cPhysics2D.AddElapsedTime(0.1f);
		// Call the physics engine update method to calculate the final velocity and displacement
		cPhysics2D.Update();
		// Get the displacement from the physics engine
		glm::vec2 v2Displacement = cPhysics2D.GetDisplacement();

		// Store the current i32vec2Index.y
		int iIndex_YAxis_OLD = i32vec2Index.y;

		// Add in the change in i32vec2Index.y
		i32vec2Index.y += (int)v2Displacement.y;
		// Add in the change in i32vec2NumMicroSteps.y
		i32vec2NumMicroSteps.y -= (int)((v2Displacement.y - (long)v2Displacement.y) * cSettings->NUM_STEPS_PER_TILE_YAXIS);

		// Update i32vec2Index.y if i32vec2NumMicroSteps.y is more than 0
		if (i32vec2NumMicroSteps.y > 0)
		{
			i32vec2Index.y -= 1;
			i32vec2NumMicroSteps.y = 0;
		}

		// Constraint the player's position within the screen boundary
		Constraint(DOWN);

		// if the player has landed on a solid tile then stop its fall
		for (int i = iIndex_YAxis_OLD; i >= i32vec2Index.y; i--)
		{
			// If the player is within a column, then check only one column.
			// If the player is betweem two columns, then check both columns.
			if (((i32vec2NumMicroSteps.x == 0) && (cMap2D->GetMapInfo(i, i32vec2Index.x) == 1)) ||
				((i32vec2NumMicroSteps.x != 0) && ((cMap2D->GetMapInfo(i, i32vec2Index.x) == 1) ||
				(cMap2D->GetMapInfo(i, i32vec2Index.x + 1) == 1))))
			{
				// Since the player reached a solid tile, then assign this tile index to i32vec2Index.y
				i32vec2Index.y = i + 1;
				// Since the player has stopped at a tile position, then reset i32vec2NumMicroSteps.y to 0
				i32vec2NumMicroSteps.y = 0;
				// Set the Physics to idle status
				cPhysics2D.SetStatus(CPhysics2D::STATUS::IDLE);
				break;
			}
		}

		// Interact with the Map
		InteractWithMap();
	}
}

/**
 @brief Let player interact with the map. You can add collectibles such as powerups and health here.
 */
void CPlayer2D::InteractWithMap(void)
{
	switch (cMap2D->GetMapInfo(i32vec2Index.y, i32vec2Index.x))
	{
	case 2:
		// Erase the tree from this position
		cMap2D->SetMapInfo(i32vec2Index.y, i32vec2Index.x, 0);
		// Play a bell sound
		cSoundController->PlaySoundByID(1);
		break;
	default:
		break;
	}
}
