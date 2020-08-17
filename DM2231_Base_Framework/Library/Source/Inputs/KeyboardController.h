/**
 CKeyboardController
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

#include "../DesignPatterns/SingletonTemplate.h"
#include <bitset>
#include <windows.h>

class CKeyboardController : public CSingletonTemplate<CKeyboardController>
{
	friend CSingletonTemplate<CKeyboardController>;

public:
	// Constant int to indicate how many keys will be processed
	const static int MAX_KEYS = 256;

	// System Interface
	// Perform pre-update operations
	void PreUpdate(void);
	// Perform update operations
	void Update(void);
	// Perform post-update operations
	void PostUpdate(void);
	
	// User Interface
	bool IsKeyDown(const unsigned char _slot);
	bool IsKeyUp(const unsigned char _slot);
	bool IsKeyPressed(const unsigned char _slot);
	bool IsKeyReleased(const unsigned char _slot);

protected:
	// Constructor
	CKeyboardController(void);

	// Destructor
	virtual ~CKeyboardController(void);

	// Bitset to store information about current and previous keypress statuses
	std::bitset<MAX_KEYS> currStatus, prevStatus;

	// Check key status
	bool CheckKeyStatus(const unsigned short key);
};
