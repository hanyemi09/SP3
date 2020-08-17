#pragma once

// Include Entity3D
#include <Primitives/Entity3D.h>

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

class CProgressBar : public CEntity3D
{
public:
	// Constructor
	CProgressBar(void);
	// Destructor
	virtual ~CProgressBar(void);

	// Initialise this class instance
	bool Init(void);

	// Set model
	virtual void SetModel(glm::mat4 model);
	// Set view
	virtual void SetView(glm::mat4 view);
	// Set projection
	virtual void SetProjection(glm::mat4 projection);

	// Update this class instance
	virtual void Update(const double dElapsedTime);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

	bool get_healthBarState();
	void set_healthBarState(bool state);

	float get_healthBarLength();

protected:
	// A transformation matrix for controlling where to render the entities
	glm::mat4 transform;
	bool decrease_healthBar;


	float fHeight, fWidth;

};
