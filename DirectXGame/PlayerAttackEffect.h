#pragma once
#include "Camera.h"
#include "GPUParticle.h"
#include "SkinningObject.h"
#include "Matrix44.h"
#include <map>
#include <memory>

class PlayerAttackEffect {
public:

	void Init();

	void Update();

	void Draw(const DaiEngine::Camera& camera);

	void Start(const Vector3& pos, const float angle);

private:

	std::unique_ptr<DaiEngine::SkinningObject> obj_;
	std::map<std::string, std::unique_ptr<DaiEngine::GPUParticle>> effect_;

	bool isEffect_ = false;
	Matrix4x4 rotateMat_ = MakeIdentity44();
	
	float timer_ = 0.0f;
	const float kDeltaTime_ = 1.0f / 60.0f;
	const float kEffectTime_ = 2.0f;
	

};

