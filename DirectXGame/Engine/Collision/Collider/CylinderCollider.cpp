#include "CylinderCollider.h"

namespace DaiEngine {
	void CylinderCollider::Init(const std::string& tag, const WorldTransform& parent, float radius) {

		tag_ = tag;
		worldTransform_.Init();
		shapeType_ = Type::Cylinder;

		worldTransform_.parent_ = &parent;

		shape_ = Shapes::Cylinder();

		cylinder_ = std::get_if<Shapes::Cylinder>(&shape_);
		cylinder_->radius = radius;

	}

	void CylinderCollider::Update(const Matrix4x4& rotateMat) {

		worldTransform_.UpdateMatrixRotate(rotateMat);

		//ローカル空間での円柱の上端と下端の位置
		Vector3 localTopCenter{ 0, 0.5f, 0 };
		Vector3 localBottomCenter{ 0, -0.5f, 0 };

		//スケールを適用（Y軸方向のスケールを高さに反映）
		localTopCenter = localTopCenter * height_;//Y軸方向のスケール
		localBottomCenter = localBottomCenter * height_;//Y軸方向のスケール

		//平行移動を適用
		cylinder_->topCenter = localTopCenter + worldTransform_.translation_;
		cylinder_->bottomCenter = localBottomCenter + worldTransform_.translation_;
	}
}
