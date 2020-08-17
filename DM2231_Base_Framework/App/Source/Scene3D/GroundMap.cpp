/**
 CGroundMap
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "GroundMap.h"

//#include <glm/glm.hpp>
#include <includes\gtc\matrix_transform.hpp>

#include <iostream>
// Include ImageLoader
#include "System\ImageLoader.h"

// Constructor
CGroundMap::CGroundMap(void)
	: cShader(NULL)
	, cSettings(NULL)
	, iTextureID_01(0)
	, iTextureID_02(0)
	, model(glm::mat4(1.0f))
	, view(glm::mat4(1.0f))
	, projection(glm::mat4(1.0f))
	, vec3MapSize(glm::vec3(1.0f))
	, vec3TileSize(glm::vec3(1.0f))
	, i32vec3NumTiles(glm::vec3(1))
{
}

// Destructor
CGroundMap::~CGroundMap(void)
{
	// Destroy all the Ground Tiles
	for (int iRow = 0; iRow < i32vec3NumTiles.x; iRow++)
	{
		for (int iCol = 0; iCol < i32vec3NumTiles.z; iCol++)
		{
			vGroundTiles[iRow][iCol].Destroy();
		}
	}

	// We set this to NULL, since it was created elsewhere so we don't delete it here
	cShader = NULL;
	// We set this to NULL, since it was created elsewhere so we don't delete it here
	cSettings = NULL;
}

// Initialise this class instance
bool CGroundMap::Init(glm::vec3 vec3MapSize, glm::i32vec3 i32vec3NumTiles)
{
	// Get the handler to the CSettings Singleton
	cSettings = CSettings::GetInstance();

	// Load texture for dark green grass
	if (LoadTexture("Image/structure.tga", iTextureID_01) == false)
	{
		std::cout << "Failed to load Scene3D_Grass_DarkGreen.tga" << std::endl;
		return false;
	}
	// Load texture for light green grass
	if (LoadTexture("Image/structure.tga", iTextureID_02) == false)
	{
		std::cout << "Failed to load Scene3D_Grass_LightGreen.tga" << std::endl;
		return false;
	}

	// Set the map size
	SetMapSize(vec3MapSize);
	// Set the number of tiles
	SetNumTiles(i32vec3NumTiles);

	return true;
}


/**
 @brief Set model
 @param model A glm::mat4 variable containing the model for this class instance
 */
void CGroundMap::SetModel(glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A glm::mat4 variable containing the model for this class instance
 */
void CGroundMap::SetView(glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A glm::mat4 variable containing the model for this class instance
 */
void CGroundMap::SetProjection(glm::mat4 projection)
{
	this->projection = projection;
}

/**
 @brief Set a shader to this class instance
 @param cShader A Shader pointer variable which contains the shader for this class instance
 */
void CGroundMap::SetShader(Shader* cShader)
{
	this->cShader = cShader;
}

/**
 @brief Set map size
 @return true is successfully if the map size was update, else false
 */
bool CGroundMap::SetMapSize(glm::vec3 vec3MapSize)
{
	this->vec3MapSize = vec3MapSize;

	return true;
}

/**
 @brief Set num tiles
 @return true is successfully if the num tiles was update, else false
 */
bool CGroundMap::SetNumTiles(glm::i32vec3 i32vec3NumTiles)
{
	this->i32vec3NumTiles = i32vec3NumTiles;

	return true;
}

/**
@brief ReCalculate the map details. Call this once after setting the parameters
@return true is the update is successful
*/
bool CGroundMap::ReCalculate(void)
{
	// Update the size of each tile in X-, Z-axes
	vec3TileSize = vec3MapSize / glm::vec3(i32vec3NumTiles);

	// The coordinate of the rear-left corner of the map in X-, Z-axes
	vec3MapRearLeft = -(vec3MapSize / 2.0f);
	vec3MapRearLeft.y = 0.0f;
	// The coordinate of the front-right corner of the map in X-, Z-axes
	vec3MapFrontRight = (vec3MapSize / 2.0f);
	vec3MapFrontRight.y = 0.0f;

	int iTextureCounter = 0;
	// Update the vGroundTiles vector of vectors
	for (int iRow = 0; iRow < i32vec3NumTiles.x; iRow++)
	{
		// Construct a vector of int
		vector<CGroundTile> vColumnsOfValues;
		for (int iCol = 0; iCol < i32vec3NumTiles.z; iCol++)
		{
			CGroundTile cGroundTile;
			// Set a shader to this class instance of CSkyBox
			cGroundTile.SetShader(cShader);
			if (cGroundTile.Init() == false)
				return false;

			// Set the texture IDs for the ground tiles
			// We have a grid design so it will be easier for us to see entities moving on the ground
			if (iRow % 2 == 0)
			{
				if (iTextureCounter % 2 == 0)
					cGroundTile.SetTextureID(iTextureID_01);
				else
					cGroundTile.SetTextureID(iTextureID_02);
			}
			else
			{
				if (iTextureCounter % 2 == 0)
					cGroundTile.SetTextureID(iTextureID_02);
				else
					cGroundTile.SetTextureID(iTextureID_01);
			}
			iTextureCounter++;

			// Set the specifications for the ground tile
			cGroundTile.SetPosition(	glm::vec3(	vec3MapRearLeft.x + iCol * vec3TileSize.x + vec3TileSize.x * 0.5f,
													0.0f, 
													vec3MapRearLeft.z + iRow * vec3TileSize.z + vec3TileSize.z * 0.5f));
			cGroundTile.SetScale(vec3TileSize);

			// Recalculate the specifications of the ground tile
			cGroundTile.ReCalculate();

			// Add the ground tile into the vColumnsOfValues vector
			vColumnsOfValues.push_back(cGroundTile);
		}
		// Add the vColumnsOfValues vector into the vGroundTiles vector
		vGroundTiles.push_back(vColumnsOfValues);
	}

	return true;
}

/**
 @brief PreRender
 */
void CGroundMap::PreRender(void)
{
	// draw skybox as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
}

/**
 @brief Render
 @param cShader A Shader* variable which contains the shader for this class instance
 */
void CGroundMap::Render(void)
{
	// If the shader is in this class, then do not render
	if (!cShader)
	{
		cout << "CGroundMap::Render(): The shader is not available for this class instance." << endl;
		return;
	}

	// Activate shader
	cShader->use();

	// Render all the Ground Tiles
	// Update the vGroundTiles vector of vectors
	for (int iRow = 0; iRow < i32vec3NumTiles.x; iRow++)
	{
		for (int iCol = 0; iCol < i32vec3NumTiles.z; iCol++)
		{
			vGroundTiles[iRow][iCol].SetView(view);
			vGroundTiles[iRow][iCol].SetProjection(projection);
			vGroundTiles[iRow][iCol].Render();
		}
	}
}

/**
 @brief PostRender
 */
void CGroundMap::PostRender(void)
{
	// Set depth function back to default
	glDepthFunc(GL_LESS);
}

/**
 @brief Get the exact position at a certain position where you only have the x- and z- values.
 @param vec3Position A const glm::vec3 variable which contains the position where only the x- and z- values are available or accurate.
 @return A glm::vec3 variable which store the exact position where the y- value is taken from this class
 */
glm::vec3 CGroundMap::GetExactPosition(glm::vec3 vec3Position)
{
	// If this vec3Position is outside of the CGroundMap, then constrain it
	if (vec3Position.x > vec3MapFrontRight.x - vec3TileSize.x * 0.5f)
		vec3Position.x = vec3MapFrontRight.x - vec3TileSize.x * 0.5f;
	if (vec3Position.z > vec3MapFrontRight.z - vec3TileSize.z * 0.5f)
		vec3Position.z = vec3MapFrontRight.z - vec3TileSize.z * 0.5f;
	if (vec3Position.x < vec3MapRearLeft.x + vec3TileSize.x * 0.5f)
		vec3Position.x = vec3MapRearLeft.x + vec3TileSize.x * 0.5f;
	if (vec3Position.z < vec3MapRearLeft.z + vec3TileSize.z * 0.5f)
		vec3Position.z = vec3MapRearLeft.z + vec3TileSize.z * 0.5f;

	// Get the indices of the ground tile where this vec3Position is in
	int iRow = (int)((vec3Position.x - vec3MapRearLeft.x)) / ((int)vec3TileSize.x);
	int iCol = (int)((vec3Position.z - vec3MapRearLeft.z)) / ((int)vec3TileSize.z);

	return vGroundTiles[iRow][iCol].GetExactPosition(vec3Position);
}

/**
@brief Load a texture
@param filename A const char* variable which contains the file name of the texture
@return An bool value where true means it is successfully loaded, otherwise false
*/
bool CGroundMap::LoadTexture(const char* filename, GLuint& iTextureID_01)
{
	// Variables used in loading the texture
	int width, height, nrChannels;

	// The texture ID
	//GLuint iNewTextureID;

	// Generate a texture ID
	glGenTextures(1, &iTextureID_01);
	// Set this texture ID to be a GL_TEXTURE_2D
	glBindTexture(GL_TEXTURE_2D, iTextureID_01);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Get the image loader singleton instance
	CImageLoader* cImageLoader = CImageLoader::GetInstance();

	// Load the image from the file
	unsigned char *data = cImageLoader->Load(filename, width, height, nrChannels, true);
	if (data)
	{
		// Check if it has alpha channel (nrChannels == 4) or none (nrChannels == 3)
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
