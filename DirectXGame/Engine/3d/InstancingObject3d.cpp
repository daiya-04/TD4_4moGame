#include "InstancingObject3d.h"
#include <cassert>
#include "TextureManager.h"
#include "DirectXCommon.h"
#include "PipelineManager.h"
#include "RootParameters.h"
#include "DirectionalLight.h"

using namespace Microsoft::WRL;
using namespace InstancingObject3dParam;

namespace DaiEngine {

	InstancingObject3d* InstancingObject3d::Create(std::shared_ptr<Model> model, uint32_t drawNum) {
		InstancingObject3d* obj = new InstancingObject3d();
		obj->Init(model, drawNum);

		return obj;
	}

	void InstancingObject3d::Init(std::shared_ptr<Model> model, uint32_t drawNum) {

		model_ = model;
		drawMaxNum_ = drawNum;

		instancingResource_ = CreateBufferResource(sizeof(ObjectGPU) * drawMaxNum_);

		auto* device = DirectXCommon::GetInstance()->GetDevice();

		D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
		instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
		instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		instancingSrvDesc.Buffer.FirstElement = 0;
		instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
		instancingSrvDesc.Buffer.NumElements = drawMaxNum_;
		instancingSrvDesc.Buffer.StructureByteStride = sizeof(ObjectGPU);
		UINT handleSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		srvHandle_.first = DirectXCommon::GetInstance()->GetCPUDescriptorHandle(DirectXCommon::GetInstance()->GetSrvHeap(), handleSize, DirectXCommon::GetInstance()->GetSrvHeapCount());
		srvHandle_.second = DirectXCommon::GetInstance()->GetGPUDescriptorHandle(DirectXCommon::GetInstance()->GetSrvHeap(), handleSize, DirectXCommon::GetInstance()->GetSrvHeapCount());
		DirectXCommon::GetInstance()->IncrementSrvHeapCount();

		device->CreateShaderResourceView(instancingResource_.Get(), &instancingSrvDesc, srvHandle_.first);
		
		instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingObjDatas_));


	}

	void InstancingObject3d::Draw(const Camera& camera, std::list<InstancingObjData>& objDatas, BlendMode blendMode) {

		drawNum_ = 0;
		for (auto& objData : objDatas) {

			if (drawNum_ < drawMaxNum_) {
				instancingObjDatas_[drawNum_].matWorld = objData.worldTransform_.matWorld_;
				instancingObjDatas_[drawNum_].matWorldInverse = objData.worldTransform_.WorldInverseTransposeMat_;
				instancingObjDatas_[drawNum_].color = objData.color_;
				instancingObjDatas_[drawNum_].color.w = objData.alpha_;
				drawNum_++;
			}
		}

		auto* commandList = DirectXCommon::GetInstance()->GetCommandList();

		PipelineManager::GetInstance()->preDraw("InstancingObject3d", blendMode);

		for (auto& mesh : model_->meshes_) {

			commandList->IASetVertexBuffers(0, 1, mesh.GetVBV());
			commandList->IASetIndexBuffer(mesh.GetIVB());

			const auto& material = mesh.GetMaterial();

			commandList->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootParameter::kMaterial), material.GetGPUVirtualAddress());
			commandList->SetGraphicsRootDescriptorTable(static_cast<UINT>(RootParameter::kObjectGPU), srvHandle_.second);
			commandList->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootParameter::kCamera), camera.GetGPUVirtualAddress());
			TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, static_cast<UINT>(RootParameter::kTexture), material.GetUVHandle());
			commandList->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootParameter::kDirectionLight), DirectionalLight::GetInstance()->GetGPUVirtualAddress());

			commandList->DrawIndexedInstanced(static_cast<UINT>(mesh.indices_.size()), static_cast<UINT>(drawNum_), 0, 0, 0);
		}

	}

	ComPtr<ID3D12Resource> InstancingObject3d::CreateBufferResource(size_t sizeInBytes) {
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
		auto* device = DirectXCommon::GetInstance()->GetDevice();
		HRESULT hr = device->CreateCommittedResource(&uploadHeapproperties, D3D12_HEAP_FLAG_NONE, &ResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&Resource));
		assert(SUCCEEDED(hr));

		return Resource;
	}
}
