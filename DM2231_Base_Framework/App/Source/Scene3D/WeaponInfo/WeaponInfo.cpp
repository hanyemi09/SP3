#include "WeaponInfo.h"
#include "Projectile.h"

#include <iostream>
using namespace std;

/**
@brief Default Constructor
*/
CWeaponInfo::CWeaponInfo()
	: iMagRounds(1)
	, iMaxMagRounds(1)
	, iTotalRounds(90)
	, iMaxTotalRounds(999)
	, dTimeBetweenShots(0.5)
	, dElapsedTime(0.0)
	, bFire(true)
	, type(RIFLE)
{
}

/**
 @brief Default Destructor
 */
CWeaponInfo::~CWeaponInfo()
{
	// We set it to NULL only since it was declared somewhere else
	cShader = NULL;
}

/**
 @brief Set a shader to this class instance
 @return cShader A Shader pointer variable which stores the shader to allocate to this class instance
 */
void CWeaponInfo::SetShader(Shader* cShader)
{
	this->cShader = cShader;
}

/**
 @brief Set the number of ammunition in the magazine for this player
 */
void CWeaponInfo::SetMagRound(const int iMagRounds)
{
	this->iMagRounds = iMagRounds;
}

/**
 @brief Set the maximum number of ammunition in the magazine for this weapon
 */
void CWeaponInfo::SetMaxMagRound(const int iMaxMagRounds)
{
	this->iMaxMagRounds = iMaxMagRounds;
}

/**
 @brief The current total number of rounds currently carried by this player
 */
void CWeaponInfo::SetTotalRound(const int iTotalRounds)
{
	this->iTotalRounds = iTotalRounds;
}

/**
 @brief The max total number of rounds currently carried by this player
 */
void CWeaponInfo::SetMaxTotalRound(const int iMaxTotalRounds)
{
	this->iMaxTotalRounds = iMaxTotalRounds;
}


/**
 @brief Get the number of ammunition in the magazine for this player
 */
int CWeaponInfo::GetMagRound(void) const
{
	return iMagRounds;
}

/**
 @brief Get the maximum number of ammunition in the magazine for this weapon
 */
int CWeaponInfo::GetMaxMagRound(void) const
{
	return iMaxMagRounds;
}

/**
 @brief Get the current total number of rounds currently carried by this player
 */
int CWeaponInfo::GetTotalRound(void) const
{
	return iTotalRounds;
}

/**
 @brief Get the max total number of rounds currently carried by this player
 */
int CWeaponInfo::GetMaxTotalRound(void) const
{
	return iMaxTotalRounds;
}

/**
 @brief Set the time between shots
 */
void CWeaponInfo::SetTimeBetweenShots(const double dTimeBetweenShots)
{
	this->dTimeBetweenShots = dTimeBetweenShots;
}

/**
 @brief Set the firing rate in rounds per min
 */
void CWeaponInfo::SetFiringRate(const int firingRate)
{
	dTimeBetweenShots = 60.0 / (double)firingRate;	// 60 seconds divided by firing rate
}

/**
 @brief Set the firing flag
 */
void CWeaponInfo::SetCanFire(const bool bFire)
{
	this->bFire = bFire;
}

/**
 @brief Get the time between shots
 */
double CWeaponInfo::GetTimeBetweenShots(void) const
{
	return dTimeBetweenShots;
}

/**
 @brief Get the firing rate
 */
int CWeaponInfo::GetFiringRate(void) const
{
	return (int)(60.0 / dTimeBetweenShots);	// 60 seconds divided by dTimeBetweenShots
}

/**
 @brief Get the firing flag
 */
bool CWeaponInfo::GetCanFire(void) const
{
	return bFire;
}

/**
 @brief Initialise this instance to default values
 */
void CWeaponInfo::Init(void)
{
	// The number of ammunition in a magazine for this weapon
	iMagRounds = 1;
	// The maximum number of ammunition for this magazine for this weapon
	iMaxMagRounds = 1;
	// The current total number of rounds currently carried by this player
	iTotalRounds = 8;
	// The max total number of rounds currently carried by this player
	iMaxTotalRounds = 8;

	// The time between shots
	dTimeBetweenShots = 0.5;
	// The elapsed time (between shots)
	dElapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}

/**
 @brief Update the elapsed time
 */
void CWeaponInfo::Update(const double dt)
{
	// If the weapon can fire, then don't check further
	if (bFire)
		return;

	dElapsedTime += dt;
	if (dElapsedTime > dTimeBetweenShots)
	{
		bFire = true;
		dElapsedTime = 0.0;
	}
}

/**
 @brief Discharge this weapon
 */
CProjectile* CWeaponInfo::Discharge(glm::vec3 vec3Position, glm::vec3 vec3Front, CEntity3D* pSource)
{
	if (bFire)
	{
		// If there is still ammo in the magazine, then fire
		if (iMagRounds > 0)
		{
			// Create a projectile. 
			// Its position is slightly in front of the player to prevent collision
			// Its direction is same as the player.
			// It will last for 2.0 seconds and travel at 20 units per frame
			CProjectile* aProjectile = new CProjectile();
			aProjectile->SetShader(cShader);
			aProjectile->Init(vec3Position + vec3Front * 0.75f, vec3Front, 2.0f, 20.0f);
			aProjectile->ActivateCollider(cShader);
			aProjectile->SetStatus(true);
			aProjectile->SetSource(pSource);
			
			// Lock the weapon after this discharge
			bFire = false;
			// Reduce the rounds by 1
			iMagRounds--;

			return aProjectile;
		}
	}
	return NULL;
}

/**
 @brief Reload this weapon
 */
void CWeaponInfo::Reload(void)
{
	if (iMagRounds < iMaxMagRounds)
	{
		if (iMaxMagRounds - iMagRounds <= iTotalRounds)
		{
			iTotalRounds -= iMaxMagRounds - iMagRounds;
			iMagRounds = iMaxMagRounds;
		}
		else
		{
			iMagRounds += iTotalRounds;
			iTotalRounds = 0;
		}
	}
}

/**
 @brief Add rounds
 */
void CWeaponInfo::AddRounds(const int newRounds)
{
	if (iTotalRounds + newRounds > iMaxTotalRounds)
		iTotalRounds = iMaxTotalRounds;
	else
		iTotalRounds += newRounds;
}

/**
 @brief Print Self
 */
void CWeaponInfo::PrintSelf(void)
{
	cout << "CWeaponInfo::PrintSelf()" << endl;
	cout << "========================" << endl;
	cout << "iMagRounds\t\t:\t" << iMagRounds << endl;
	cout << "iMaxMagRounds\t\t:\t" << iMaxMagRounds << endl;
	cout << "iTotalRounds\t\t:\t" << iTotalRounds << endl;
	cout << "iMaxTotalRounds\t\t:\t" << iMaxTotalRounds << endl;
	cout << "dTimeBetweenShots\t:\t" << dTimeBetweenShots << endl;
	cout << "dElapsedTime\t\t:\t" << dElapsedTime << endl;
	cout << "bFire\t\t:\t" << bFire << endl;
}
