/**
 CPhysics3D
 By: Toh Da Jun
 Date: Mar 2020
 */
#include "Physics3D.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CPhysics3D::CPhysics3D(void)
	: v3InitialVelocity(glm::vec3(0.0f))
	, v3FinalVelocity(glm::vec3(0.0f))
	, v3Acceleration(glm::vec3(0.0f))
	, v3Displacement(glm::vec3(0.0f))
	, fTotalElapsedTime(0.0f)
	, fElapsedTime(0.0f)
	, sCurrentStatus(STATUS::IDLE)
{
}

/**
 @brief Destructor This destructor has protected access modifier as this class will be a Singleton
 */
CPhysics3D::~CPhysics3D(void)
{
}

/**
@brief Init Initialise this instance
*/ 
bool CPhysics3D::Init(void)
{
	// Reset these variables
	v3InitialVelocity = glm::vec3(0.0f);
	v3FinalVelocity = glm::vec3(0.0f);
	v3Acceleration = glm::vec3(0.0f);		// Acceleration does not need to be reset here.
	v3Displacement = glm::vec3(0.0f);
	fTotalElapsedTime = 0.0f;
	fElapsedTime = 0.0f;
	return true;
}

// Set methods
// Set Initial velocity
void CPhysics3D::SetInitialVelocity(const glm::vec3 v3InitialVelocity)
{
	this->v3InitialVelocity = v3InitialVelocity;	// Initial velocity
}

// Set Final velocity
void CPhysics3D::SetFinalVelocity(const glm::vec3 v3FinalVelocity)
{
	this->v3FinalVelocity = v3FinalVelocity;		// Final velocity
}

// Set Acceleration
void CPhysics3D::SetAcceleration(const glm::vec3 v3Acceleration)
{
	this->v3Acceleration = v3Acceleration;		// Acceleration
}

// Set Displacement
void CPhysics3D::SetDisplacement(const glm::vec3 v3Displacement)
{
	this->v3Displacement = v3Displacement;		// Displacement
}

// Set Total Elapsed Time
void CPhysics3D::SetTotalElapsedTime(const float fTotalElapsedTime)
{
	this->fTotalElapsedTime = fTotalElapsedTime;	// Total Elapsed Time
}

// Set Elapsed Time
void CPhysics3D::SetElapsedTime(const float fElapsedTime)
{
	this->fElapsedTime = fElapsedTime;	// Total Elapsed Time
}

// Set Status
void CPhysics3D::SetStatus(const STATUS sStatus)
{
	// If there is a change in status, then reset to default values
	if (sCurrentStatus != sStatus)
	{
		// Reset to default values
		Init();

		// Store the new status
		sCurrentStatus = sStatus;
	}
}

// Get methods
// Get Initial velocity
glm::vec3 CPhysics3D::GetInitialVelocity(void) const
{
	return v3InitialVelocity;	// Initial velocity
}

// Get Final velocity
glm::vec3 CPhysics3D::GetFinalVelocity(void) const
{
	return v3FinalVelocity;		// Final velocity
}

// Get Acceleration
glm::vec3 CPhysics3D::GetAcceleration(void) const
{
	return v3Acceleration;		// Acceleration
}

// Get Displacement
glm::vec3 CPhysics3D::GetDisplacement(void) const
{
	return v3Displacement;		// Displacement
}

// Get Time
float CPhysics3D::GetTotalElapsedTime(void) const
{
	return fTotalElapsedTime;	// Total Elapsed Time
}

// Get Status
CPhysics3D::STATUS CPhysics3D::GetStatus(void) const
{
	return sCurrentStatus;
}

// Update
void CPhysics3D::Update(void)
{
	if (sCurrentStatus == JUMP)
	{
		// Calculate the final velocity
		v3FinalVelocity = v3InitialVelocity + (v3Acceleration + v3Gravity)* fElapsedTime;
		// Calculate the updated acceleration
		v3Acceleration = (v3FinalVelocity - v3InitialVelocity) / fElapsedTime;
		// Calculate the displacement
		v3Displacement = v3FinalVelocity * fElapsedTime - 
							0.5f * (v3Acceleration + v3Gravity) * fElapsedTime * fElapsedTime;
		// Update v3InitialVelocity
		v3InitialVelocity = v3FinalVelocity;

	}
	else if (sCurrentStatus == FALL)
	{
		// Calculate the final velocity
		v3FinalVelocity = v3InitialVelocity + v3Gravity * fElapsedTime;
		// Calculate the displacement
		v3Displacement = v3FinalVelocity * fElapsedTime - 0.5f * v3Gravity * fElapsedTime * fElapsedTime;
		// Update v3InitialVelocity
		v3InitialVelocity = v3FinalVelocity;
	}
}

// Add elapsed time
void CPhysics3D::AddElapsedTime(const float fElapsedTime)
{
	this->fElapsedTime = fElapsedTime;
	fTotalElapsedTime += fElapsedTime;
}

// Calculate the distance between two vec2 varables
float CPhysics3D::CalculateDistance(glm::vec3 source, glm::vec3 destination)
{
	return glm::length(destination - source);
}

// PrintSelf
void CPhysics3D::PrintSelf(void)
{
	cout << "CPhysics3D::PrintSelf()" << endl;
	cout << "v3InitialVelocity\t=\t[" << v3InitialVelocity.x << ", " << v3InitialVelocity.y << "]" << endl;
	cout << "v3FinalVelocity\t=\t[" << v3FinalVelocity.x << ", " << v3FinalVelocity.y << "]" << endl;
	cout << "v3Acceleration\t=\t[" << v3Acceleration.x << ", " << v3Acceleration.y << "]" << endl;
	cout << "v3Displacement\t=\t[" << v3Displacement.x << ", " << v3Displacement.y << "]" << endl;
	cout << "fTotalElapsedTime\t=\t" << fTotalElapsedTime << endl;
	cout << "fElapsedTime\t=\t" << fElapsedTime << endl;
}
