#include "EntityManager.h"

#include "../Application.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CEntityManager::CEntityManager(void)
	: model(glm::mat4(1.0f))
	, view(glm::mat4(1.0f))
	, projection(glm::mat4(1.0f))
	, enemy_deathCount(0)
{
}

/**
 @brief Destructor
 */
CEntityManager::~CEntityManager(void)
{
	// Remove all CEntity3D
	std::list<CEntity3D*>::iterator it = lEntity3D.begin(), end = lEntity3D.end();
	while (it != end)
	{
		// Delete if done
		delete *it;
		it = lEntity3D.erase(it);
	}
}

/**
 @brief Initialise this class instance
 */
bool CEntityManager::Init(void)
{
	//enemyHealth = 8;
	enemy_deathCount = 0;

	cSoundController = CSoundController::GetInstance();
	cSoundController->Init();
	//cSoundController->LoadSound("../Sounds/damage.ogg", 1);

	lEntity3D.clear();
	return true;
}

/**
 @brief Set model
 @param model A glm::mat4 variable containing the model for this class instance
 */
void CEntityManager::SetModel(glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A glm::mat4 variable containing the model for this class instance
 */
void CEntityManager::SetView(glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A glm::mat4 variable containing the model for this class instance
 */
void CEntityManager::SetProjection(glm::mat4 projection)
{
	this->projection = projection;
}

/**
@brief Add a CEntity3D* to this class instance
*/
void CEntityManager::Add(CEntity3D* cEntity3D)
{
	lEntity3D.push_back(cEntity3D);
}

/**
@brief Remove a CEntity3D* from this class instance
*/
bool CEntityManager::Erase(CEntity3D* cEntity3D)
{
	// Find the entity's iterator
	std::list<CEntity3D*>::iterator findIter = std::find(lEntity3D.begin(), lEntity3D.end(), cEntity3D);

	// Delete the entity if found
	if (findIter != lEntity3D.end())
	{
		// Delete the CEntity3D
		delete *findIter;
		// Go to the next iteration after erasing from the list
		findIter = lEntity3D.erase(findIter);
		return true;
	}
	// Return false if not found
	return false;
}

/**
@brief Collision Check for a CEntity3D*
*/
bool CEntityManager::CollisionCheck(CEntity3D* cEntity3D)
{
	bool bResult = false;

	std::list<CEntity3D*>::iterator it, end;
	end = lEntity3D.end();
	for (it = lEntity3D.begin(); it != end; ++it)
	{
		// Check for collisions between the 2 entities
		if (cEntity3D->CheckForCollision(*it) == true)
		{
			if ((*it)->GetType() == CEntity3D::TYPE::NPC)
			{
				// Rollback the cEntity3D's position
				cEntity3D->RollbackPosition();
				// Rollback the NPC's position
				(*it)->RollbackPosition();
				cout << "** Collision between Player and NPC ***" << endl;
				bResult = true;
				
				cSoundController->PlaySoundByID(1);

				static_cast<CProgressBar*>(cProgressBar)->set_healthBarState(true);

				// Quit this loop since a collision has been found
				break;
			}
			else if ((*it)->GetType() == CEntity3D::TYPE::PROJECTILE)
			{
				// Mark the projectile for deletion
				(*it)->SetToDelete(true);
				cout << "** Collision between Player and Projectile ***" << endl;
				bResult = true;
				// Quit this loop since a collision has been found
				break;
			}
			else if ((*it)->GetType() == CEntity3D::TYPE::STRUCTURE)
			{
				// Rollback the cEntity3D's position
				cEntity3D->RollbackPosition();

				cSoundController->PlaySoundByID(2);

				cout << "** Collision between Player and Structure ***" << endl;
				bResult = true;
				// Quit this loop since a collision has been found
				break;
			}
		}
	}
	return bResult;
}

/**
 @brief Update this class instance
 */
void CEntityManager::Update(const double dElapsedTime)
{
	std::list<CEntity3D*>::iterator it, end;
	std::list<CEntity3D*>::iterator it_other;

	// Update all CEntity3D
	end = lEntity3D.end();
	for (it = lEntity3D.begin(); it != end; ++it)
	{
		(*it)->Update(dElapsedTime);
	}

	// Check for collisions among them
	end = lEntity3D.end();
	for (it = lEntity3D.begin(); it != end; ++it)
	{
		for (it_other = it; it_other != end; ++it_other)
		{
			// If the 2 entities to check are the same, then skip this iteration
			if (it_other == it)
				continue;

			// Check for collisions between the 2 entities
			if ((*it)->CheckForCollision(*it_other) == true)
			{
				if (((*it)->GetType() == CEntity3D::TYPE::NPC) &&
					((*it_other)->GetType() == CEntity3D::TYPE::PROJECTILE))
				{
					if (static_cast<CEnemy3D*>(*it)->get_enemyHealth() != 0)
					{
						(*it)->RollbackPosition();
					}
					else
					{
						(*it)->SetToDelete(true);
						++enemy_deathCount;
					}

					(*it_other)->SetToDelete(true);
					cout << "** Collision between NPC and Projectile ***" << endl;

					static_cast<CEnemy3D*>(*it)->set_enemyHealth(static_cast<CEnemy3D*>(*it)->get_enemyHealth() - 1);
				}
				else if (((*it)->GetType() == CEntity3D::TYPE::PROJECTILE) &&
					((*it_other)->GetType() == CEntity3D::TYPE::NPC))
				{
					(*it)->SetToDelete(true);

					if (static_cast<CEnemy3D*>(*it)->get_enemyHealth() != 0)
					{
						(*it_other)->RollbackPosition();
					}
					else
					{
						(*it_other)->SetToDelete(true);
						++enemy_deathCount;
					}

					cout << "** Collision between NPC and Projectile ***" << endl;

					static_cast<CEnemy3D*>(*it)->set_enemyHealth(static_cast<CEnemy3D*>(*it)->get_enemyHealth() - 1);
				}
				else if (((*it)->GetType() == CEntity3D::TYPE::PROJECTILE) &&
					((*it_other)->GetType() == CEntity3D::TYPE::PROJECTILE))
				{
					(*it)->SetToDelete(true);
					(*it_other)->SetToDelete(true);
					cout << "** Collision between 2 Projectiles ***" << endl;
				}
				else if (((*it)->GetType() == CEntity3D::TYPE::NPC) &&
					((*it_other)->GetType() == CEntity3D::TYPE::NPC))
				{
					(*it)->RollbackPosition();
					(*it_other)->RollbackPosition();
					cout << "** Collision between 2 NPCs ***" << endl;
				}
				else if (((*it)->GetType() == CEntity3D::TYPE::NPC) &&
					((*it_other)->GetType() == CEntity3D::TYPE::STRUCTURE))
				{
					(*it)->RollbackPosition();
					//(*it_other)->RollbackPosition();
					cout << "** Collision between NPC and STRUCTURE ***" << endl;
				}
				else if (((*it)->GetType() == CEntity3D::TYPE::STRUCTURE) &&
					((*it_other)->GetType() == CEntity3D::TYPE::NPC))
				{
					(*it_other)->RollbackPosition();
					//(*it_other)->RollbackPosition();
					cout << "** Collision between NPC and STRUCTURE ***" << endl;
				}
			}
		}
	}
}

/**
 @brief CleanUp all CEntity3Ds which are marked for deletion
 */
void CEntityManager::CleanUp(void)
{
	std::list<CEntity3D*>::iterator it, end;
	it = lEntity3D.begin();
	end = lEntity3D.end();
	while (it != end)
	{
		if ((*it)->IsToDelete())
		{
			// Delete the CEntity3D
			delete *it;
			// Go to the next iteration after erasing from the list
			it = lEntity3D.erase(it);
		}
		else
		{
			// Go to the next iteration
			++it;
		}
	}
}

/**
 @brief Render this class instance
 */
void CEntityManager::Render(void)
{
	// Render all entities
	std::list<CEntity3D*>::iterator it, end;
	end = lEntity3D.end();
	for (it = lEntity3D.begin(); it != end; ++it)
	{
		(*it)->SetView(view);
		(*it)->SetProjection(projection);
		(*it)->PreRender();
		(*it)->Render();
		(*it)->PostRender();
	}
}

int CEntityManager::get_enemy_deathCount()
{
	return enemy_deathCount;
}

void CEntityManager::set_enemy_deathCount(int x)
{
	enemy_deathCount = x;
}

void CEntityManager::set_ProgressBar(CProgressBar* pBar)
{
	cProgressBar = pBar;
}
