#include "Scene2D.h"
#include <iostream>
using namespace std;

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include "System\filesystem.h"

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CScene2D::CScene2D(void)
	: cShader(NULL)
	, cMap2D(NULL)
	, cTextRenderer(NULL)
	, cKeyboardController(NULL)
	, cFPSCounter(NULL)
	, cSoundController(NULL)
{
}

/**
 @brief Destructor
 */
CScene2D::~CScene2D(void)
{
	if (cSoundController)
	{
		cSoundController->Destroy();
		cSoundController = NULL;
	}

	// We won't delete this since it was created elsewhere
	cFPSCounter = NULL;

	// We won't delete this since it was created elsewhere
	cKeyboardController = NULL;

	if (cTextRenderer)
	{
		cTextRenderer->Destroy();
		cTextRenderer = NULL;
	}

	if (cMap2D)
	{
		cMap2D->Destroy();
		cMap2D = NULL;
	}

	if (cShader)
	{
		delete cShader;
		cShader = NULL;
	}
}

/**
@brief Init Initialise this instance
*/ 
bool CScene2D::Init(void)
{
	// Setup the shaders
	cShader = new Shader("Shader//Scene2D.vs", "Shader//Scene2D.fs");

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	cShader->use();
	cShader->setInt("texture1", 0);

	// Create and initialise the Map 2D
	cMap2D = CMap2D::GetInstance();
	// Set a shader to this class
	cMap2D->SetShader(cShader);
	// Initialise the instance
	if (cMap2D->Init() == false)
	{
		cout << "Failed to load CMap2D" << endl;
		return false;
	}

	// Setup the shaders
	textShader = new Shader("Shader//text.vs", "Shader//text.fs");

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	textShader->use();

	// Create and initialise the Map 2D
	cTextRenderer = CTextRenderer::GetInstance();
	// Set a shader to this class
	cTextRenderer->SetShader(textShader);
	// Initialise the instance
	if (cTextRenderer->Init() == false)
	{
		cout << "Failed to load CTextRenderer" << endl;
		return false;
	}

	// Store the keyboard controller singleton instance here
	cKeyboardController = CKeyboardController::GetInstance();

	// Store the CFPSCounter singleton instance here
	cFPSCounter = CFPSCounter::GetInstance();

	// Load the sounds into CSoundController
	cSoundController = CSoundController::GetInstance();
	cSoundController->Init();
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Bell.ogg"), 1, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Explosion.ogg"), 2, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Jump.ogg"), 3, true);

	return true;
}

/**
@brief Update Update this instance
*/
void CScene2D::Update(const double dElapsedTime)
{
	// Call the Map2D's update method
	cMap2D->Update(dElapsedTime);

	// Get keyboard updates
	if (cKeyboardController->IsKeyDown(GLFW_KEY_F6))
	{
		// Save the current game to a save file
		// Make sure the file is open
		try {
			if (cMap2D->SaveMap("Maps/DM2231_Map_Level_01_SAVEGAME.csv") == false)
			{
				throw runtime_error("Unable to save the current game to a file");
			}
		}
		catch (runtime_error e)
		{
			cout << "Runtime error: " << e.what();
			return;
		}
	}
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CScene2D::PreRender(void)
{
	// Reset the OpenGL rendering environment
	glLoadIdentity();

	// Clear the screen and buffer
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Enable 2D texture rendering
	glEnable(GL_TEXTURE_2D);

	// Call the Map2D's PreRender()
	cMap2D->PreRender();

	// Call the Map2D's PreRender()
	cTextRenderer->PreRender();
}

/**
 @brief Render Render this instance
 */
void CScene2D::Render(void)
{
	// Call the Map2D's Render()
	cMap2D->Render();

	// Call the CTextRenderer's Render()
	cTextRenderer->Render("DM2231 GDEV 2D", 10.0f, 10.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.0f));
	// Render FPS info
	cTextRenderer->Render(cFPSCounter->GetFrameRateString(), 
							10.0f, 580.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.0f));
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CScene2D::PostRender(void)
{
	// Call the Map2D's PostRender()
	cMap2D->PostRender();

	// Call the Map2D's PostRender()
	cTextRenderer->PostRender();
}