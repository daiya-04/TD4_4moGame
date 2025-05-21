#include "SkinningObject.h"
#include <cassert>
#include "TextureManager.h"
#include "DirectionalLight.h"
#include "RootParameters.h"
#include "PipelineManager.h"
#include "DirectXCommon.h"
#include "AnimationManager.h"

using namespace Microsoft::WRL;
using namespace SkinningObjParam;

namespace DaiEngine {
	SkinningObject* SkinningObject::Create(std::shared_ptr<Model> model) {

		SkinningObject* obj = new SkinningObject();
		obj->Initialize(model);

		return obj;
	}

	void SkinningObject::preDraw() {

		PipelineManager::GetInstance()->preDraw("Skinning");

	}

	void SkinningObject::postDraw() {



	}

	void SkinningObject::Initialize(std::shared_ptr<Model> model) {
		model_ = model;
		worldTransform_.Init();

		skeleton_ = Skeleton::Create(model_->rootNode_);

		for (auto& mesh : model_->meshes_) {
			if (mesh.isSkin_) {
				auto& skinCluster = skinClusters_.emplace_back(SkinCluster());
				skinCluster.Create(skeleton_, mesh);
			}
		}

		skinningInfoBuff_ = CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), sizeof(uint32_t));
		skinningInfoBuff_->Map(0, nullptr, reinterpret_cast<void**>(&skinningInfoData_));

		deadEffectBuffer_ = CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), sizeof(DeadEffectData));
		deadEffectBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&deadEffectData_));

		threshold_ = 0.0f;
		visible_ = true;

	}

	void SkinningObject::Update() {

		worldTransform_.UpdateMatrix();
		animation_.Play(skeleton_);

		skeleton_.Update();
		for (auto& skinCluster : skinClusters_) {
			skinCluster.Update(skeleton_);
		}

	}

	void SkinningObject::Update(const Matrix4x4& rotateMat) {

		worldTransform_.UpdateMatrixRotate(rotateMat);
		animation_.Play(skeleton_);

		skeleton_.Update();
		for (auto& skinCluster : skinClusters_) {
			skinCluster.Update(skeleton_);
		}

	}

	void SkinningObject::Draw(const Camera& camera) {

		if (!visible_) { return; }

		worldTransform_.Map();

		ID3D12GraphicsCommandList* commandList = DirectXCommon::GetInstance()->GetCommandList();

		for (auto& mesh : model_->meshes_) {

			if (mesh.isSkin_) {
				skinningInfoData_->numVertex_ = static_cast<uint32_t>(mesh.vertices_.size());

				D3D12_RESOURCE_BARRIER preBarrier = {};
				preBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				preBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
				preBarrier.Transition.pResource = mesh.GetUavResource();
				preBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
				preBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
				preBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
				commandList->ResourceBarrier(1, &preBarrier);


				PipelineManager::GetInstance()->preDispatch("Skinning");

				commandList->SetComputeRootDescriptorTable(static_cast<UINT>(ComputeRootParam::kPalette), skinClusters_[mesh.GetSkinNumber()].paletteSrvHandle_.second);

				commandList->SetComputeRootDescriptorTable(static_cast<UINT>(ComputeRootParam::kInputVertex), mesh.GetVertexSrvhandleGPU());

				commandList->SetComputeRootDescriptorTable(static_cast<UINT>(ComputeRootParam::kInfluence), skinClusters_[mesh.GetSkinNumber()].influenceSrvHandle_.second);

				commandList->SetComputeRootDescriptorTable(static_cast<UINT>(ComputeRootParam::kOutputVertex), mesh.GetVertexUavHandleGPU());

				commandList->SetComputeRootConstantBufferView(static_cast<UINT>(ComputeRootParam::kSkinningInfo), skinningInfoBuff_->GetGPUVirtualAddress());

				commandList->Dispatch(static_cast<UINT>(mesh.vertices_.size() + 1023) / 1024, 1, 1);

				D3D12_RESOURCE_BARRIER postBarrier = {};
				postBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				postBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
				postBarrier.Transition.pResource = mesh.GetUavResource();
				postBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
				postBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
				postBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
				commandList->ResourceBarrier(1, &postBarrier);
			}

			deadEffectData_->threshold_ = threshold_;

			PipelineManager::GetInstance()->preDraw("Skinning");

			if (mesh.isSkin_) {
				commandList->IASetVertexBuffers(0, 1, mesh.GetSkinnedVBV());
			}
			else {
				commandList->IASetVertexBuffers(0, 1, mesh.GetVBV());
			}
			
			commandList->IASetIndexBuffer(mesh.GetIVB());

			const auto& material = mesh.GetMaterial();
			commandList->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootParameter::kMaterial), material.GetGPUVirtualAddress());
			//wvp用のCBufferの場所の設定
			commandList->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootParameter::kWorldTransform), worldTransform_.GetGPUVirtualAddress());

			commandList->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootParameter::kCamera), camera.GetGPUVirtualAddress());

			TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, static_cast<UINT>(RootParameter::kTexture), material.GetUVHandle());

			TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, static_cast<UINT>(RootParameter::kEnvironmentTex), TextureManager::Load("skyBox.dds"));

			commandList->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootParameter::kDirectionLight), DirectionalLight::GetInstance()->GetGPUVirtualAddress());

			commandList->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootParameter::kDeadEffect), deadEffectBuffer_->GetGPUVirtualAddress());

			commandList->DrawIndexedInstanced(static_cast<UINT>(mesh.indices_.size()), 1, 0, 0, 0);
		}

	}

	void SkinningObject::SetAnimation(const std::string& fileName,bool isLoop) {

		animation_ = AnimationManager::Load(fileName);
		animation_.Start(isLoop);

	}

	Vector3 SkinningObject::GetWorldPos() const {
		Vector3 worldPos;

		worldPos.x = worldTransform_.matWorld_.m[3][0];
		worldPos.y = worldTransform_.matWorld_.m[3][1];
		worldPos.z = worldTransform_.matWorld_.m[3][2];

		return worldPos;
	}

	ComPtr<ID3D12Resource> SkinningObject::CreateBufferResource(ComPtr<ID3D12Device> device, size_t sizeInBytes) {
		//リソース用のヒープの設定
		D3D12_HEAP_PROPERTIES uploadHeapproperties{};
		uploadHeapproperties.Type = D3D12_HEAP_TYPE_UPLOAD;//UploadHeapを使う
		//リソースの設定
		D3D12_RESOURCE_DESC ResourceDesc{};
		//バッファリソース。テクスチャの場合はまた別の設定をする
		ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		ResourceDesc.Width = sizeInBytes; //リソースのサイズ。
		//バッファの場合はこれにする決まり
		ResourceDesc.Height = 1;
		ResourceDesc.DepthOrArraySize = 1;
		ResourceDesc.MipLevels = 1;
		ResourceDesc.SampleDesc.Count = 1;
		//バッファの場合はこれらにする決まり
		ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		//実際に頂点リソースを作る
		ComPtr<ID3D12Resource> Resource = nullptr;
		HRESULT hr = device->CreateCommittedResource(&uploadHeapproperties, D3D12_HEAP_FLAG_NONE, &ResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&Resource));
		assert(SUCCEEDED(hr));

		return Resource;
	}


}