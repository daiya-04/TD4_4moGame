#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <string>
#include <array>
#include <d3dx12.h>
#include <DirectXTex.h>

namespace DaiEngine {
	class TextureManager {
	private:
		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	public:
		static const size_t kNumTextures = 256;

		struct Texture {
			ComPtr<ID3D12Resource> resource;
			D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_{};
			D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_{};
			std::string name;
		};

	private:

		ID3D12Device* device_ = nullptr;
		UINT srvDescriptorHandleSize_ = 0;
		std::string directoryPath_ = "Resources/textures/";
		std::array<Texture, kNumTextures> textures_;
		int32_t useTextureNum_ = 0;
		std::array<ComPtr<ID3D12Resource>, kNumTextures> intermediateResource_;

	public:

		static TextureManager* GetInstance();
		//テクスチャ読み込み
		static uint32_t Load(const std::string& fileName);
		//モデル読み込み時にUV画像を読み込む
		static uint32_t LoadUv(const std::string& fileName, const std::string& filePath);

		void Initialize();

		void LoadAllFile();

		void SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* commandList, UINT rootParamIndex, uint32_t textureHandle);

		const D3D12_RESOURCE_DESC GetResourceDesc(uint32_t textureHandle);

		std::string GetTextureName(uint32_t textureHandle);

		D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUHandle(uint32_t textureHandle);

		std::array<Texture, kNumTextures> GetTextures() const { return textures_; }


	private:

		uint32_t LoadInternal(const std::string& fileName);

		uint32_t LoadUvInternal(const std::string& fileName, const std::string& filePath);

		void LoadFile(const std::string& fileName);

		//リソースの生成
		static ComPtr<ID3D12Resource> CreateBufferResource(ComPtr<ID3D12Device> device, size_t sizeInBytes);

		[[nodiscard]]
		ComPtr<ID3D12Resource> UploadTextureData(const DirectX::ScratchImage& mipImage, const Texture& tex);

	private:

		TextureManager() = default;
		~TextureManager() = default;
		TextureManager(const TextureManager&) = delete;
		TextureManager& operator=(const TextureManager&) = delete;

	};
}
