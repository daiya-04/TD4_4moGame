#include "ModelManager.h"

#include "DirectXCommon.h"
#include <fstream>
#include <sstream>
#include <cassert>
#include "TextureManager.h"
#include "ModelManager.h"
#include "Line.h"
#include "ShapesDraw.h"

using namespace Microsoft::WRL;

namespace DaiEngine {
	ModelManager* ModelManager::GetInstance() {
		static ModelManager instance;

		return &instance;
	}

	std::shared_ptr<Model> ModelManager::LoadOBJ(const std::string& modelName) {
		return ModelManager::GetInstance()->LoadInternal(modelName, "obj");
	}

	std::shared_ptr<Model> ModelManager::LoadGLTF(const std::string& modelName) {
		return ModelManager::GetInstance()->LoadInternal(modelName, "gltf");
	}

	std::shared_ptr<Model> ModelManager::LoadInternal(const std::string& modelName, const std::string& extension) {

		assert(useModelNum_ < kNumModel);
		uint32_t handle = useModelNum_;


		auto it = std::find_if(models_.begin(), models_.end(), [&](const auto& model) {return model->name_ == modelName; });

		if (it != models_.end()) {
			handle = static_cast<uint32_t>(std::distance(models_.begin(), it));
			return models_[handle];
		}

		if (extension == "obj") {
			LoadObjFile(modelName);
		}
		else if (extension == "gltf") {
			LoadGltfFile(modelName);
		}

		models_[handle]->name_ = modelName;
		for (auto& mesh : models_[handle]->meshes_) {
			mesh.Init();
			if (extension == "gltf") {
				mesh.SkinnedInit();
			}
		}

		useModelNum_++;
		return models_[handle];
	}

	void ModelManager::LoadObjFile(const std::string& modelName) {

		std::vector<Vector4> positions;  //位置
		std::vector<Vector3> normals;  //法線
		std::vector<Vector2> texcoords;  //テクスチャ座標
		std::string line;  //ファイルから呼んだ1行を格納するもの

		Assimp::Importer importer;
		filename_ = modelName + ".obj";
		directoryPath_ = "Resources/model/" + modelName + "/";
		std::string filePath = directoryPath_ + filename_;
		const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
		assert(scene->HasMeshes());

		auto& model = models_.emplace_back(std::make_shared<Model>());

		model->meshes_.resize(scene->mNumMeshes);
		for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
			aiMesh* mesh = scene->mMeshes[meshIndex];
			assert(mesh->HasNormals());
			assert(mesh->HasTextureCoords(0));

			model->meshes_[meshIndex].vertices_.resize(mesh->mNumVertices);
			for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];

				model->meshes_[meshIndex].vertices_[vertexIndex].pos_ = { -position.x,position.y,position.z,1.0f };
				model->meshes_[meshIndex].vertices_[vertexIndex].normal = { -normal.x,normal.y,normal.z };
				model->meshes_[meshIndex].vertices_[vertexIndex].uv_ = { texcoord.x,texcoord.y };
			}
			for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
				aiFace& face = mesh->mFaces[faceIndex];
				assert(face.mNumIndices == 3);
				for (uint32_t element = 0; element < face.mNumIndices; ++element) {
					uint32_t vertexIndex = face.mIndices[element];
					model->meshes_[meshIndex].indices_.push_back(vertexIndex);
				}
			}

			aiMaterial* srcMaterial = scene->mMaterials[mesh->mMaterialIndex];
			if (srcMaterial->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
				aiString textureFilePath;
				srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
				std::string materialFilename;
				materialFilename = directoryPath_ + textureFilePath.C_Str();
				Material material;
				material.Init();
				material.SetUVHandle(TextureManager::GetInstance()->LoadUv(textureFilePath.C_Str(), materialFilename));
				model->materials_.push_back(material);
				model->meshes_[meshIndex].SetMaterial(model->materials_.back());
				
			}
		}

	}

	void ModelManager::LoadGltfFile(const std::string& modelName) {

		std::vector<Vector4> positions;  //位置
		std::vector<Vector3> normals;  //法線
		std::vector<Vector2> texcoords;  //テクスチャ座標
		std::string line;  //ファイルから呼んだ1行を格納するもの

		Assimp::Importer importer;
		filename_ = modelName + ".gltf";
		directoryPath_ = "Resources/model/" + modelName + "/";
		std::string filePath = directoryPath_ + filename_;
		const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
		assert(scene->HasMeshes());

		auto& model = models_.emplace_back(std::make_shared<Model>());

		model->meshes_.resize(scene->mNumMeshes);
		int32_t skinCount = 0;
		for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
			aiMesh* mesh = scene->mMeshes[meshIndex];
			assert(mesh->HasNormals());
			assert(mesh->HasTextureCoords(0));

			model->meshes_[meshIndex].name_ = mesh->mName.C_Str();
			model->meshes_[meshIndex].vertices_.resize(mesh->mNumVertices);
			for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];

				model->meshes_[meshIndex].vertices_[vertexIndex].pos_ = { -position.x,position.y,position.z,1.0f };
				model->meshes_[meshIndex].vertices_[vertexIndex].normal = { -normal.x,normal.y,normal.z };
				model->meshes_[meshIndex].vertices_[vertexIndex].uv_ = { texcoord.x,texcoord.y };
			}
			for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
				aiFace& face = mesh->mFaces[faceIndex];
				assert(face.mNumIndices == 3);
				for (uint32_t element = 0; element < face.mNumIndices; ++element) {
					uint32_t vertexIndex = face.mIndices[element];
					model->meshes_[meshIndex].indices_.push_back(vertexIndex);
				}
			}

			

			if (mesh->mNumBones != 0) {
				model->meshes_[meshIndex].isSkin_ = true;
				model->meshes_[meshIndex].SetSkinNumber(skinCount);
				skinCount++;
			}

			for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
				aiBone* bone = mesh->mBones[boneIndex];
				std::string jointName = bone->mName.C_Str();
				Mesh::JointWeightData& jointWeightData = model->meshes_[meshIndex].skinClusterData_[jointName];

				aiMatrix4x4 bindPoseMatrixAssimp = bone->mOffsetMatrix.Inverse();
				aiVector3D translate, scale;
				aiQuaternion rotate;
				bindPoseMatrixAssimp.Decompose(scale, rotate, translate);
				Quaternion q = { rotate.x, -rotate.y, -rotate.z, rotate.w };
				Matrix4x4 TMat = MakeTranslateMatrix({ -translate.x, translate.y, translate.z });
				Matrix4x4 SMat = MakeScaleMatrix({ scale.x,scale.y,scale.z });
				Matrix4x4 bindPoseMat = SMat * q.MakeRotateMatrix() * TMat;
				jointWeightData.inverseBindPoseMatrix_ = bindPoseMat.Inverse();
				for (uint32_t weightIndex = 0; weightIndex < bone->mNumWeights; ++weightIndex) {
					jointWeightData.vertexWeights_.push_back({ bone->mWeights[weightIndex].mWeight,bone->mWeights[weightIndex].mVertexId });
				}
			}
			

			aiMaterial* srcMaterial = scene->mMaterials[mesh->mMaterialIndex];
			if (srcMaterial->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
				aiString textureFilePath;
				srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
				std::string materialFilename;
				materialFilename = directoryPath_ + textureFilePath.C_Str();
				Material material;
				material.Init();
				material.SetUVHandle(TextureManager::GetInstance()->LoadUv(textureFilePath.C_Str(), materialFilename));
				model->materials_.push_back(material);
				model->meshes_[meshIndex].SetMaterial(model->materials_.back());
				
			}
		}

		model->rootNode_ = ReadNode(scene->mRootNode);

	}

	Model::Node ModelManager::ReadNode(aiNode* node) {

		Model::Node result;
		aiVector3D translate, scale;
		aiQuaternion rotate;
		node->mTransformation.Decompose(scale, rotate, translate); //assimpの行列からSRTを抽出する
		result.transform_.translate_ = { -translate.x,translate.y ,translate.z }; //x軸を反転、さらに回転方向が逆なので軸を反転させる
		result.transform_.rotate_ = { rotate.x, -rotate.y, -rotate.z, rotate.w }; //x軸を反転
		result.transform_.scale_ = { scale.x,scale.y,scale.z }; //scaleはそのまま
		result.localMatrix_ = MakeScaleMatrix(result.transform_.scale_) * result.transform_.rotate_.MakeRotateMatrix() * MakeTranslateMatrix(result.transform_.translate_);

		result.name_ = node->mName.C_Str(); //Node名を格納
		result.children_.resize(node->mNumChildren); //子供の数だけ確保
		for (size_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
			result.children_[childIndex] = ReadNode(node->mChildren[childIndex]);
		}

		return result;
	}

	

}

