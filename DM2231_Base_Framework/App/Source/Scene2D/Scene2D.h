/**
 CScene2D
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"
// Include shader
#include "RenderControl\shader.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include the Map2D as we will use it to check the player's movements and actions
#include "Map2D.h"
// Include TextRenderer
#include "RenderControl\TextRenderer.h"

// Include Keyboard controller
#include "Inputs\KeyboardController.h"

// FPS Counter
#include "TimeControl\FPSCounter.h"

// Include SoundController
#include "..\SoundController\SoundController.h"

class CScene2D : public CSingletonTemplate<CScene2D>
{
	friend CSingletonTemplate<CScene2D>;
public:
	// Init
	bool Init(void);

	// Update
	void Update(const double dElapsedTime);

	// PreRender
	void PreRender(void);

	// Render
	void Render(void);

	// PostRender
	void PostRender(void);

	// Handler to the Shader Program instance
	Shader* cShader;
	// Handler to the Text Shader Program instance
	Shader* textShader;

protected:
	// The handler containing the instance of the 2D Map
	CMap2D* cMap2D;

	// The handler containing the instance of the CTextRenderer
	CTextRenderer* cTextRenderer;

	// Keyboard Controller singleton instance
	CKeyboardController* cKeyboardController;

	// A transformation matrix for controlling where to render the entities
	glm::mat4 transform;

	// FPS Control
	CFPSCounter* cFPSCounter;

	// Handler to the CSoundController
	CSoundController* cSoundController;

	// Constructor
	CScene2D(void);
	// Destructor
	virtual ~CScene2D(void);
};

