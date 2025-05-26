#include "ClearScene.h"

#include "AudioManager.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "SceneManager.h"
#include "ModelManager.h"
#include "Input.h"

ClearScene::~ClearScene() {
	bgm_->StopSound();
}

void ClearScene::Init() {

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


	bgm_ = DaiEngine::AudioManager::Load("BGM/GameClear.mp3");
	bgm_->Play();



}

void ClearScene::Update() {

#ifdef _DEBUG
	
	//デバッグ用シーン切り替えコマンド
	if (DaiEngine::Input::GetInstance()->PushKey(DIK_LCONTROL) && DaiEngine::Input::GetInstance()->TriggerKey(DIK_1)) {
		DaiEngine::SceneManager::GetInstance()->ChangeScene("Title");
	}
	if (DaiEngine::Input::GetInstance()->PushKey(DIK_LCONTROL) && DaiEngine::Input::GetInstance()->TriggerKey(DIK_2)) {
		DaiEngine::SceneManager::GetInstance()->ChangeScene("Game");
	}
	if (DaiEngine::Input::GetInstance()->PushKey(DIK_LCONTROL) && DaiEngine::Input::GetInstance()->TriggerKey(DIK_4)) {
		DaiEngine::SceneManager::GetInstance()->ChangeScene("GameOver");
	}
	
	if (DaiEngine::Input::GetInstance()->PushKey(DIK_LCONTROL) && DaiEngine::Input::GetInstance()->TriggerKey(DIK_0)) {
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

void ClearScene::DrawBackGround() {



}

void ClearScene::DrawModel() {



}

void ClearScene::DrawParticle() {



}

void ClearScene::DrawUI() {



}

void ClearScene::DrawPostEffect() {



}

void ClearScene::DrawRenderTexture() {



}

void ClearScene::DebugGUI() {
#ifdef _DEBUG



#endif // _DEBUG
}
