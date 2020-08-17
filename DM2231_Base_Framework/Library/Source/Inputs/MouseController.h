/**
 MouseController
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

// Include SingletonTemplate
#include "../DesignPatterns/SingletonTemplate.h"

class MouseController : public CSingletonTemplate<MouseController>
{
	friend CSingletonTemplate<MouseController>;

public:
	enum BUTTON_TYPE
	{
		LMB = 0,
		RMB,
		MMB,
		NUM_MB
	};
	enum SCROLL_TYPE
	{
		SCROLL_TYPE_XOFFSET = 0,
		SCROLL_TYPE_YOFFSET,
		NUM_SCROLL_TYPE
	};

	// System Interface
	void UpdateMousePosition(double _x, double _y);
	void UpdateMouseButtonPressed(int _slot);
	void UpdateMouseButtonReleased(int _slot);
	void UpdateMouseScroll(const double xoffset, const double yoffset);
	void EndFrameUpdate();

	// User Interface
	bool IsButtonDown(unsigned char _slot);
	bool IsButtonUp(unsigned char _slot);
	bool IsButtonPressed(unsigned char _slot);
	bool IsButtonReleased(unsigned char _slot);
	double GetMouseScrollStatus(SCROLL_TYPE _scrolltype);

	void GetMousePosition(float& _resultX, float& _resultY);
	void GetMousePosition(double& _resultX, double& _resultY);
	void GetMouseDelta(float& _resultX, float& _resultY);
	void GetMouseDelta(double& _resultX, double& _resultY);

	inline bool GetKeepMouseCentered(){ return keepMouseCentered; };
	inline void SetKeepMouseCentered(bool _value){ keepMouseCentered = _value; };

protected:
	// Constructor
	MouseController(void);
	// Destructor
	~MouseController(void);

	double curr_posX, curr_posY, prev_posX, prev_posY;
	unsigned char currBtnStatus, prevBtnStatus;
	double xoffset, yoffset;
	bool keepMouseCentered;
};
