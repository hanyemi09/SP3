/**
 CEntity2DFactory
 By: Toh Da Jun
 Date: Mar 2020
 */

#pragma once

// Include Singleton template
#include "DesignPatterns\SingletonTemplate.h"

// Include map storage
#include <map>

// Include CEntity2D
#include "Entity2D.h"

typedef int EntityID;
#define ENEMY2D 100

class CEntity2DFactory : public CSingletonTemplate<CEntity2DFactory>
{
	friend CSingletonTemplate<CEntity2DFactory>;

public:
	// Constructor
	CEntity2DFactory(void);
	// Destructor
	~CEntity2DFactory(void);

	// Create an entity and add to the map
	CEntity2D* Create(const EntityID iIndex);

	// Get an entity from this map
	CEntity2D* GetEntity(const int iIndex);
	// Add an entity to this map
	void AddEntity(const int iIndex, CEntity2D* _newGoodies);
	// Remove an entity from this map
	bool RemoveEntity(const int iIndex);
	// Get the number of goodies in this map
	int GetNumOfEntity() const;

protected:
	// The current index
	int index;
	// The map of all the entity created
	std::map<int, CEntity2D*> entityMap;

	// Destroy this class instance
	void Destroy(void);
};