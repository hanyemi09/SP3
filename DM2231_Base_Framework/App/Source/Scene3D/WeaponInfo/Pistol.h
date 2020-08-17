#pragma once

#include "WeaponInfo.h"

class CPistol :	public CWeaponInfo
{
public:
	// Default Constructor
	CPistol(void);
	// Destructor
	virtual ~CPistol(void);

	// Initialise this instance to default values
	void Init(void);
};

