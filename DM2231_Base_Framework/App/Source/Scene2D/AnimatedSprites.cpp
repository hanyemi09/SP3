/**
 CAnimatedSprites
 By: Toh Da Jun
 Date: Mar 2020
 */
#include "AnimatedSprites.h"

#include <iostream>
using namespace std;

// Include GLEW
#include <GL/glew.h>

// Include ImageLoader
#include "System\ImageLoader.h"

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CAnimatedSprites::CAnimatedSprites(void)
	: iTextureID_NEUTRAL(0)
	, iIndex_LEFT(0)
	, iIndex_RIGHT(0)
	, iIndex_UP(0)
	, iIndex_DOWN(0)
{

}

/**
 @brief Destructor This destructor has protected access modifier as this class will be a Singleton
 */
CAnimatedSprites::~CAnimatedSprites(void)
{

}

/**
  @brief Initialise this instance
  */
bool CAnimatedSprites::Init(void)
{
	return true;
}

/**
 @brief Add a Texture ID to this
 @param sDIRECTION A DIRECTION struct which indicates the direction which the animation is moving towards
 @param iTextureID A const GLuint variable which stores the texture ID
 */
void CAnimatedSprites::Add(const DIRECTION sDIRECTION, const GLuint iTextureID)
{
	int iID = 0;

	switch (sDIRECTION)
	{
	case NEUTRAL:
		iTextureID_NEUTRAL = iTextureID;
		break;
	case LEFT:
		// Use current map size as the ID
		iID = MapOfTextureIDs_LEFT.size();
		// Store the texture ID into MapOfTextureIDs
		MapOfTextureIDs_LEFT.insert(pair<int, int>(iID, iTextureID));
		break;
	case RIGHT:
		// Use current map size as the ID
		iID = MapOfTextureIDs_RIGHT.size();
		// Store the texture ID into MapOfTextureIDs
		MapOfTextureIDs_RIGHT.insert(pair<int, int>(iID, iTextureID));
		break;
	case UP:
		// Use current map size as the ID
		iID = MapOfTextureIDs_UP.size();
		// Store the texture ID into MapOfTextureIDs
		MapOfTextureIDs_UP.insert(pair<int, int>(iID, iTextureID));
		break;
	case DOWN:
		// Use current map size as the ID
		iID = MapOfTextureIDs_DOWN.size();
		// Store the texture ID into MapOfTextureIDs
		MapOfTextureIDs_DOWN.insert(pair<int, int>(iID, iTextureID));
		break;
	default:
		break;
	}
}

/**
 @brief Remove a Texture ID from this
 @param sDIRECTION A DIRECTION struct which indicates the direction which the animation is moving towards
 @param iTextureID A const GLuint variable which stores the texture ID
 */
void CAnimatedSprites::Remove(const DIRECTION sDIRECTION, const GLuint iTextureID)
{
	switch (sDIRECTION)
	{
	case NEUTRAL:
		iTextureID_NEUTRAL = 0;
		break;
	case LEFT:
		// Remove a map entry by its texture ID from MapOfTextureIDs
		for (std::map<int, int>::iterator it = MapOfTextureIDs_LEFT.begin(); it != MapOfTextureIDs_LEFT.end(); it++)
		{
			if ((it->second) == iTextureID)
			{
				it = MapOfTextureIDs_LEFT.erase(it);
				break;
			}
		}
		break;
	case RIGHT:
		// Remove a map entry by its texture ID from MapOfTextureIDs
		for (std::map<int, int>::iterator it = MapOfTextureIDs_RIGHT.begin(); it != MapOfTextureIDs_RIGHT.end(); it++)
		{
			if ((it->second) == iTextureID)
			{
				it = MapOfTextureIDs_RIGHT.erase(it);
				break;
			}
		}
		break;
	case UP:
		// Remove a map entry by its texture ID from MapOfTextureIDs
		for (std::map<int, int>::iterator it = MapOfTextureIDs_UP.begin(); it != MapOfTextureIDs_UP.end(); it++)
		{
			if ((it->second) == iTextureID)
			{
				it = MapOfTextureIDs_UP.erase(it);
				break;
			}
		}
		break;
	case DOWN:
		// Remove a map entry by its texture ID from MapOfTextureIDs
		for (std::map<int, int>::iterator it = MapOfTextureIDs_DOWN.begin(); it != MapOfTextureIDs_DOWN.end(); it++)
		{
			if ((it->second) == iTextureID)
			{
				it = MapOfTextureIDs_DOWN.erase(it);
				break;
			}
		}
		break;
	default:
		break;
	}
}

/**
 @brief Get ID
 @param sDIRECTION A DIRECTION struct which indicates the direction which the animation is moving towards
 */
GLuint CAnimatedSprites::Get(const DIRECTION sDIRECTION)
{
	switch (sDIRECTION)
	{
	case NEUTRAL:
		return iTextureID_NEUTRAL;
		break;
	case LEFT:
		if (MapOfTextureIDs_LEFT.size() == 0)
			return iTextureID_NEUTRAL;
		iIndex_LEFT++;
		if (iIndex_LEFT >= (int)MapOfTextureIDs_LEFT.size())
			iIndex_LEFT = 0;
		return MapOfTextureIDs_LEFT.at(iIndex_LEFT);
		break;
	case RIGHT:
		if (MapOfTextureIDs_RIGHT.size() == 0)
			return iTextureID_NEUTRAL;
		iIndex_RIGHT++;
		if (iIndex_RIGHT >= (int)MapOfTextureIDs_RIGHT.size())
			iIndex_RIGHT = 0;
		return MapOfTextureIDs_RIGHT.at(iIndex_RIGHT);
		break;
	case UP:
		if (MapOfTextureIDs_UP.size() == 0)
			return iTextureID_NEUTRAL;
		iIndex_UP++;
		if (iIndex_UP >= (int)MapOfTextureIDs_UP.size())
			iIndex_UP = 0;
		return MapOfTextureIDs_UP.at(iIndex_UP);
		break;
	case DOWN:
		if (MapOfTextureIDs_DOWN.size() == 0)
			return iTextureID_NEUTRAL;
		iIndex_DOWN++;
		if (iIndex_DOWN >= (int)MapOfTextureIDs_DOWN.size())
			iIndex_DOWN = 0;
		return MapOfTextureIDs_DOWN.at(iIndex_DOWN);
		break;
	default:
		break;
	}
	return 0;
}