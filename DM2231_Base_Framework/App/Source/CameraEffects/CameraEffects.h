#pragma once

// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

// Include Entity3D
#include <Primitives/Entity3D.h>

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

class CCameraEffects : public CSingletonTemplate<CCameraEffects>, public CEntity3D
{
	friend class CSingletonTemplate<CCameraEffects>;

public:
	// Initialise this class instance
	bool Init(void);

	// Set model
	virtual void SetModel(glm::mat4 model);
	// Set view
	virtual void SetView(glm::mat4 view);
	// Set projection
	virtual void SetProjection(glm::mat4 projection);

	// Activate BloodScreen
	virtual void Activate_BloodScreen(void);
	// Set Countdown Timer for BloodScreen
	virtual bool SetTimer_BloodScreen(const float fCountdownTimer_BloodScreen = 2.0f);
	// Get Countdown Timer for BloodScreen
	virtual float GetTimer_BloodScreen(void) const;

	// Update this class instance
	virtual void Update(const double dElapsedTime);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

protected:
	// A transformation matrix for controlling where to render the entities
	glm::mat4 transform;

	// Boolean flag to indicate if the BloodScreen is rendered
	bool bStatus_BloodScreen;
	// Countdown Timer for BloodScreen
	float fCountdownTimer_BloodScreen;

	// Constructor
	CCameraEffects(void);
	// Destructor
	virtual ~CCameraEffects(void);
};
