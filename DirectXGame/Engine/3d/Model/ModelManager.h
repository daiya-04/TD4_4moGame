#pragma once
#include <d3d12.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <wrl.h>
#include <string>
#include <vector>
#include <array>
#include <memory>
#include <optional>
#include <map>
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Quaternion.h"
#include "Matrix44.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "WorldTransform.h"

struct QuaternionTransform {
	Vector3 translate_;
	Quaternion rotate_;
	Vector3 scale_;
};

namespace DaiEngine {
	class Model {
	public:

		struct Node {
			QuaternionTransform transform_;
			Matrix4x4 localMatrix_;
			std::string name_;
			std::vector<Node> children_;
		};

	public:


		Node rootNode_;
		//メッシュ配列
		std::vector<Mesh> meshes_;
		//マテリアル配列
		std::vector<Material> materials_;

		//modelファイルの名前
		std::string name_;

	};

	class ModelManager {
	public:

		static const size_t kNumModel = 128;

	private:


		std::string filename_;
		std::string directoryPath_;
		std::vector<std::shared_ptr<Model>> models_;
		uint32_t useModelNum_ = 0;

	public:

		static ModelManager* GetInstance();
		//.objファイルの読み込み
		static std::shared_ptr<Model> LoadOBJ(const std::string& modelName);
		//.gltfファイルの読み込み
		static std::shared_ptr<Model> LoadGLTF(const std::string& modelName);

	private:

		std::shared_ptr<Model> LoadInternal(const std::string& modelName, const std::string& extension);

		void LoadObjFile(const std::string& modelName);

		void LoadGltfFile(const std::string& modelName);

		//void LoadMaterialTemplateFile(const std::string& fileName);
		//ノードの読み込み
		Model::Node ReadNode(aiNode* node);

	private:

		ModelManager() = default;
		~ModelManager() = default;
		ModelManager(const ModelManager&) = delete;
		ModelManager& operator=(const ModelManager&) = delete;

	};
}

