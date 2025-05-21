#include "Skeleton.h"

#include "Line.h"

namespace DaiEngine {
	Skeleton Skeleton::Create(const Model::Node& rootNade) {
		Skeleton skeleton;
		skeleton.root_ = CreateJoint(rootNade, {}, skeleton.joints_);

		for (const Joint& joint : skeleton.joints_) {
			skeleton.jointMap_.emplace(joint.name_, joint.index_);
		}

		return skeleton;
	}

	int32_t Skeleton::CreateJoint(const Model::Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints) {

		Joint joint;
		joint.name_ = node.name_;
		joint.localMat_ = node.localMatrix_;
		joint.skeletonSpaceMat_ = MakeIdentity44();
		joint.transform_ = node.transform_;
		joint.index_ = int32_t(joints.size());
		joint.parent_ = parent;
		joints.push_back(joint);
		for (const Model::Node& child : node.children_) {
			int32_t childIndex = CreateJoint(child, joint.index_, joints);
			joints[joint.index_].children_.push_back(childIndex);
		}

		return joint.index_;
	}

	void Skeleton::Update() {

		for (Joint& joint : joints_) {
			joint.localMat_ = MakeScaleMatrix(joint.transform_.scale_) * joint.transform_.rotate_.MakeRotateMatrix() * MakeTranslateMatrix(joint.transform_.translate_);
			if (joint.parent_) {
				joint.skeletonSpaceMat_ = joint.localMat_ * joints_[*joint.parent_].skeletonSpaceMat_;
			}
			else {
				joint.skeletonSpaceMat_ = joint.localMat_;
			}
		}
	}

	void Skeleton::Draw(const WorldTransform& worldTransform, const Camera& camera) {
#ifdef _DEBUG
		for (auto& joint : joints_) {

			if (!joint.parent_) { continue; }

			Matrix4x4 startPointMat = joint.skeletonSpaceMat_ * worldTransform.matWorld_;
			Matrix4x4 endPointMat = joints_[*joint.parent_].skeletonSpaceMat_ * worldTransform.matWorld_;

			Vector3 start = {
				startPointMat.m[3][0],
				startPointMat.m[3][1],
				startPointMat.m[3][2],
			};

			Vector3 end = {
				endPointMat.m[3][0],
				endPointMat.m[3][1],
				endPointMat.m[3][2],
			};

			Line::Draw(start, end, camera);

		}
#endif // _DEBUG
	}

	Vector3 Skeleton::GetSkeletonPos(const std::string& jointName) {

		int32_t jointIndex = jointMap_[jointName];

		Vector3 jointPos = {
			joints_[jointIndex].skeletonSpaceMat_.m[3][0],
			joints_[jointIndex].skeletonSpaceMat_.m[3][1],
			joints_[jointIndex].skeletonSpaceMat_.m[3][2],
		};

		return jointPos;
	}
}