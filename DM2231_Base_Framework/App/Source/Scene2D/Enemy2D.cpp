/**
 CEnemy2D
 By: Toh Da Jun
 Date: Mar 2020
 */
#include "Enemy2D.h"

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
CEnemy2D::CEnemy2D(void)
	: //cShader(NULL)
	//, VAO(0)
	//, VBO(0)
	//, EBO(0)
	//, iTextureID(0)
	cMap2D(NULL)
	//, cSettings(NULL)
	, cPlayer2D(NULL)
	, sCurrentFSM(FSM::IDLE)
	, iFSMCounter(0)
	, dTotalElapsedTime(0.0)
{
	transform = glm::mat4(1.0f);	// make sure to initialize matrix to identity matrix first

	// Initialise vecIndex
	i32vec2Index = glm::i32vec2(0);

	// Initialise vecNumMicroSteps
	i32vec2NumMicroSteps = glm::i32vec2(0);

	// Initialise vec2UVCoordinate
	vec2UVCoordinate = glm::vec2(0.0f);

	i32vec2Destination = glm::i32vec2(0, 0);	// Initialise the iDestination
	i32vec2Direction = glm::i32vec2(0, 0);		// Initialise the iDirection
}

/**
 @brief Destructor This destructor has protected access modifier as this class will be a Singleton
 */
CEnemy2D::~CEnemy2D(void)
{
	// We won't delete this since it was created elsewhere
	cPlayer2D = NULL;

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
bool CEnemy2D::Init(void)
{
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

	// Load the enemy2D texture
	if (LoadTexture("Image/Scene2D_EnemyTile.tga") == false)
	{
		std::cout << "Failed to load enemy2D tile texture" << std::endl;
		return false;
	}

	// Set the Physics to fall status by default
	cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);

	return true;
}

/**
 @brief Update this instance
 */
void CEnemy2D::Update(const double dElapsedTime)
{
	// Real-Time Loop Control
	// Update the total elapsed time since last update
	dTotalElapsedTime += dElapsedTime;
	// Check if the total elapsed time is more than the minimum elapsed time
	if (dTotalElapsedTime < dMinimumElapsedTime)
	{
		// Quit this update if total elapsed time is less than the minimum elapsed time
		return;
	}

	// Reset the total elapsed time to 0 and continue with this update
	dTotalElapsedTime = 0.0;

	switch (sCurrentFSM)
	{
	case IDLE:
#ifdef _DEBUGMODE
		cout << "IDLE : " << iFSMCounter << endl;
#endif
		if (iFSMCounter > iMaxFSMCounter)
		{
			sCurrentFSM = PATROL;
			iFSMCounter = 0;
		}
		iFSMCounter++;
		break;
	case PATROL:
#ifdef _DEBUGMODE
		cout << "PATROL : " << iFSMCounter << endl;
#endif
		if (iFSMCounter > iMaxFSMCounter)
		{
			sCurrentFSM = IDLE;
			iFSMCounter = 0;
		}
		else if (cPhysics2D.CalculateDistance(i32vec2Index, cPlayer2D->Geti32vec2Index()) < 5.0f)
		{
			sCurrentFSM = ATTACK;
			iFSMCounter = 0;
		}
		else
		{
			// Patrol around
			// Update the Enemy2D's position for attack
			UpdatePosition();
		}
		iFSMCounter++;
		break;
	case ATTACK:
#ifdef _DEBUGMODE
		cout << "ATTACK : " << iFSMCounter << endl;
#endif
		if (cPhysics2D.CalculateDistance(i32vec2Index, cPlayer2D->Geti32vec2Index()) < 5.0f)
		{
			// Attack
			// Update direction to move towards for attack
			UpdateDirection();

			// Update the Enemy2D's position for attack
			UpdatePosition();
		}
		else
		{
#ifdef _DEBUGMODE
			cout << "ATTACK : Distance " << cPhysics2D.CalculateDistance(i32vec2Index, cPlayer2D->Geti32vec2Index()) << endl;
#endif

			if (iFSMCounter > iMaxFSMCounter)
			{
				sCurrentFSM = PATROL;
				iFSMCounter = 0;
				cout << "ATTACK : Reset counter: " << iFSMCounter << endl;
			}
			iFSMCounter++;
		}
		break;
	}

	// Update Jump or Fall
	UpdateJumpFall(dElapsedTime);

	// Update the UV Coordinates
	vec2UVCoordinate.x = cSettings->ConvertIndexToUVSpace(cSettings->x, i32vec2Index.x, false, i32vec2NumMicroSteps.x*cSettings->MICRO_STEP_XAXIS);
	vec2UVCoordinate.y = cSettings->ConvertIndexToUVSpace(cSettings->y, i32vec2Index.y, false, i32vec2NumMicroSteps.y*cSettings->MICRO_STEP_YAXIS);
}

///**
// @brief Set up the OpenGL display environment before rendering
// */
//void CEnemy2D::PreRender(void)
//{
//	// bind textures on corresponding texture units
//	glActiveTexture(GL_TEXTURE0);
//
//	// Activate blending mode
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	// Activate the shader
//	cShader->use();
//}

/**
 @brief Render this instance
 */
void CEnemy2D::Render(void)
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

///**
// @brief PostRender Set up the OpenGL display environment after rendering.
// */
//void CEnemy2D::PostRender(void)
//{
//	// Disable blending
//	glDisable(GL_BLEND);
//}

///**
// @brief Set a shader to this class instance
// @param cShader A Shader variable which stores the Shader instance
// */
//void CEnemy2D::SetShader(Shader* cShader)
//{
//	this->cShader = cShader;
//}

/**
 @brief Set the handle to cPlayer to this class instance
 @param cPlayer2D A CPlayer2D* variable which contains the pointer to the CPlayer2D instance
 */
void CEnemy2D::SetPlayer2D(CPlayer2D* cPlayer2D)
{
	this->cPlayer2D = cPlayer2D;

	// Update the enemy's direction
	UpdateDirection();
}


///**
//@brief Load a texture, assign it a code and store it in MapOfTextureIDs.
//@param filename A const char* variable which contains the file name of the texture
//*/
//bool CEnemy2D::LoadTexture(const char* filename)
//{
//	// Variables used in loading the texture
//	int width, height, nrChannels;
//	
//	// texture 1
//	// ---------
//	glGenTextures(1, &iTextureID);
//	glBindTexture(GL_TEXTURE_2D, iTextureID);
//	// set the texture wrapping parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	// set texture filtering parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//	CImageLoader* cImageLoader = CImageLoader::GetInstance();
//	unsigned char *data = cImageLoader->Load(filename, width, height, nrChannels, true);
//	if (data)
//	{
//		if (nrChannels == 3)
//			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//		else if (nrChannels == 4)
//			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//
//		// Generate mipmaps
//		glGenerateMipmap(GL_TEXTURE_2D);
//	}
//	else
//	{
//		return false;
//	}
//	// Free up the memory of the file data read in
//	free(data);
//
//	return true;
//}

/**
 @brief Constraint the enemy2D's position within a boundary
 @param eDirection A DIRECTION enumerated data type which indicates the direction to check
 */
void CEnemy2D::Constraint(DIRECTION eDirection)
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
		cout << "CEnemy2D::Constraint: Unknown direction." << endl;
	}
}

/**
 @brief Find a feasible position for the enemy2D's current position
 @param iOldIndex A const int variable which stores the old index of the enemy2D.
 @param eDirection A DIRECTION enumerated data type which indicates the direction to check
 @return true if the enemy2D movement was not possible and snapped to a feasible position
 */
bool CEnemy2D::FindFeasiblePosition(const int iOldIndex, DIRECTION eDirection)
{
	if (eDirection == LEFT)
	{
		// If the enemy2D within the screen boundary, then check further
		if ((i32vec2NumMicroSteps.x != 0) && (i32vec2Index.x >= 0))
		{
			// Check if this new position is not accessible, then go back to old position
			if (cMap2D->GetMapInfo(i32vec2Index.y, i32vec2Index.x) == 1)
			{
				// Snap to the previous index
				i32vec2Index.x = iOldIndex;
				// Reset the i32vec2NumMicroSteps.x so that the enemy2D is aligned to the column
				i32vec2NumMicroSteps.x = 0;
				return true;
			}
			// If the new position is between 2 rows, then check the other row as well
			if (i32vec2NumMicroSteps.y != 0)
			{
				// If the new position is not feasible, then go back to the old position
				if (cMap2D->GetMapInfo(i32vec2Index.y + 1, i32vec2Index.x) == 1)
				{
					// Snap to the previous index
					i32vec2Index.x = iOldIndex;
					// Reset the i32vec2NumMicroSteps.x so that the enemy2D is aligned to the column
					i32vec2NumMicroSteps.x = 0;
					return true;
				}
			}
		}
	}
	else if (eDirection == RIGHT)
	{
		// If the enemy2D within the screen boundary, then check further
		if ((i32vec2NumMicroSteps.x != 0) && (i32vec2Index.x < ((int)cSettings->NUM_TILES_XAXIS) - 1))
		{
			// Check if this new position is not accessible, then go back to old position
			if (cMap2D->GetMapInfo(i32vec2Index.y, i32vec2Index.x + 1) == 1)
			{
				// Snap to the previous index
				i32vec2Index.x = iOldIndex;
				// Reset the i32vec2NumMicroSteps.x so that the enemy2D is aligned to the column
				i32vec2NumMicroSteps.x = 0;
				return true;
			}
			// If the new position is between 2 rows, then check the other row as well
			if (i32vec2NumMicroSteps.y != 0)
			{
				// If the new position is not feasible, then go back to the old position
				if (cMap2D->GetMapInfo(i32vec2Index.y + 1, i32vec2Index.x + 1) == 1)
				{
					// Snap to the previous index
					i32vec2Index.x = iOldIndex;
					// Reset the i32vec2NumMicroSteps.x so that the enemy2D is aligned to the column
					i32vec2NumMicroSteps.x = 0;
					return true;
				}
			}
		}
	}
	else if (eDirection == UP)
	{
		// If the enemy2D within the screen boundary, then check further
		if ((i32vec2NumMicroSteps.y != 0) && (i32vec2Index.y < ((int)cSettings->NUM_TILES_YAXIS) - 1))
		{
			// Check if this new position is not accessible, then go back to old position
			if (cMap2D->GetMapInfo(i32vec2Index.y + 1, i32vec2Index.x) == 1)
			{
				// Snap to the previous index
				i32vec2Index.y = iOldIndex;
				// Reset the i32vec2NumMicroSteps.y so that the enemy2D is aligned to the row
				i32vec2NumMicroSteps.y = 0;
				return true;
			}
			// If the new position is between 2 columns, then check the other column as well
			if (i32vec2NumMicroSteps.x != 0)
			{
				// If the new position is not feasible, then go back to the old position
				if (cMap2D->GetMapInfo(i32vec2Index.y + 1, i32vec2Index.x + 1) == 1)
				{
					// Snap to the previous index
					i32vec2Index.y = iOldIndex;
					// Reset the i32vec2NumMicroSteps.y so that the enemy2D is aligned to the row
					i32vec2NumMicroSteps.y = 0;
					return true;
				}
			}
		}
	}
	else if (eDirection == DOWN)
	{
		// If the enemy2D within the screen boundary, then check further
		if ((i32vec2NumMicroSteps.y != 0) && (i32vec2Index.y >= 0))
		{
			// Check if this new position is not accessible, then go back to old position
			if (cMap2D->GetMapInfo(i32vec2Index.y, i32vec2Index.x) == 1)
			{
				// Snap to the previous index
				i32vec2Index.y = iOldIndex;
				// Reset the i32vec2NumMicroSteps.y so that the enemy2D is aligned to the row
				i32vec2NumMicroSteps.y = 0;
				return true;
			}
			// If the new position is between 2 columns, then check the other column as well
			if (i32vec2NumMicroSteps.x != 0)
			{
				// If the new position is not feasible, then go back to the old position
				if (cMap2D->GetMapInfo(i32vec2Index.y, i32vec2Index.x + 1) == 1)
				{
					// Snap to the previous index
					i32vec2Index.y = iOldIndex;
					// Reset the i32vec2NumMicroSteps.y so that the enemy2D is aligned to the row
					i32vec2NumMicroSteps.y = 0;
					return true;
				}
			}
		}
	}
	else
	{
		cout << "CEnemy2D::FindFeasiblePosition: Unknown direction." << endl;
	}
	return false;
}

// Check if the enemy2D is in mid-air
bool CEnemy2D::IsMidAir(void)
{
	// if the enemy2D is at the bottom row, then he is not in mid-air for sure
	if (i32vec2Index.y == 0)
		return false;

	// Check if the tile below the enemy2D's current position is empty
	if ((i32vec2NumMicroSteps.x == 0) && (cMap2D->GetMapInfo(i32vec2Index.y-1, i32vec2Index.x) == 0))
	{
		return true;
	}

	return false;
}

// Update Jump or Fall
void CEnemy2D::UpdateJumpFall(const double dElapsedTime)
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

		// Constraint the enemy2D's position within the screen boundary
		Constraint(UP);

		// if the enemy2D has hit a solid tile then stop its jump
		for (int i = iIndex_YAxis_OLD; i <= i32vec2Index.y; i++)
		{
			// If the enemy2D is within a column, then check only one column.
			// If the enemy2D is betweem two columns, then check both columns.
			if (((i32vec2NumMicroSteps.x == 0) && (cMap2D->GetMapInfo(i, i32vec2Index.x) == 1)) ||
				((i32vec2NumMicroSteps.x != 0) && ((cMap2D->GetMapInfo(i, i32vec2Index.x) == 1) ||
				(cMap2D->GetMapInfo(i, i32vec2Index.x + 1) == 1))))
			{
				// Since the enemy2D i reached a solid tile, then assign this tile index to i32vec2Index.y
				i32vec2Index.y = i - 1;
				// Since the enemy2D has stopped at a tile position, then reset i32vec2NumMicroSteps.y to 0
				i32vec2NumMicroSteps.y = 0;
				// Set the Physics to fall status
				cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);
				break;
			}
		}

		// Interact with the Player. Fall down after collision
		if (InteractWithPlayer())
		{
			// Set status to fall
			cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);
		}

		// If the enemy2D is still jumping and the initial velocity has reached zero or below zero, 
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

		// Constraint the enemy2D's position within the screen boundary
		Constraint(DOWN);

		// if the enemy2D has landed on a solid tile then stop its fall
		for (int i = iIndex_YAxis_OLD; i >= i32vec2Index.y; i--)
		{
			// If the enemy2D is within a column, then check only one column.
			// If the enemy2D is betweem two columns, then check both columns.
			if (((i32vec2NumMicroSteps.x == 0) && (cMap2D->GetMapInfo(i, i32vec2Index.x) == 1)) ||
				((i32vec2NumMicroSteps.x != 0) && ((cMap2D->GetMapInfo(i, i32vec2Index.x) == 1) ||
				(cMap2D->GetMapInfo(i, i32vec2Index.x + 1) == 1))))
			{
				// Since the enemy2D reached a solid tile, then assign this tile index to i32vec2Index.y
				i32vec2Index.y = i + 1;
				// Since the enemy2D has stopped at a tile position, then reset i32vec2NumMicroSteps.y to 0
				i32vec2NumMicroSteps.y = 0;
				// Set the Physics to idle status
				cPhysics2D.SetStatus(CPhysics2D::STATUS::IDLE);
				break;
			}
		}

		// Interact with the Player. Fall down after collision
		if (InteractWithPlayer())
		{
			// Set status to fall
			cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);
		}
	}
}

/**
 @brief Let enemy2D interact with the player.
 */
bool CEnemy2D::InteractWithPlayer(void)
{
	glm::i32vec2 i32vec2PlayerPos = cPlayer2D->Geti32vec2Index();
	
	// Check if the enemy2D is within 1.5 indices of the player2D
	if (((i32vec2Index.x >= i32vec2PlayerPos.x - 0.5) && 
		(i32vec2Index.x <= i32vec2PlayerPos.x + 0.5))
		&& 
		((i32vec2Index.y >= i32vec2PlayerPos.y - 0.5) &&
		(i32vec2Index.y <= i32vec2PlayerPos.y + 0.5)))
	{
		cout << "Gotcha!" << endl;
		// Since the player has been caught, then reset the FSM
		sCurrentFSM = IDLE;
		iFSMCounter = 0;
		return true;
	}
	return false;
}

#include <math.h>

/**
 @brief Update the enemy's direction.
 */
void CEnemy2D::UpdateDirection(void)
{
	// Set the destination to the player
	i32vec2Destination = cPlayer2D->Geti32vec2Index();

	// Calculate the direction between enemy2D and player2D
	i32vec2Direction = i32vec2Destination - i32vec2Index;

	// Calculate the distance between enemy2D and player2D
	float fDistance = cPhysics2D.CalculateDistance(i32vec2Index, i32vec2Destination);
	if (fDistance >= 0.01f)
	{
		// Calculate direction vector.
		// We need to round the numbers as it is easier to work with whole numbers for movements
		i32vec2Direction.x = (int)round(i32vec2Direction.x / fDistance);
		i32vec2Direction.y = (int)round(i32vec2Direction.y / fDistance);
	}
	else
	{
		// Since we are not going anywhere, set this to 0.
		i32vec2Direction = glm::i32vec2(0);
	}
}

/**
 @brief Flip horizontal direction. For patrol use only
 */
void CEnemy2D::FlipHorizontalDirection(void)
{
	i32vec2Direction.x *= -1;
}

/**
@brief Update position.
*/
void CEnemy2D::UpdatePosition(void)
{
	// if the player is to the left or right of the enemy2D, then jump to attack
	if (i32vec2Direction.x < 0)
	{
		// Move left
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

		// Constraint the enemy2D's position within the screen boundary
		Constraint(LEFT);

		// Find a feasible position for the enemy2D's current position
		if (FindFeasiblePosition(iOldIndex, LEFT) == true)
			FlipHorizontalDirection();

		// Check if enemy2D is in mid-air, such as walking off a platform
		if (IsMidAir() == true)
		{
			cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);
		}

		// Interact with the Map
		InteractWithPlayer();
	}
	else if (i32vec2Direction.x > 0)
	{
		// Move right
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

		// Constraint the enemy2D's position within the screen boundary
		Constraint(RIGHT);

		// Find a feasible position for the enemy2D's current position
		if (FindFeasiblePosition(iOldIndex, RIGHT) == true)
			FlipHorizontalDirection();

		// Check if enemy2D is in mid-air, such as walking off a platform
		if (IsMidAir() == true)
		{
			cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);
		}

		// Interact with the Map
		InteractWithPlayer();
	}

	// if the player is above the enemy2D, then jump to attack
	if (i32vec2Direction.y > 0)
	{
		if (cPhysics2D.GetStatus() == CPhysics2D::STATUS::IDLE)
		{
			cPhysics2D.SetStatus(CPhysics2D::STATUS::JUMP);
			cPhysics2D.SetInitialVelocity(glm::vec2(0.0f, 12.0f));
		}
	}

}
