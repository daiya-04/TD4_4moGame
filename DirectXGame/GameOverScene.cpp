#include "GameOverScene.h"

#include "AudioManager.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "SceneManager.h"
#include "ModelManager.h"
#include "Input.h"

GameOverScene::~GameOverScene() {
	bgm_->StopSound();
}

void GameOverScene::Init() {

	///
	//カメラ初期化
	camera_.Init();
	camera_.SetFOV(45.0f);
	//ポイントライト初期化
	pointLight_.Init();
	//スポット初期化
	spotLight_.Init();
	//Object3dクラスにライトのセット
	DaiEngine::Object3d::SetPointLight(&pointLight_);
	DaiEngine::Object3d::SetSpotLight(&spotLight_);
	///


	bgm_ = DaiEngine::AudioManager::Load("BGM/GameOver.mp3");
	bgm_->Play();



}

void GameOverScene::Update() {

	auto* input = DaiEngine::Input::GetInstance();

#ifdef _DEBUG

	///デバッグ用シーン切り替えコマンド
	//「Ctrl + 1」でタイトルシーンへ
	if (input->PushKey(DIK_LCONTROL) && input->TriggerKey(DIK_1)) {
		DaiEngine::SceneManager::GetInstance()->ChangeScene("Title");
	}
	//「Ctrl + 2」でゲームシーンへ
	if (input->PushKey(DIK_LCONTROL) && input->TriggerKey(DIK_2)) {
		DaiEngine::SceneManager::GetInstance()->ChangeScene("Game");
	}
	//「Ctrl + 3」でクリアシーンへ
	if (input->PushKey(DIK_LCONTROL) && input->TriggerKey(DIK_3)) {
		DaiEngine::SceneManager::GetInstance()->ChangeScene("Clear");
	}
	//「Ctrl + 0」でテストシーンへ
	if (input->PushKey(DIK_LCONTROL) && input->TriggerKey(DIK_0)) {
		DaiEngine::SceneManager::GetInstance()->ChangeScene("Debug");
	}


#endif // _DEBUG







	///
	//ライト更新
	pointLight_.Update();
	spotLight_.Update();

	//カメラ更新
	camera_.UpdateViewMatrix();
	///

}

void GameOverScene::DrawBackGround() {



}

void GameOverScene::DrawModel() {



}

void GameOverScene::DrawParticle() {



}

void GameOverScene::DrawUI() {



}

void GameOverScene::DrawPostEffect() {



}

void GameOverScene::DrawRenderTexture() {



}

void GameOverScene::DebugGUI() {
#ifdef _DEBUG



#endif // _DEBUG
}