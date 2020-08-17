#include "JoystickController.h"

// Include GLFW
#include <GLFW/glfw3.h>

/**
 @brief This Constructor is a protected access modified as this class instance will be a Singleton.
 */
CJoystickController::CJoystickController(void)
	: iJoystickID(-1)
	, bJoystickPresent(false)
	, iButtonCount(0)
	, iAxesCount(0)
{
}

/**
 @brief This Destructor is a protected access modified as this class instance will be a Singleton.
 */
CJoystickController::~CJoystickController(void)
{
}

/**
 @brief Init this class instance and find a joystick connected to your computer.
		Note that the current codes allow for only one joystick to be used. If you wish to have
		more joysticks, then you need to modify the codes.
 */
#include <iostream>
using namespace std;

void CJoystickController::Init(void)
{
	// Initialise the array to store the statuses for the axes
	for (int i = 0; i < iMaxAxesCount; i++)
	{
		fAxesStatus[i] = 0.0f;
	}
	// Initialise the array to store the button statuses
	for (int i = 0; i < iMaxButtonCount; i++)
	{
		ucButtonStatus[i] = GLFW_RELEASE;
	}
	// Find the first joystick
	for (int i = 0; i < 16; i++)
	{
		int present = glfwJoystickPresent(i);
		if (present == 1)
		{
			iJoystickID = i;
			cout << "Joystick found at " << iJoystickID << endl;
			bJoystickPresent = true;
			break;
		}
	}
	if (iJoystickID == -1)
		cout << "No joystick found." << endl;
}

/**
 @brief Perform pre-update operations
 */
void CJoystickController::PreUpdate(void)
{
}


/**
@brief Perform post-update operations
*/
void CJoystickController::PostUpdate(void)
{
}

/**
 @brief Perform update operation for a key
 */
void CJoystickController::Update(const int iJoystickIndex)
{
	if ((iJoystickIndex < 0) && (iJoystickIndex > iMaxJoystickCount))
		return;
	if (glfwJoystickPresent(GLFW_JOYSTICK_1) == 1)
	{
		bJoystickPresent = true;
		//std::cout << bJoystickPresent << std::endl;

		const float *axes = glfwGetJoystickAxes(iJoystickIndex, &iAxesCount);
		for (int i = 0; i < iAxesCount; i++)
		{
			fAxesStatus[i] = axes[i];
		}
		//std::cout << iAxesCount << std::endl;

		/*
		std::cout << "Left Stick X Axis: " << axes[0] << std::endl; // tested with PS4 controller connected via micro USB cable
		std::cout << "Left Stick Y Axis: " << axes[1] << std::endl; // tested with PS4 controller connected via micro USB cable
		std::cout << "Right Stick X Axis: " << axes[2] << std::endl; // tested with PS4 controller connected via micro USB cable
		std::cout << "Right Stick Y Axis: " << axes[3] << std::endl; // tested with PS4 controller connected via micro USB cable
		std::cout << "Left Trigger/L2: " << axes[4] << std::endl; // tested with PS4 controller connected via micro USB cable
		std::cout << "Right Trigger/R2: " << axes[5] << std::endl; // tested with PS4 controller connected via micro USB cable
		*/

		const unsigned char *buttons = glfwGetJoystickButtons(iJoystickIndex, &iButtonCount);
		for (int i = 0; i < iButtonCount; i++)
		{
			ucButtonStatus[i] = buttons[i];
		}
	}
	else
		bJoystickPresent = false;
}

/**
 @brief Tools for Axes
 @param int A const int variable to check for a joystick status for a certain axis

 Axes
 0 Left and right on left joystick.
 1 Up and down on left joystick.
 2 Left and right back triggers.
 3 Up and down on right joystick.
 4 Left and right on right joystick.
 */
bool CJoystickController::IsJoystickActivated(const DIRECTION eAxisIndex) const
{
	if (bJoystickPresent == false)
		return false;

	// We only check for the left joystick for now.
	switch (eAxisIndex)
	{
	case FORWARD:
		if (fAxesStatus[1] > 0.01f)
			return true;
		break;
	case BACKWARD:
		if (fAxesStatus[1] < -0.01f)
			return true;
		break;
	case LEFT:
		if (fAxesStatus[0] < -0.01f)
			return true;
		break;
	case RIGHT:
		if (fAxesStatus[0] > 0.01f)
			return true;
		break;
	default:
		break;
	}
	return false;
}

/**
 @brief Check if a button is pressed down
 @param int A const int variable to check in the currStatus array

 The following codes are for checking on the button statuses. The indices for each button is shown here.
 0 A
 1 B
 2 X
 3 Y
 4 Left shoulder button
 5 Right shoulder button
 6 Back button <
 7 Start button >
 8 Left joystick button
 9 Right joystick button
 10 D-Pad Up
 11 D-Pad Right
 12 D-Pad Down
 13 D-Pad Left
 */
bool CJoystickController::IsButtonDown(const int iButtonIndex) const
{
	if (bJoystickPresent == false)
		return false;

	if ((iButtonIndex >= 0) && (iButtonIndex < iMaxButtonCount))
	{
		//std::cout << "Pressed" << std::endl;
		if (ucButtonStatus[iButtonIndex] == GLFW_PRESS)
			return true;
	}
	return false;
}

/**
 @brief Check if a button is up a.k.a. not pressed down
 @param int A const int variable to check in the currStatus array
 */
bool CJoystickController::IsButtonUp(const int iButtonIndex) const
{
	if (bJoystickPresent == false)
		return false;

	if ((iButtonIndex >= 0) && (iButtonIndex < iMaxButtonCount))
	{
		//std::cout << "Released" << std::endl;
		if (ucButtonStatus[iButtonIndex] == GLFW_RELEASE)
			return true;
	}
	return false;
}

/**
 @brief Check if a button is pressed down and held down
 @param int A const int variable to check in the currStatus and prevStatus array
 */
bool CJoystickController::IsButtonPressed(const int iButtonIndex) const
{
	if (bJoystickPresent == false)
		return false;

	if ((iButtonIndex >= 0) && (iButtonIndex < iMaxButtonCount))
	{
		//std::cout << "Repeated" << std::endl;
		if (ucButtonStatus[iButtonIndex] == GLFW_REPEAT)
			return true;
	}
	return false;
}

/**
 @brief Check if a button is released after being held down
 @param int A const int variable to check in the currStatus array
 */
bool CJoystickController::IsButtonReleased(const int iButtonIndex) const
{
	if (bJoystickPresent == false)
		return false;

	if ((iButtonIndex >= 0) && (iButtonIndex < iMaxButtonCount))
	{
		if (ucButtonStatus[iButtonIndex] == GLFW_RELEASE)
			return true;
	}
	return false;
}

/**
@brief Get joystick ID
*/
int CJoystickController::GetJoystickID(void) const
{
	return iJoystickID;
}

/**
 @brief Get joystick name
 */
const char* CJoystickController::GetJoystickName(const int iJoystickIndex) const
{
	if (bJoystickPresent == false)
		return false;

	if ((iJoystickIndex >= 0) && (iJoystickIndex < iMaxJoystickCount))
	{
		//std::cout << name << std::endl;
		return glfwGetJoystickName(iJoystickIndex);
	}

	return NULL;
}
