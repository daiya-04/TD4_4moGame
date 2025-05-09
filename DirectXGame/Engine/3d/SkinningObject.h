#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <string>
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Matrix44.h"
#include <vector>
#include "WorldTransform.h"
#include "Camera.h"
#include "ModelManager.h"
#include "Animation.h"
#include "SkinCluster.h"

namespace DaiEngine {
	class SkinningObject {
	private:
		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	private:

		struct SkinningInfoData {
			uint32_t numVertex_;
		};

		struct DeadEffectData {
			float  threshold_;
		};

	public: //静的メンバ関数

		//モデルの生成
		static SkinningObject* Create(std::shared_ptr<Model> model);
		//描画前処理
		static void preDraw();
		//描画後処理
		static void postDraw();

		static ComPtr<ID3D12Resource> CreateBufferResource(ComPtr<ID3D12Device> device, size_t sizeInBytes);

	private: //メンバ変数

		std::shared_ptr<Model> model_;
		SkinCluster* skinCluster_ = nullptr;

		ComPtr<ID3D12Resource> skinningInfoBuff_;
		SkinningInfoData* skinningInfoData_ = nullptr;

		ComPtr<ID3D12Resource> deadEffectBuffer_;
		DeadEffectData* deadEffectData_ = nullptr;

		bool visible_ = true;

	public:

		WorldTransform worldTransform_;
		float threshold_ = 0.0f;

	public: //メンバ関数

		//初期化
		void Initialize(std::shared_ptr<Model> model);
		//更新
		//void Update();
		//描画
		void Draw(const Camera& camera);

		void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

		void SetModel(std::shared_ptr<Model> model) { model_ = model; }

		void SetSkinCluster(SkinCluster* skinCluster) { skinCluster_ = skinCluster; }

		std::shared_ptr<Model> GetModel() const { return model_; }

		Vector3 GetWorldPos() const;

		void SetVisible(bool visible) { visible_ = visible; }

	};
}
