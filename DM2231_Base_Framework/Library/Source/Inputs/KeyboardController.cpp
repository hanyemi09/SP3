#include "KeyboardController.h"

/**
 @brief This Constructor is a protected access modified as this class instance will be a Singleton.
 */
CKeyboardController::CKeyboardController(void)
{
}

/**
 @brief This Destructor is a protected access modified as this class instance will be a Singleton.
 */
CKeyboardController::~CKeyboardController(void)
{
}

/**
 @brief Perform pre-update operations
 */
void CKeyboardController::PreUpdate(void)
{
}

/**
 @brief Perform update operations
 */
void CKeyboardController::Update(void)
{
	bool _isPressed = false;

	// Update Keyboard Input
	for (int i = 0; i < CKeyboardController::MAX_KEYS; ++i)
	{
		_isPressed = CheckKeyStatus(i);

		if (_isPressed != currStatus[i])
		{
			// Backup the curStatus to prevStatus
			prevStatus.set(i, currStatus[i]);
			// Set the new status to curStatus
			currStatus.set(i, _isPressed);
		}
	}

}

/**
@brief Perform post-update operations
*/
void CKeyboardController::PostUpdate(void)
{
	// Copy the entire currStatus to prevStatus. Note that this method will not compare 
	// the current and previous status correctly for IsKeyPressed and IsKeyReleased
	prevStatus = currStatus;
}

/**
 @brief Check if a key is pressed down
 @param _slot A const unsigned char variable to check in the currStatus array
 */
bool CKeyboardController::IsKeyDown(const unsigned char _slot)
{
	return currStatus.test(_slot);
}

/**
 @brief Check if a key is up a.k.a. not pressed down
 @param _slot A const unsigned char variable to check in the currStatus array
 */
bool CKeyboardController::IsKeyUp(const unsigned char _slot)
{
	return !currStatus.test(_slot);
}

/**
 @brief Check if a key is pressed down and held down
 @param _slot A const unsigned char variable to check in the currStatus and prevStatus array
 */
bool CKeyboardController::IsKeyPressed(const unsigned char _slot)
{
	return IsKeyDown(_slot) && !prevStatus.test(_slot);
}

/**
 @brief Check if a key is released after being held down
 @param _slot A const unsigned char variable to check in the currStatus array
 */
bool CKeyboardController::IsKeyReleased(const unsigned char _slot)
{
	return IsKeyUp(_slot) && prevStatus.test(_slot);
}

/**
 @brief Check the status of a key
 @param key An unsigned short variable to pass the keycode to this method
 @return true if the key is being pressed down, false otherwise
*/
bool CKeyboardController::CheckKeyStatus(const unsigned short key)
{
	return ((GetAsyncKeyState(key) & 0x8001) != 0);
}