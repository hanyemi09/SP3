#include "Entity2DFactory.h"
#include <stdlib.h>
#include <sstream>

#include "Enemy2D.h"

// Constructor
CEntity2DFactory::CEntity2DFactory(void)
	: index(0)
{
}

// Destructor
CEntity2DFactory::~CEntity2DFactory(void)
{
	// Clear the memory before destroying this class instance
	Destroy();
}

// Create an Entity and add to the map
CEntity2D* CEntity2DFactory::Create(const EntityID iIndex)
{
	CEntity2D* cNewEntity = NULL;

	switch (iIndex)
	{
	case ENEMY2D:
		// Create a new CEnemy2D instance
		cNewEntity = new CEnemy2D();
		// Add to the map
		AddEntity(iIndex, cNewEntity);
		break;
	default:
		break;
	}

	return cNewEntity;
}

// Get an Entity from this map
CEntity2D* CEntity2DFactory::GetEntity(const int iIndex)
{
	if (entityMap.count(iIndex) != 0)
		return entityMap[iIndex];

	return nullptr;
}

// Add an Entity to this map
void CEntity2DFactory::AddEntity(const int iIndex, CEntity2D* _newEntity)
{
	// Trivial Rejection : Invalid pointer provided
	if (_newEntity == nullptr)
		return;

	// Clean up first if there is an existing Entity with the same name
	RemoveEntity(iIndex);

	// Add the entity now
	entityMap[iIndex] = _newEntity;
}

// Remove an Entity from this map
bool CEntity2DFactory::RemoveEntity(const int iIndex)
{
	CEntity2D* currEntity = GetEntity(iIndex);
	if (currEntity != nullptr)
	{
		delete currEntity;
		entityMap.erase(iIndex);
		return true;
	}
	return false;
}


// Clear the memory
void CEntity2DFactory::Destroy(void)
{
	// Iterate through the entityMap
	for (std::map<int, CEntity2D*>::iterator it = entityMap.begin(); it != entityMap.end(); ++it)
	{
		// If the value/second was not deleted elsewhere, then delete it here
		if (it->second != NULL)
		{
			delete it->second;
			it->second = NULL;
		}
	}

	// Remove all elements in entityMap
	entityMap.clear();
}
