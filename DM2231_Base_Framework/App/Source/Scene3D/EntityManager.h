#pragma once

// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

// Include Entity3D
#include <Primitives/Entity3D.h>

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include list
#include <list>

#include "Enemy3D.h"
#include "EnemyBoss3D.h"
#include "../CameraEffects/ProgressBar.h"

#include "../SoundController/SoundController.h"

class CEntityManager : public CSingletonTemplate<CEntityManager>
{
	friend class CSingletonTemplate<CEntityManager>;

public:
	// Initialise this class instance
	bool Init(void);

	// Set model
	virtual void SetModel(glm::mat4 model);
	// Set view
	virtual void SetView(glm::mat4 view);
	// Set projection
	virtual void SetProjection(glm::mat4 projection);

	// Add a CEntity3D* to this class instance
	virtual void Add(CEntity3D* cEntity3D);
	// Remove a CEntity3D* from this class instance
	virtual bool Erase(CEntity3D* cEntity3D);

	// Collision Check for a CEntity3D*
	virtual bool CollisionCheck(CEntity3D* cEntity3D);

	// Update this class instance
	virtual void Update(const double dElapsedTime);

	// CleanUp all CEntity3Ds which are marked for deletion
	virtual void CleanUp(void);

	// Render this class instance
	virtual void Render(void);

	int get_enemy_deathCount();
	void set_enemy_deathCount(int x);

	void set_ProgressBar(CProgressBar* pBar);
	//int enemyHealth;

protected:
	// Render Settings
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	// List of CEntity3D
	std::list<CEntity3D*> lEntity3D;

	//CEnemy3D* cEnemy3D;

	//CEnemyBoss3D cEnemyBoss3D;

	CProgressBar* cProgressBar;

	CSoundController* cSoundController;

	int enemy_deathCount;

	// Default Constructor
	CEntityManager(void);
	// Destructor
	virtual ~CEntityManager(void);
};
