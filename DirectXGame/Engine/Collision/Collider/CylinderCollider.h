#pragma once
#include "Collider.h"
#include "Matrix44.h"

namespace DaiEngine {
	class CylinderCollider : public Collider {
	public:

		void Init(const std::string& tag, const WorldTransform& parent, float radius , float height);

		void Update(const Matrix4x4& rotateMat);

		void SetRadius(float radius) { cylinder_->radius = radius; }
		const float GetRadius() const { return cylinder_->radius; }
		
		void SetHeight(float height) { height_ = height; }
		const float GetHeight() const { return height_; }

	private:

		Shapes::Cylinder* cylinder_ = nullptr;

		float height_ = 0.0f;

	};
}
