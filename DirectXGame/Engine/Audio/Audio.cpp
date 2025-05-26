#include "Audio.h"
#include "Log.h"
#include <cassert>

#pragma comment(lib,"xaudio2.lib")
#pragma comment(lib,"Mf.lib")
#pragma comment(lib,"mfplat.lib")
#pragma comment(lib,"Mfreadwrite.lib")
#pragma comment(lib,"mfuuid.lib")

using namespace Microsoft::WRL;

namespace DaiEngine {

	ComPtr<IXAudio2> Audio::xAudio2_;
	IXAudio2MasteringVoice* Audio::masterVoice_ = nullptr;
	float Audio::bgmVolume_ = 0.5f;
	float Audio::seVolume_ = 0.5;

	void Audio::Init() {
		HRESULT hr;
		//XAudioエンジンのインスタンスを作成
		hr = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
		assert(SUCCEEDED(hr));
		//マスターボイスを作成
		hr = xAudio2_->CreateMasteringVoice(&masterVoice_);
		assert(SUCCEEDED(hr));
	}

	void Audio::DstoroyVoice() {
		if (masterVoice_) {
			masterVoice_->DestroyVoice();
			masterVoice_ = nullptr;
		}
	}

	void Audio::Update() {
		if (sourceVoices_) {

			float sourceVolume = volume_;

			if (audioType_ == AudioType::BGM) {
				sourceVolume *= bgmVolume_;
				SetVolume(sourceVolume);
			}
			else if (audioType_ == AudioType::SE) {
				sourceVolume *= seVolume_;
				SetVolume(sourceVolume);
			}
			XAUDIO2_VOICE_STATE state{};
			sourceVoices_->GetState(&state);
			if (state.BuffersQueued == 0) {
				DestroyPlayHandle();
				isStop_ = true;
			}

		}
	}

	void Audio::Play() {

		if (IsValidPlayhandle() && audioType_ == Audio::AudioType::BGM) {
			return;
		}

		if (IsValidPlayhandle() && audioType_ == AudioType::SE) {
			StopSound();
		}

		float sourceVolume = volume_;

		HRESULT hr;

		//再生する波形データの設定
		XAUDIO2_BUFFER buf{};
		buf.pAudioData = buffer_.data();
		buf.AudioBytes = bufferSize_;
		buf.Flags = XAUDIO2_END_OF_STREAM;
		if (audioType_ == AudioType::BGM) {
			buf.LoopCount = XAUDIO2_LOOP_INFINITE;
		}
		//sourceVoiceの作成
		IXAudio2SourceVoice* pSourcVoice = nullptr;
		hr = xAudio2_->CreateSourceVoice(&pSourcVoice, &wfex_);
		assert(SUCCEEDED(hr));

		hr = pSourcVoice->SubmitSourceBuffer(&buf);
		assert(SUCCEEDED(hr));

		hr = pSourcVoice->Start();
		assert(SUCCEEDED(hr));

		sourceVoices_ = pSourcVoice;
		isStop_ = false;

		if (audioType_ == AudioType::BGM) {
			sourceVolume *= bgmVolume_;
			SetVolume(sourceVolume);
		}
		else if (audioType_ == AudioType::SE) {
			sourceVolume *= seVolume_;
			SetVolume(sourceVolume);
		}

	}

	void Audio::SoundPlayLoopEnd() {
		if (IsValidPlayhandle()) {
			sourceVoices_->Stop();
			DestroyPlayHandle();
		}
	}

	void Audio::StopSound() {
		if (IsValidPlayhandle()) {
			sourceVoices_->Stop();
			sourceVoices_->FlushSourceBuffers();
			DestroyPlayHandle();
			isStop_ = true;
		}
	}

	void Audio::SetPitch(float pitch) {
		sourceVoices_->SetFrequencyRatio(pitch);
	}

	void Audio::SetVolume(float volume) {
		sourceVoices_->SetVolume(volume);
	}

	bool Audio::IsValidPlayhandle() {
		return sourceVoices_ != nullptr;
	}

	void Audio::DestroyPlayHandle() {
		if (sourceVoices_) {
			sourceVoices_->DestroyVoice();
			sourceVoices_ = nullptr;
		}
	}

	void Audio::SoundUnload() {
		//バッファのメモリの解放
		buffer_.clear();
		bufferSize_ = 0;
		wfex_ = {};
	}
}
