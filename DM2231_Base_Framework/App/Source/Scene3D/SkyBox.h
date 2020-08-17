/**
 CSkyBox
 By: Toh Da Jun
 Date: Apr 2020
 */
#pragma once

// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

#include <Primitives\Entity3D.h>

#include <GL/glew.h>
#include <includes\glm.hpp>

#include <RenderControl\shader.h>
#include <vector>

#include "GameControl\Settings.h"

class CSkyBox : public CEntity3D, public CSingletonTemplate<CSkyBox>
{
	friend CSingletonTemplate<CSkyBox>;
public:
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

protected:
	std::vector<std::string> skyboxTexture;

	// Constructor
	CSkyBox(void);

	// Destructor
	~CSkyBox(void);

	// Load SkyBox textures
	GLuint LoadSkyBoxTextures(void);
};
