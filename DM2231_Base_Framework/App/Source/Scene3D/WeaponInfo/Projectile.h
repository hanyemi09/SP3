#pragma once

// Include Entity3D
#include <Primitives/Entity3D.h>

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

class CProjectile : public CEntity3D
{
public:
	// Default Constructor
	CProjectile(void);
	// Destructor
	virtual ~CProjectile(void);

	// Init the projectile
	bool Init(	glm::vec3 vec3Position,
				glm::vec3 vec3Front,
				const double dLifetime,
				const float fSpeed);

	// Set model
	virtual void SetModel(glm::mat4 model);
	// Set view
	virtual void SetView(glm::mat4 view);
	// Set projection
	virtual void SetProjection(glm::mat4 projection);

	// Activate the CCollider for this class instance
	virtual void ActivateCollider(Shader* cLineShader = NULL);

	// Activate the projectile. true == active, false == inactive
	void SetStatus(const bool bStatus);
	void SetDirection(glm::vec3 vec3Front);
	// Set the lifetime of the projectile
	void SetLifetime(const double dLifetime);
	// Set the speed of the projectile
	void SetSpeed(const float fSpeed);
	// Set the source of the projectile
	void SetSource(CEntity3D* pSource);

	// get status of the projectile. true == active, false == inactive
	bool GetStatus(void) const;
	// Get the direction of the projectile
	glm::vec3 GetDirection(void);
	// Get the lifetime of the projectile
	double GetLifetime(void) const;
	// Get the speed of the projectile
	float GetSpeed(void) const;
	// Get the source of the projectile
	CEntity3D* GetSource(void) const;

	// Update the status of this projectile
	virtual void Update(const double dElapsedTime);
	// PreRender
	virtual void PreRender(void);
	// Render this projectile
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

protected:
	// Boolean flag to indicate if this projectile is active. If not active, then do not compute/update
	bool bStatus;
	// Remaining lifetime in seconds
	double dLifetime;
	// Speed of this projectile in unit / sec
	float fSpeed;
	// The source which fired this projectile
	CEntity3D* pSource;
};

