#pragma once
#include "Vec3.h"
#include "Matrix44.h"
#include "Quaternion.h"
#include <vector>
#include <string>
#include <optional>
#include "ModelManager.h"
#include "WorldTransform.h"
#include "Camera.h"


namespace DaiEngine {
	class Skeleton {
	public:

		struct Joint {
			QuaternionTransform transform_;
			Matrix4x4 localMat_;
			Matrix4x4 skeletonSpaceMat_;
			std::string name_;
			std::vector<int32_t> children_; //子JointのIndexのリスト。いなければ空
			int32_t index_; //自身のindex
			std::optional<int32_t> parent_; //親Jointのindex。いなければnull
		};

	public:

		static Skeleton Create(const Model::Node& rootNode);

		void Update();

		void Draw(const WorldTransform& worldTransform, const Camera& camera);

		Vector3 GetSkeletonPos(const std::string& jointName);

		Vector3 GetWorldPosition(const std::string& jointName, const WorldTransform& worldTransform);

	private:
		//ジョイント生成
		static int32_t CreateJoint(const Model::Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);

	public:

		int32_t root_; //RootJointのIndex
		std::map<std::string, int32_t> jointMap_; //Joint名とIndexとの辞書
		std::vector<Joint> joints_;
	};
}


