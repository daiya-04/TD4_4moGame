#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "Vec3.h"
#include "Vec4.h"
#include "Matrix44.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "ModelManager.h"
#include "PipelineManager.h"
#include <list>

namespace DaiEngine {

	struct InstancingObjData {
		WorldTransform worldTransform_;
		Vector4 color_;
		float alpha_;
	};

	class InstancingObject3d {
	private:
		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	public:
		//モデルの生成
		static InstancingObject3d* Create(std::shared_ptr<Model> model, uint32_t drawNum);

	private:

		static ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

	private:
		//GPUに送るデータ
		struct ObjectGPU {
			Matrix4x4 matWorld;
			Matrix4x4 matWorldInverse;
			Vector4 color;
		};

	private:
		//モデルデータ
		std::shared_ptr<Model> model_;

		ComPtr<ID3D12Resource> instancingResource_;
		std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> srvHandle_;
		ObjectGPU* instancingObjDatas_ = nullptr;

		uint32_t drawMaxNum_ = 0;
		uint32_t drawNum_ = 0;

	public:

		void Init(std::shared_ptr<Model> model, uint32_t drawNum);

		void Draw(const Camera& camera, std::list<InstancingObjData>& objDatas, BlendMode blendMode = BlendMode::kAlpha);



	};
}
