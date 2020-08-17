#pragma once

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include Entity3D
#include <Primitives/Entity3D.h>

// Include shader
#include <RenderControl\shader.h>

// Include Projectile
#include "Projectile.h"

class CWeaponInfo
{
public:
	// Default Constructor
	CWeaponInfo(void);
	// Destructor
	virtual ~CWeaponInfo(void);

	// Set a shader to this class instance
	virtual void SetShader(Shader* cShader);

	// Set the number of ammunition in the magazine for this player
	virtual void SetMagRound(const int iMagRounds);
	// Set the maximum number of ammunition in the magazine for this weapon
	virtual void SetMaxMagRound(const int iMagRounds);
	// The current total number of rounds currently carried by this player
	virtual void SetTotalRound(const int iTotalRounds);
	// The max total number of rounds currently carried by this player
	virtual void SetMaxTotalRound(const int iMaxTotalRounds);

	// Get the number of ammunition in the magazine for this player
	virtual int GetMagRound(void) const;
	// Get the maximum number of ammunition in the magazine for this weapon
	virtual int GetMaxMagRound(void) const;
	// Get the current total number of rounds currently carried by this player
	virtual int GetTotalRound(void) const;
	// Get the max total number of rounds currently carried by this player
	virtual int GetMaxTotalRound(void) const;

	// Set the time between shots
	virtual void SetTimeBetweenShots(const double dTimeBetweenShots);
	// Set the firing rate in rounds per min
	virtual void SetFiringRate(const int iFiringRate);
	// Set the firing flag
	virtual void SetCanFire(const bool bFire);

	// Get the time between shots
	virtual double GetTimeBetweenShots(void) const;
	// Get the firing rate
	virtual int GetFiringRate(void) const;
	// Get the firing flag
	virtual bool GetCanFire(void) const;

	// Initialise this instance to default values
	virtual void Init(void);
	// Update the elapsed time
	virtual void Update(const double dElapsedTime);
	// Discharge this weapon
	virtual CProjectile* Discharge(glm::vec3 vec3Position, glm::vec3 vec3Front, CEntity3D* pSource = NULL);
	// Reload this weapon
	virtual void Reload(void);
	// Add rounds
	virtual void AddRounds(const int iNewRounds);

	// Print Self
	virtual void PrintSelf(void);

	enum WeaponType
	{
		PISTOL,
		RIFLE,
	};

	WeaponType type;

protected:
	// Handler to the Shader Program instance
	Shader* cShader;

	// The number of ammunition in a magazine for this weapon
	int iMagRounds;
	// The maximum number of ammunition for this magazine for this weapon
	int iMaxMagRounds;
	// The current total number of rounds currently carried by this player
	int iTotalRounds;
	// The max total number of rounds currently carried by this player
	int iMaxTotalRounds;

	// The time between shots in milliseconds
	double dTimeBetweenShots;
	// The elapsed time (between shots) in milliseconds
	double dElapsedTime;
	// Boolean flag to indicate if weapon can fire now
	bool bFire;
};
