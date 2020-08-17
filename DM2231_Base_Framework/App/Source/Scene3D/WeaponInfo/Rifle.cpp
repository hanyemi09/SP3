#include "Rifle.h"

/**
@brief Default Constructor
*/
CRifle::CRifle()
{
}

/**
@brief Default Destructor
*/
CRifle::~CRifle()
{
}

/**
@brief Initialise this instance to default values
*/
void CRifle::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	type = CWeaponInfo::WeaponType::RIFLE;

	// The number of ammunition in a magazine for this weapon
	iMagRounds = 30;
	// The maximum number of ammunition for this magazine for this weapon
	iMaxMagRounds = 30;
	// The current total number of rounds currently carried by this player
	iTotalRounds = 99;
	// The max total number of rounds currently carried by this player
	iMaxTotalRounds = 99;

	// The time between shots
	dTimeBetweenShots = 0.05;
	// The elapsed time (between shots)
	dElapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}
