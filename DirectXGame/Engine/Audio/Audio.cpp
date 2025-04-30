#include "Audio.h"
#include "Log.h"
#include <cassert>

#pragma comment(lib,"xaudio2.lib")
#pragma comment(lib,"Mf.lib")
#pragma comment(lib,"mfplat.lib")
#pragma comment(lib,"Mfreadwrite.lib")
#pragma comment(lib,"mfuuid.lib")

namespace DaiEngine {
	Audio* Audio::GetInstance() {
		static Audio instance;

		return &instance;
	}

	Audio::~Audio() {
		for (size_t index = 0; index < kMaxNumPlayHandles; ++index) {
			DestroyPlayHandle(index);
		}

		if (masterVoice_) {
			masterVoice_->DestroyVoice();
			masterVoice_ = nullptr;
		}

		MFShutdown();
	}

	size_t Audio::Load(const std::string& filename) {
		return Audio::GetInstance()->LoadInternal(filename);
	}

	void Audio::Initialize() {
		HRESULT hr;
		//XAudioエンジンのインスタンスを作成
		hr = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
		assert(SUCCEEDED(hr));
		//マスターボイスを作成
		hr = xAudio2_->CreateMasteringVoice(&masterVoice_);
		assert(SUCCEEDED(hr));

		for (size_t index = 0; index < kMaxNumPlayHandles; ++index) {
			DestroyPlayHandle(index);
		}

		MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);
	}

	void Audio::Update() {
		for (size_t index = 0; index < kMaxNumPlayHandles; ++index) {
			if (sourceVoices_[index]) {
				XAUDIO2_VOICE_STATE state{};
				sourceVoices_[index]->GetState(&state);
				if (state.BuffersQueued == 0) {
					DestroyPlayHandle(index);
				}
			}
		}
	}

	size_t Audio::Play(size_t soundHandle, float volume, bool loop) {
		HRESULT hr;
		const SoundData& soundData = soundData_.at(soundHandle);

		//再生する波形データの設定
		XAUDIO2_BUFFER buf{};
		buf.pAudioData = soundData.pBuffer.data();
		buf.AudioBytes = soundData.bufferSize;
		buf.Flags = XAUDIO2_END_OF_STREAM;
		if (loop) {
			buf.LoopCount = XAUDIO2_LOOP_INFINITE;
		}

		size_t playHandle = FindUnUsedPlayHandle();
		//プレイハンドルがいっぱい
		assert(playHandle < kMaxNumPlayHandles);

		//sourceVoiceの作成
		IXAudio2SourceVoice* pSourcVoice = nullptr;
		hr = xAudio2_->CreateSourceVoice(&pSourcVoice, &soundData.wfex);
		assert(SUCCEEDED(hr));

		hr = pSourcVoice->SubmitSourceBuffer(&buf);
		assert(SUCCEEDED(hr));

		hr = pSourcVoice->Start();
		assert(SUCCEEDED(hr));

		sourceVoices_[playHandle] = pSourcVoice;
		sourceVoices_[playHandle]->SetVolume(volume);
		return playHandle;
	}

	void Audio::SoundPlayLoopEnd(size_t playHandle) {
		if (IsValidPlayhandle(playHandle)) {
			sourceVoices_[playHandle]->Stop();
			DestroyPlayHandle(playHandle);
		}
	}

	size_t Audio::LoadInternal(const std::string& filename) {
		//読み込み済みのファイルの検索
		auto iter = std::find_if(soundData_.begin(), soundData_.end(), [&](const SoundData& soundData) {
			return soundData.filename == filename;
			});
		if (iter != soundData_.end()) {
			return std::distance(soundData_.begin(), iter);
		}

		//SourceReaderの作成
		ComPtr<IMFSourceReader> MFSourceReader;
		std::string directoryPath = "Resources/sounds/";
		MFCreateSourceReaderFromURL(ConvertString(directoryPath + filename).c_str(), NULL, &MFSourceReader);

		//メディアタイプの取得
		ComPtr<IMFMediaType> MFMediaType;
		MFCreateMediaType(&MFMediaType);
		MFMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
		MFMediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
		MFSourceReader->SetCurrentMediaType(static_cast<DWORD>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), nullptr, MFMediaType.Get());

		//MFMediaType->Release();
		MFMediaType = nullptr;
		MFSourceReader->GetCurrentMediaType(static_cast<DWORD>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), &MFMediaType);


		//オーディオデータ形式の読み込み
		WAVEFORMATEX* waveFormat{ nullptr };
		MFCreateWaveFormatExFromMFMediaType(MFMediaType.Get(), &waveFormat, nullptr);

		//データの読み込み
		std::vector<BYTE> mediaData;
		while (true) {

			ComPtr<IMFSample> MFSample;
			DWORD dwStreamFlags{ 0 };
			MFSourceReader->ReadSample(static_cast<DWORD>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, nullptr, &dwStreamFlags, nullptr, &MFSample);

			if (dwStreamFlags & MF_SOURCE_READERF_ENDOFSTREAM) { break; }

			ComPtr<IMFMediaBuffer> MFMediaBuffer;
			MFSample->ConvertToContiguousBuffer(&MFMediaBuffer);

			BYTE* buffer{ nullptr };
			DWORD cbCurrentLength{ 0 };
			MFMediaBuffer->Lock(&buffer, nullptr, &cbCurrentLength);

			mediaData.resize(mediaData.size() + cbCurrentLength);
			memcpy(mediaData.data() + mediaData.size() - cbCurrentLength, buffer, cbCurrentLength);

			MFMediaBuffer->Unlock();
		}

		//returnする為の音声データ
		SoundData soundData{};
		soundData.filename = filename;
		soundData.wfex = *waveFormat;
		soundData.bufferSize = sizeof(BYTE) * static_cast<uint32_t>(mediaData.size());
		soundData.pBuffer = std::move(mediaData);
		soundData_.emplace_back(soundData);

		return soundData_.size() - 1;
	}

	void Audio::StopSound(size_t playHandle) {
		assert(playHandle < kMaxNumPlayHandles);
		DestroyPlayHandle(playHandle);
	}

	void Audio::SetPitch(size_t playHandle, float pitch) {
		assert(playHandle < kMaxNumPlayHandles);
		sourceVoices_[playHandle]->SetFrequencyRatio(pitch);
	}

	void Audio::SetValume(size_t playHandle, float volume) {
		assert(playHandle < kMaxNumPlayHandles);
		sourceVoices_[playHandle]->SetVolume(volume);
	}

	bool Audio::IsValidPlayhandle(size_t playHandle) {
		return playHandle < kMaxNumPlayHandles && sourceVoices_[playHandle] != nullptr;
	}

	size_t Audio::FindUnUsedPlayHandle() {
		for (size_t index = 0; index < kMaxNumPlayHandles; ++index) {
			if (sourceVoices_[index] == nullptr) {
				return index;
			}
		}
		return size_t(-1);
	}

	void Audio::DestroyPlayHandle(size_t playHandle) {
		assert(playHandle < kMaxNumPlayHandles);
		if (sourceVoices_[playHandle]) {
			sourceVoices_[playHandle]->DestroyVoice();
			sourceVoices_[playHandle] = nullptr;
		}
	}

	void Audio::SoundUnload(size_t soundHandle) {
		//バッファのメモリの解放
		soundData_.at(soundHandle).pBuffer.clear();
		soundData_.at(soundHandle).bufferSize = 0;
		soundData_.at(soundHandle).wfex = {};
		soundData_.erase(soundData_.begin() + soundHandle);
	}
}
