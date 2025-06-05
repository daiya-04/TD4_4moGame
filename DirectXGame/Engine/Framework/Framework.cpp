#include "Framework.h"

#include "ImGuiManager.h"
#include "Input.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "DirectionalLight.h"
#include "Line.h"
#include "GlobalVariables.h"
#include "AudioManager.h"
#include "DXCompiler.h"
#include "PipelineManager.h"
#include "RandomEngine.h"

namespace DaiEngine {

	void DSFramework::Init() {

		WinApp::GetInstance()->CreateGameWindow(L"すいーつらっしゅ！");


		DirectXCommon::GetInstance()->Initialize();
		ImGuiManager::GetInstance()->Initialize();

		DXCompiler::GetInstance()->Init();

		Input::GetInstance()->Initialize();
		AudioManager::GetInstance()->Init();

		TextureManager::GetInstance()->Initialize();
		TextureManager::GetInstance()->LoadAllFile();

		RandomEngine::GetInstance();

		PipelineManager::GetInstance()->Init();

		Line::Init();

		GlobalVariables::GetInstance()->LoadFiles();

		DirectionalLight::GetInstance()->Init();

	}

	void DSFramework::Update() {

		if (WinApp::GetInstance()->ProcessMessage()) { WinApp::GetInstance()->GameEnd(); }


		ImGuiManager::GetInstance()->Begin();

		Input::GetInstance()->Update();
		AudioManager::GetInstance()->Update();

		GlobalVariables::GetInstance()->Update();

		DirectionalLight::GetInstance()->Update();

		SceneManager::GetInstance()->Update();

	}

	void DSFramework::Finalize() {

		ImGuiManager::GetInstance()->Finalize();
		WinApp::GetInstance()->TerminateGameWindow();

	}

	void DSFramework::Run() {

		//初期化
		Init();

		//ゲームループ
		while (true) {

			//更新
			Update();

			//終了リクエストが来たら抜ける
			if (WinApp::GetInstance()->IsEndRequest()) {
				AudioManager::GetInstance()->AllAudioStop();
				if (AudioManager::GetInstance()->IsAllAudioStop()) {
					break;
				}
			}

			//描画
			Draw();

		}

		//ゲームの終了
		Finalize();
	}
}
