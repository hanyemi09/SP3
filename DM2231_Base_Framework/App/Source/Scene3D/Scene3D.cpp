#include "Scene3D.h"
#include <iostream>
using namespace std;

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include "System\filesystem.h"

// Include this for glm::to_string() function
#define GLM_ENABLE_EXPERIMENTAL
#include <includes/gtx/string_cast.hpp>

// Include Pistol
#include "WeaponInfo\Pistol.h"

// Include CStructure3D
#include "Structure3D.h"
#include "Structure3D_2.h"
#include "WeaponInfo/Rifle.h"

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CScene3D::CScene3D(void)
	: cSettings(NULL)
	, cShader(NULL)
	, cSimpleShader(NULL)
	, cTextRenderer(NULL)
	, cGUIShader(NULL)
	, cGUISimpleShader(NULL)
	, cRenderToTextureShader(NULL)
	, cKeyboardController(NULL)
	, cMouseController(NULL)
	, cJoystickController(NULL)
	, cFPSCounter(NULL)
	, cSoundController(NULL)
	, cEntityManager(NULL)
	, cPlayer3D(NULL)
	, cCamera(NULL)
	, cSkyBox(NULL)
	, cGroundMap(NULL)
	, cCameraEffects(NULL)
	, cProgressBar(NULL)
	, cMinimap(NULL)
	, cCrossHair(NULL)
	, cWeaponInfo(NULL)
	, renderBoss(false)
	, printLoseScreen(false)
	, printWinScreen(false)
	, bossDED(false)
{
}

/**
 @brief Destructor
 */
CScene3D::~CScene3D(void)
{
	// Destroy the cMinimap
	if (cCrossHair)
	{
		cCrossHair->Destroy();
		cCrossHair = NULL;
	}

	// Destroy the cMinimap
	if (cMinimap)
	{
		cMinimap->Destroy();
		cMinimap = NULL;
	}
	
	// Destroy the cCameraEffects
	if (cProgressBar)
	{
		delete cProgressBar;
		cProgressBar = NULL;
	}

	// Destroy the cCameraEffects
	if (cCameraEffects)
	{
		cCameraEffects->Destroy();
		cCameraEffects = NULL;
	}

	// Destroy the cGroundMap
	if (cGroundMap)
	{
		cGroundMap->Destroy();
		cGroundMap = NULL;
	}

	// Destroy the cSkyBox
	if (cSkyBox)
	{
		cSkyBox->Destroy();
		cSkyBox = NULL;
	}

	// Destroy the camera
	if (cCamera)
	{
		cCamera->Destroy();
		cCamera = NULL;
	}

	// Destroy the cPlayer3D
	if (cPlayer3D)
	{
		cPlayer3D->Destroy();
		cPlayer3D = NULL;
	}

	// Destroy the cEntityManager
	if (cEntityManager)
	{
		cEntityManager->Destroy();
		cEntityManager = NULL;
	}
	// Destroy the cSoundController
	if (cSoundController)
	{
		cSoundController->Destroy();
		cSoundController = NULL;
	}

	// We won't delete this since it was created elsewhere
	cFPSCounter = NULL;

	// Destroy the cJoystickController
	if (cJoystickController)
	{
		cJoystickController->Destroy();
		cJoystickController = NULL;
	}

	// We won't delete this since it was created elsewhere
	cMouseController = NULL;

	// We won't delete this since it was created elsewhere
	cKeyboardController = NULL;
	

	if (cTextRenderer)
	{
		cTextRenderer->Destroy();
		cTextRenderer = NULL;
	}

	if (cRenderToTextureShader)
	{
		delete cRenderToTextureShader;
		cRenderToTextureShader = NULL;
	}
	if (cGUISimpleShader)
	{
		delete cGUISimpleShader;
		cGUISimpleShader = NULL;
	}
	if (cGUIShader)
	{
		delete cGUIShader;
		cGUIShader = NULL;
	}
	if (groundShader)
	{
		delete groundShader;
		groundShader = NULL;
	}
	if (skyBoxShader)
	{
		delete skyBoxShader;
		skyBoxShader = NULL;
	}
	if (textShader)
	{
		delete textShader;
		textShader = NULL;
	}

	if (cSimpleShader)
	{
		delete cSimpleShader;
		cSimpleShader = NULL;
	}

	if (cShader)
	{
		delete cShader;
		cShader = NULL;
	}

	if (cWeaponInfo)
	{
		cWeaponInfo = NULL;
	}

	// We won't delete this since it was created elsewhere
	cSettings = NULL;
}

/**
 @brief Init Initialise this instance
 @return true if the initialisation is successful, else false
 */ 
bool CScene3D::Init(void)
{
	cSettings = CSettings::GetInstance();

	srand(time(NULL));

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Setup the shaders
	cShader = new Shader("Shader//Scene3D.vs", "Shader//Scene3D.fs");
	// Setup the shaders
	cSimpleShader = new Shader("Shader//SimpleShader.vs", "Shader//SimpleShader.fs");
	// Setup the shaders
	textShader = new Shader("Shader//text.vs", "Shader//text.fs");
	// Setup the shaders
	skyBoxShader = new Shader("Shader//SkyBox.vs", "Shader//SkyBox.fs");
	// Setup the shaders
	groundShader = new Shader("Shader//Ground.vs", "Shader//Ground.fs");
	// Setup the shaders
	cGUIShader = new Shader("Shader//GUIShader.vs", "Shader//GUIShader.fs");
	// Setup the shaders
	cGUISimpleShader = new Shader("Shader//GUISimpleShader.vs", "Shader//GUISimpleShader.fs");
	// Setup the shaders
	cRenderToTextureShader = new Shader("Shader//RenderToTexture.vs", "Shader//RenderToTexture.fs");

	// Configure the camera
	cCamera = CCamera::GetInstance();
	cCamera->vec3Position = glm::vec3(0.0f, 0.5f, 3.0f);

	// Store the keyboard controller singleton instance here
	cKeyboardController = CKeyboardController::GetInstance();

	// Store the mouse controller singleton instance here
	cMouseController = CMouseController::GetInstance();

	// Store the joystick controller singleton instance here
	cJoystickController = CJoystickController::GetInstance();
	// Initialise the joystick controller. It will help us to find a joystick connected to your computer
	cJoystickController->Init();

	// Store the CFPSCounter singleton instance here
	cFPSCounter = CFPSCounter::GetInstance();

	// Load the sounds into CSoundController
	cSoundController = CSoundController::GetInstance();
	cSoundController->Init();

	cSoundController->LoadSound(FileSystem::getPath("Sounds\\damage.ogg"), 1, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\wallbump.ogg"), 2, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\pistolshot.ogg"), 3, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\rifle.ogg"), 4, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\losesound.ogg"), 5, true, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\winsound.ogg"), 6, true, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\bossmusic.ogg"), 7, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\pistolreload.ogg"), 8, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\riflereload.ogg"), 9, true);

	// Initialise the EntityManager
	cEntityManager = CEntityManager::GetInstance();
	cEntityManager->Init();

	// Initialise the cPlayer3D
	cPlayer3D = CPlayer3D::GetInstance();
	cPlayer3D->SetShader(cShader);
	cPlayer3D->Init();
	cPlayer3D->AttachCamera(cCamera);
	//cPlayer3D->SetScale(glm::vec3(0.5f));
	cPlayer3D->ActivateCollider(cSimpleShader);
	// Assign a cPistol to the cPlayer3D
	CPistol* cPistol = new CPistol();
	cPistol->Init();
	cPistol->SetShader(cSimpleShader);
	cPlayer3D->SetWeapon(0, cPistol);

	float randPos = rand() % 3 + 2;
	float randPos2 = rand() % 6 + 4;
	//float randPos3 = rand() % 8 + 7;

	// Initialise the cEnemy3D
	CEnemy3D* cEnemy3D = new CEnemy3D(glm::vec3(2, 10.f, 2));
	CEnemy3D* cEnemy3D2 = new CEnemy3D(glm::vec3(2, 10.f, 4));
	CEnemy3D* cEnemy3D3 = new CEnemy3D(glm::vec3(5, 10.f, 0));
	CEnemy3D* cEnemy3D4 = new CEnemy3D(glm::vec3(-2, 10.f, -2));
	CEnemy3D* cEnemy3D5 = new CEnemy3D(glm::vec3(-2, 10.f, -4));

	cEnemy3D->SetShader(cShader);
	cEnemy3D->Init();
	cEnemy3D->ActivateCollider(cSimpleShader);
	cEntityManager->Add(cEnemy3D);

	cEnemy3D2->SetShader(cShader);
	cEnemy3D2->Init();
	cEnemy3D2->ActivateCollider(cSimpleShader);
	cEntityManager->Add(cEnemy3D2);

	cEnemy3D3->SetShader(cShader);
	cEnemy3D3->Init();
	cEnemy3D3->ActivateCollider(cSimpleShader);
	cEntityManager->Add(cEnemy3D3);

	cEnemy3D4->SetShader(cShader);
	cEnemy3D4->Init();
	cEnemy3D4->ActivateCollider(cSimpleShader);
	cEntityManager->Add(cEnemy3D4);

	cEnemy3D5->SetShader(cShader);
	cEnemy3D5->Init();
	cEnemy3D5->ActivateCollider(cSimpleShader);
	cEntityManager->Add(cEnemy3D5);

	// Initialise a CStructure3D
	CStructure3D* cStructure3D = new CStructure3D(glm::vec3(10.f, 0.5f, 0.0f));
	CStructure3D* cStructure3D2 = new CStructure3D(glm::vec3(-10.f, 0.5f, 0.0f));
	CStructure3D* cStructure3D3 = new CStructure3D(glm::vec3(0.0f, 0.5f, -10.f));
	CStructure3D* cStructure3D4 = new CStructure3D(glm::vec3(0.0f, 0.5f, 10.f));

	cStructure3D->SetShader(cShader);
	cStructure3D->Init();
	cStructure3D->SetScale(glm::vec3(1, 5, 100));
	cStructure3D->ActivateCollider(cSimpleShader);
	cEntityManager->Add(cStructure3D);

	cStructure3D2->SetShader(cShader);
	cStructure3D2->Init();
	cStructure3D2->SetScale(glm::vec3(1, 5, 100));
	cStructure3D2->ActivateCollider(cSimpleShader);
	cEntityManager->Add(cStructure3D2);

	cStructure3D3->SetShader(cShader);
	cStructure3D3->Init();
	cStructure3D3->SetScale(glm::vec3(100, 5, 1));
	cStructure3D3->ActivateCollider(cSimpleShader);
	cEntityManager->Add(cStructure3D3);

	cStructure3D4->SetShader(cShader);
	cStructure3D4->Init();
	cStructure3D4->SetScale(glm::vec3(100, 5, 1));
	cStructure3D4->ActivateCollider(cSimpleShader);
	cEntityManager->Add(cStructure3D4);

	CStructure2_3D* cStructure3D_2 = new CStructure2_3D(glm::vec3(6, 0.5, 6));
	CStructure2_3D* cStructure3D_22 = new CStructure2_3D(glm::vec3(6, 0.5, -6));
	CStructure2_3D* cStructure3D_23 = new CStructure2_3D(glm::vec3(-6, 0.5, 6));
	CStructure2_3D* cStructure3D_24 = new CStructure2_3D(glm::vec3(-6, 0.5, -6));

	CStructure2_3D* cStructure3D_25 = new CStructure2_3D(glm::vec3(-6, 0.5, 0));
	CStructure2_3D* cStructure3D_26 = new CStructure2_3D(glm::vec3(6, 0.5, 0));
	CStructure2_3D* cStructure3D_27 = new CStructure2_3D(glm::vec3(0, 0.5, -6));
	CStructure2_3D* cStructure3D_28 = new CStructure2_3D(glm::vec3(0, 0.5, 6));

	cStructure3D_2->SetShader(cShader);
	cStructure3D_2->Init();
	cStructure3D_2->SetScale(glm::vec3(0.5, 5, 0.5));
	cStructure3D_2->ActivateCollider(cSimpleShader);
	cEntityManager->Add(cStructure3D_2);

	cStructure3D_22->SetShader(cShader);
	cStructure3D_22->Init();
	cStructure3D_22->SetScale(glm::vec3(0.5, 5, 0.5));
	cStructure3D_22->ActivateCollider(cSimpleShader);
	cEntityManager->Add(cStructure3D_22);

	cStructure3D_23->SetShader(cShader);
	cStructure3D_23->Init();
	cStructure3D_23->SetScale(glm::vec3(0.5, 5, 0.5));
	cStructure3D_23->ActivateCollider(cSimpleShader);
	cEntityManager->Add(cStructure3D_23);

	cStructure3D_24->SetShader(cShader);
	cStructure3D_24->Init();
	cStructure3D_24->SetScale(glm::vec3(0.5, 5, 0.5));
	cStructure3D_24->ActivateCollider(cSimpleShader);
	cEntityManager->Add(cStructure3D_24);

	cStructure3D_25->SetShader(cShader);
	cStructure3D_25->Init();
	cStructure3D_25->SetScale(glm::vec3(0.5, 5, 0.5));
	cStructure3D_25->ActivateCollider(cSimpleShader);
	cEntityManager->Add(cStructure3D_25);

	cStructure3D_26->SetShader(cShader);
	cStructure3D_26->Init();
	cStructure3D_26->SetScale(glm::vec3(0.5, 5, 0.5));
	cStructure3D_26->ActivateCollider(cSimpleShader);
	cEntityManager->Add(cStructure3D_26);

	cStructure3D_27->SetShader(cShader);
	cStructure3D_27->Init();
	cStructure3D_27->SetScale(glm::vec3(0.5, 5, 0.5));
	cStructure3D_27->ActivateCollider(cSimpleShader);
	cEntityManager->Add(cStructure3D_27);

	cStructure3D_28->SetShader(cShader);
	cStructure3D_28->Init();
	cStructure3D_28->SetScale(glm::vec3(0.5, 5, 0.5));
	cStructure3D_28->ActivateCollider(cSimpleShader);
	cEntityManager->Add(cStructure3D_28);

	// Load the SkyBox
	cSkyBox = CSkyBox::GetInstance();
	// Set a shader to this class instance of CSkyBox
	cSkyBox->SetShader(skyBoxShader);
	cSkyBox->Init();

	// Load the Ground
	cGroundMap = CGroundMap::GetInstance();
	cGroundMap->SetShader(groundShader);
	cGroundMap->Init(glm::vec3(50.0f, 1.0f, 50.0f), glm::i32vec3(10, 1, 10));
	// Set a shader to this class instance of CSkyBox
	cGroundMap->ReCalculate();

	// Create and initialise the TextRenderer
	cTextRenderer = CTextRenderer::GetInstance();
	// Set a shader to this class
	cTextRenderer->SetShader(textShader);
	// Initialise the instance
	if (cTextRenderer->Init() == false)
	{
		cout << "Failed to load CTextRenderer" << endl;
		return false;
	}

	// Load the CameraEffects
	cCameraEffects = CCameraEffects::GetInstance();
	// Set a shader to this class instance of CameraEffects
	cCameraEffects->SetShader(cGUIShader);
	cCameraEffects->Init();

	// Load the ProgressBar
	cProgressBar = new CProgressBar();
	// Set a shader to this class instance of CameraEffects
	cProgressBar->SetShader(cGUISimpleShader);
	cProgressBar->Init();

	cEntityManager->set_ProgressBar(cProgressBar);

	// Load the Minimap
	cMinimap = CMinimap::GetInstance();
	// Set a shader to this class instance of Minimap
	cMinimap->SetShader(cRenderToTextureShader);
	cMinimap->Init();

	// Load the cCrossHair
	cCrossHair = CCrossHair::GetInstance();
	// Set a shader to this class instance of CameraEffects
	cCrossHair->SetShader(cGUIShader);
	cCrossHair->Init();

	return true;
}

/**
@brief Update Update this instance
*/
void CScene3D::Update(const double dElapsedTime)
{
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_0))
	{
		if (cPlayer3D->IsCameraAttached())
			cPlayer3D->AttachCamera();
		else
			cPlayer3D->AttachCamera(cCamera);

		// Reset the key so that it will not repeat until the key is released and pressed again
		CKeyboardController::GetInstance()->ResetKey(GLFW_KEY_0);
	}
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_9))
	{
		// Toggle the crosshair on or off
		if (cCrossHair->GetStatus() == false)
			cCrossHair->SetStatus(true);
		else
			cCrossHair->SetStatus(false);
		
		// Reset the key so that it will not repeat until the key is released and pressed again
		CKeyboardController::GetInstance()->ResetKey(GLFW_KEY_9);
	}

	if (printLoseScreen == true || printWinScreen == true)
	{
		cSoundController->LoadSound(FileSystem::getPath("Sounds\\dud.ogg"), 1, true);
		cSoundController->LoadSound(FileSystem::getPath("Sounds\\dud.ogg"), 2, true);
		cSoundController->LoadSound(FileSystem::getPath("Sounds\\dud.ogg"), 3, true);
		cSoundController->LoadSound(FileSystem::getPath("Sounds\\dud.ogg"), 4, true);
		cSoundController->LoadSound(FileSystem::getPath("Sounds\\dud.ogg"), 7, true);
	}

	// Store the current position, if rollback is needed.
	cPlayer3D->StorePositionForRollback();

	// Update the joystick
	cJoystickController->Update(cJoystickController->GetJoystickID());

	// Get keyboard updates for player3D
	if ((CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_W))
		|| (cJoystickController->IsJoystickActivated(CJoystickController::FORWARD)))
		cPlayer3D->ProcessMovement(CPlayer3D::FORWARD, (float)dElapsedTime);
	if ((CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_S))
		|| (cJoystickController->IsJoystickActivated(CJoystickController::BACKWARD)))
		cPlayer3D->ProcessMovement(CPlayer3D::BACKWARD, (float)dElapsedTime);
	if ((CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_A))
		|| (cJoystickController->IsJoystickActivated(CJoystickController::LEFT)))
		cPlayer3D->ProcessMovement(CPlayer3D::LEFT, (float)dElapsedTime);
	if ((CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_D))
		|| (cJoystickController->IsJoystickActivated(CJoystickController::RIGHT)))
		cPlayer3D->ProcessMovement(CPlayer3D::RIGHT, (float)dElapsedTime);
	if ((CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_W) && CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
		|| (cJoystickController->IsJoystickActivated(CJoystickController::FORWARD)))
		cPlayer3D->ProcessMovement(CPlayer3D::SPRINT, (float)dElapsedTime);
	if ((CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_SPACE))
		|| (cJoystickController->IsButtonDown(0))) // Button 0 is the A button on the GamePad
		cPlayer3D->SetToJump();

	// Get keyboard updates for camera
	if (!cPlayer3D->IsCameraAttached())
	{
		if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_I))
			cCamera->ProcessKeyboard(CCamera::FORWARD, (float)dElapsedTime);
		if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_K))
			cCamera->ProcessKeyboard(CCamera::BACKWARD, (float)dElapsedTime);
		if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_J))
			cCamera->ProcessKeyboard(CCamera::LEFT, (float)dElapsedTime);
		if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_L))
			cCamera->ProcessKeyboard(CCamera::RIGHT, (float)dElapsedTime);
	}

	if (cPlayer3D->IsCameraAttached())
	{
		// Get mouse updates
		cPlayer3D->ProcessRotate((float)cMouseController->GetMouseDeltaX(), (float)cMouseController->GetMouseDeltaY());
		cCamera->ProcessMouseScroll((float)cMouseController->GetMouseScrollStatus(CMouseController::SCROLL_TYPE::SCROLL_TYPE_YOFFSET));
	}
	else
	{
		// Get mouse updates
		cCamera->ProcessMouseMovement((float)cMouseController->GetMouseDeltaX(), (float)cMouseController->GetMouseDeltaY());
		cCamera->ProcessMouseScroll((float)cMouseController->GetMouseScrollStatus(CMouseController::SCROLL_TYPE::SCROLL_TYPE_YOFFSET));
	}

	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_1))
	{
		cPlayer3D->SetCurrentWeapon(0);
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_2))
	{
		cPlayer3D->SetCurrentWeapon(1);
	}
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_R))
	{
		if (cWeaponInfo->type == CWeaponInfo::WeaponType::PISTOL)
		{
			cSoundController->PlaySoundByID(8);
		}
		else
		{
			cSoundController->PlaySoundByID(9);
		}

		cPlayer3D->GetWeapon()->Reload();
	}

	// Get mouse button updates
	if ((cMouseController->IsButtonReleased(CMouseController::BUTTON_TYPE::LMB))
		|| (cJoystickController->IsButtonDown(2))) // Button 2 is the X button on the GamePad
	{
		// Try to create a projectile using the primary weapon, 0
		CProjectile* cProjectile = cPlayer3D->DischargeWeapon();
		if (cWeaponInfo->type == CWeaponInfo::WeaponType::PISTOL)
		{
			if (cWeaponInfo->GetMagRound() > 0)
			{
				cCamera->fPitch = 90;
				cCamera->fYaw = 90;
				cSoundController->PlaySoundByID(3);
			}
		}
		else
		{
			if (cWeaponInfo->GetMagRound() > 0)
			{
				cSoundController->PlaySoundByID(4);
			}
		}
		// If the projectile was successfully created then add to the EntityManager
		if (cProjectile)
			cEntityManager->Add(cProjectile);
	}
	else if (cMouseController->IsButtonDown(CMouseController::BUTTON_TYPE::RMB))
	{
		cCamera->fZoom = 25.f;
		// Scope mode
	}
	else if (cMouseController->IsButtonUp(CMouseController::BUTTON_TYPE::RMB))
	{
		cCamera->fZoom = 45.f;
	}

	if (cEntityManager->get_enemy_deathCount() > 4)
	{
		renderBoss = true;
	}

	if (renderBoss == true)
	{
		//cSoundController->PlaySoundByID(7);
		//cSoundController->VolumeDecrease(7);

		CEnemyBoss3D* cEnemyBoss3D = new CEnemyBoss3D(glm::vec3(4.f, 40.f, -4.f));

		cEnemyBoss3D->SetShader(cShader);
		cEnemyBoss3D->Init();
		cEnemyBoss3D->ActivateCollider(cSimpleShader);
		cEntityManager->Add(cEnemyBoss3D);

		CEnemyBoss3D* cEnemyBoss3D_2 = new CEnemyBoss3D(glm::vec3(-4.f, 40.f, 4.f));

		cEnemyBoss3D_2->SetShader(cShader);
		cEnemyBoss3D_2->Init();
		cEnemyBoss3D_2->ActivateCollider(cSimpleShader);
		cEntityManager->Add(cEnemyBoss3D_2);

		CRifle* cRifle = new CRifle();
		cRifle->Init();
		cRifle->SetShader(cSimpleShader);
		cPlayer3D->SetWeapon(1, cRifle);

		cEntityManager->set_enemy_deathCount(0);
		renderBoss = false;
		bossDED = true;
	}

	// WIN LOSE CONDITIONS
	if (cEntityManager->get_enemy_deathCount() > 1 && bossDED == true && printWinScreen == false)
	{
		cSoundController->PlaySoundByID(6);
		printWinScreen = true;
	}

	if (static_cast<CProgressBar*>(cProgressBar)->get_healthBarLength() <= 0 && printLoseScreen == false)
	{
		cSoundController->PlaySoundByID(5);
		printLoseScreen = true;
	}

	// Post Update the mouse controller
	cMouseController->PostUpdate();

	// Update the Entities
	cEntityManager->Update(dElapsedTime);

	// Check for Player3D colliding with Entities
	if (cEntityManager->CollisionCheck(cPlayer3D) == true)
	{
		cCameraEffects->Activate_BloodScreen();
	}

	// Clean up the deleted CEntity3D in the entity manager
	cEntityManager->CleanUp();

	// Update the player3D
	cPlayer3D->Update(dElapsedTime);

	// Update camera effects
	cCameraEffects->Update(dElapsedTime);

	// Update progress bar
	cProgressBar->Update(dElapsedTime);

	cWeaponInfo = cPlayer3D->GetWeapon();
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CScene3D::PreRender(void)
{
	// Reset the OpenGL rendering environment
	glLoadIdentity();

	// Clear the screen and buffer
	glClearColor(0.0f, 0.1f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 @brief Render Render this instance
 */
void CScene3D::Render(void)
{
	// Part 1: Render for the minimap by binding to framebuffer and render to color texture
	//         But the camera is move to top-view of the scene

	// Backup some key settings for the camera and player
	glm::vec3 storePlayerPosition = cPlayer3D->GetPosition();
	float storeCameraYaw = cCamera->fYaw;
	float storeCameraPitch = cCamera->fPitch;
	glm::vec3 storeCameraPosition = cCamera->vec3Position;
	// Adjust camera yaw and pitch so that it is looking from a top-view of the terrain
	cCamera->fYaw += 180.0f;
	cCamera->fPitch = -90.0f;
	// We store the player's position into the camera as we want the minimap to focus on the player
	cCamera->vec3Position = glm::vec3(storePlayerPosition.x, 10.0f, storePlayerPosition.z);
	// Recalculate all the camera vectors. 
	// We disable pitch constrains for this specific case as we want the camera to look straight down
	cCamera->ProcessMouseMovement(0, 0, false);
	// Generate the view and projection
	glm::mat4 playerView = cCamera->GetViewMatrix();
	glm::mat4 playerProjection = glm::perspective(	glm::radians(45.0f),
													(float)cSettings->iWindowWidth / (float)cSettings->iWindowHeight,
													0.1f, 1000.0f);

	// Set the camera parameters back to the previous values
	cCamera->fYaw = storeCameraYaw;
	cCamera->fPitch = storeCameraPitch;
	cCamera->vec3Position = storeCameraPosition;
	cCamera->ProcessMouseMovement(0, 0, true); // call this to make sure it updates its camera vectors, note that we disable pitch constrains for this specific case (otherwise we can't reverse camera's pitch values)

	// Activate the minimap system
	cMinimap->Activate();
	// Setup the rendering environment
	cMinimap->PreRender();

	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

	// Render the Ground for the minimap
	cGroundMap->SetView(playerView);
	cGroundMap->SetProjection(playerProjection);
	cGroundMap->PreRender();
	cGroundMap->Render();
	cGroundMap->PostRender();

	// Render the entities for the minimap
	cEntityManager->SetView(playerView);
	cEntityManager->SetProjection(playerProjection);
	cEntityManager->Render();

	// Render the player for the minimap
	cPlayer3D->SetView(playerView);
	cPlayer3D->SetProjection(playerProjection);
	cPlayer3D->PreRender();
	cPlayer3D->Render();
	cPlayer3D->PostRender();

	// Deactivate the cMinimap so that we can render as per normal
	cMinimap->Deactivate();

	// Part 2: Render the entire scene as per normal
	// Get the camera view and projection
	glm::mat4 view = CCamera::GetInstance()->GetViewMatrix();;
	glm::mat4 projection = glm::perspective(	glm::radians(CCamera::GetInstance()->fZoom),
												(float)cSettings->iWindowWidth / (float)cSettings->iWindowHeight,
												0.1f, 1000.0f);
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render the SkyBox
	cSkyBox->SetView(view);
	cSkyBox->SetProjection(projection);
	cSkyBox->PreRender();
	cSkyBox->Render();
	cSkyBox->PostRender();

	// Render the Ground
	cGroundMap->SetView(view);
	cGroundMap->SetProjection(projection);
	cGroundMap->PreRender();
	cGroundMap->Render();
	cGroundMap->PostRender();

	// Render the entities
	cEntityManager->SetView(view);
	cEntityManager->SetProjection(projection);
	cEntityManager->Render();

	// Render the player
	cPlayer3D->SetView(view);
	cPlayer3D->SetProjection(projection);
	cPlayer3D->PreRender();
	cPlayer3D->Render();
	cPlayer3D->PostRender();

	// now draw the mirror quad with screen texture
	// --------------------------------------------
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

	cCameraEffects->PreRender();
	cCameraEffects->Render();
	cCameraEffects->PostRender();

	cProgressBar->PreRender();
	cProgressBar->Render();
	cProgressBar->PostRender();

	cCrossHair->PreRender();
	cCrossHair->Render(cPlayer3D->GetWeapon());
	cCrossHair->PostRender();

	cMinimap->Render();

	// Call the cTextRenderer's PreRender()
	cTextRenderer->PreRender();

	// Call the CTextRenderer's Render()
	textShader->use();
	//cTextRenderer->Render("DM2231 GDEV 2D", 10.0f, 10.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.0f));
	// Render FPS info
	//	cTextRenderer->Render(cFPSCounter->GetFrameRateString(), 10.0f, 580.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.0f));
	// Render Camera Position
	cTextRenderer->Render(glm::to_string(cPlayer3D->GetPosition()), 10.0f, 30.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.0f));
	// Render Camera Position
	cTextRenderer->Render(glm::to_string(cCamera->vec3Position), 10.0f, 10.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.0f));
	if (cWeaponInfo->type == CWeaponInfo::WeaponType::PISTOL)
	{
		cTextRenderer->Render("PISTOL", 70, 535, 1, glm::vec3(0, 0, 0));
	}
	else
	{
		cTextRenderer->Render("RIFLE", 70, 535, 1, glm::vec3(0, 0, 0));
	}
	cTextRenderer->Render(std::to_string(cWeaponInfo->GetMagRound()) + "/" + std::to_string(cWeaponInfo->GetMaxMagRound()), 100, 500, 1, glm::vec3(0, 0, 0));

	// Call the cTextRenderer's PostRender()
	cTextRenderer->PostRender();

	return;
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CScene3D::PostRender(void)
{
}