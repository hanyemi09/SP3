#pragma once

#include "WeaponInfo.h"

class CRifle :	public CWeaponInfo
{
public:
	// Default Constructor
	CRifle(void);
	// Destructor
	virtual ~CRifle(void);

	// Initialise this instance to default values
	void Init(void);
};

