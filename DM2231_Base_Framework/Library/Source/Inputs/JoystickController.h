/**
 CJoystickController
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

#include "../DesignPatterns/SingletonTemplate.h"
#include <windows.h>

class CJoystickController : public CSingletonTemplate<CJoystickController>
{
	friend CSingletonTemplate<CJoystickController>;

public:
	enum DIRECTION {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		NUM_DIRECTION
	};

	// System Interface
	// Init
	void Init(void);

	// Perform pre-update operations
	void PreUpdate(void);
	// Perform update operation for a key
	void Update(const int iJoystickIndex);
	// Perform post-update operations
	void PostUpdate(void);

	// Tools for Axes
	bool IsJoystickActivated(const DIRECTION eAxisIndex) const;

	// Tools for Buttons
	bool IsButtonDown(const int iButtonIndex) const;
	bool IsButtonUp(const int iButtonIndex) const;
	bool IsButtonPressed(const int iButtonIndex) const;
	bool IsButtonReleased(const int iButtonIndex) const;

	// Get joystick ID
	int GetJoystickID(void) const;
	// Get joystick name
	const char* GetJoystickName(const int iJoystickIndex) const;

protected:
	// The joystick ID
	int iJoystickID;

	// boolean flag to indicate if the joystick was connected to the computer properly
	bool bJoystickPresent;

	// The maximum number of buttons on the joystick
	const int iMaxButtonCount = 16;

	// The number of buttons on the joystick
	int iButtonCount;

	// Array containing statuses of up to 16 buttons on the joystick
	unsigned char ucButtonStatus[16];

	// The maximum number of axes on the joystick
	const int iMaxAxesCount = 16;

	// The number of axes on the joystick
	int iAxesCount;

	// Array containing statuses of up to 16 axes on the joystick
	float fAxesStatus[16];

	// The maximum number of joysticks
	const int iMaxJoystickCount = 16;

	// Constructor
	CJoystickController(void);

	// Destructor
	virtual ~CJoystickController(void);
};
