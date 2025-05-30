#include "TitleScene.h"

#include "TextureManager.h"
#include "ModelManager.h"
#include "ImGuiManager.h"
#include "SceneManager.h"
#include "Input.h"
#include "GlobalVariables.h"
#include "WinApp.h"
#include "ParticleManager.h"
#include "Easing.h"
#include "AudioManager.h"

TitleScene::~TitleScene() {
	bgm_->StopSound();
}

void TitleScene::SetGlobalVariables() {
	DaiEngine::GlobalVariables* globalVariables = DaiEngine::GlobalVariables::GetInstance();

	
	//AボタンUIの調整項目追加
	std::string groupName = "TitleLogo";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Translation", titleLogo_->GetPosition());
	//ワープホールの調整項目追加
	groupName = "UI_GameFinish";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Translation", gameFinishUI_->GetPosition());
	//ゲーム開始演出の調整項目追加
	groupName = "UI_GameStart";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Translation", gameStartUI_->GetPosition());


}

void TitleScene::ApplyGlobalVariables() {
	DaiEngine::GlobalVariables* globalVariables = DaiEngine::GlobalVariables::GetInstance();
	
	//AボタンUIのパラメータ設定
	std::string groupName = "TitleLogo";
	titleLogo_->SetPosition(globalVariables->GetVec2Value(groupName, "Translation"));
	
	//ワープホールのパラメータ設定
	groupName = "UI_GameFinish";
	gameFinishUI_->SetPosition(globalVariables->GetVec2Value(groupName, "Translation"));

	//ゲーム開始演出のパラメータ設定
	groupName = "UI_GameStart";
	gameStartUI_->SetPosition(globalVariables->GetVec2Value(groupName, "Translation"));


}

void TitleScene::Init() {
	
	//カメラ初期化
	camera_.Init();
	camera_.SetFOV(45.0f);
	//ポイントライト初期化
	pointLight_.Init();
	//スポットライト初期化
	spotLight_.Init();
	//Object3dクラスにライトのセット
	DaiEngine::Object3d::SetPointLight(&pointLight_);
	DaiEngine::Object3d::SetSpotLight(&spotLight_);

	///テクスチャ読み込み
	uint32_t titleLogoTex = DaiEngine::TextureManager::Load("titleLogo.png");
	uint32_t gameStartTex = DaiEngine::TextureManager::Load("GameStart.png");
	uint32_t gameFinifhTex = DaiEngine::TextureManager::Load("GameFinish.png");
	///

	bgm_ = DaiEngine::AudioManager::Load("BGM/Title.mp3");
	bgm_->Play();

	titleLogo_.reset(DaiEngine::Sprite::Create(titleLogoTex, {}));

	gameStartUI_.reset(DaiEngine::Sprite::Create(gameStartTex, {}));
	gameStartUI_->SetSize({ 350.0f,70 });
	gameStartUI_->SetTextureArea({ 350.0f * static_cast<float>(gStartUISwitch_),0.0f }, { 350.0f,70 });

	gameFinishUI_.reset(DaiEngine::Sprite::Create(gameFinifhTex, {}));
	gameFinishUI_->SetSize({ 350.0f,70 });
	gameFinishUI_->SetTextureArea({ 350.0f * static_cast<float>(gFinishUISwitch_),0.0f }, { 350.0f,70 });
	


	//調整項目の追加と代入
	SetGlobalVariables();
	ApplyGlobalVariables();
	
	
}

void TitleScene::Update() {
	DebugGUI();

#ifdef _DEBUG
	//調整項目代入
	ApplyGlobalVariables();
	//デバッグ用シーン切り替えコマンド
	if (DaiEngine::Input::GetInstance()->PushKey(DIK_LCONTROL) && DaiEngine::Input::GetInstance()->TriggerKey(DIK_2)) {
		DaiEngine::SceneManager::GetInstance()->ChangeScene("Game");
	}
	if (DaiEngine::Input::GetInstance()->PushKey(DIK_LCONTROL) && DaiEngine::Input::GetInstance()->TriggerKey(DIK_3)) {
		DaiEngine::SceneManager::GetInstance()->ChangeScene("Clear");
	}
	if (DaiEngine::Input::GetInstance()->PushKey(DIK_LCONTROL) && DaiEngine::Input::GetInstance()->TriggerKey(DIK_4)) {
		DaiEngine::SceneManager::GetInstance()->ChangeScene("GameOver");
	}

	if (DaiEngine::Input::GetInstance()->PushKey(DIK_LCONTROL) && DaiEngine::Input::GetInstance()->TriggerKey(DIK_0)) {
		DaiEngine::SceneManager::GetInstance()->ChangeScene("Debug");
	}
	

#endif // _DEBUG
	

	switch (select_) {
		case Select::Start:

			if (DaiEngine::Input::GetInstance()->TriggerKey(DIK_DOWN)) {
				select_ = Select::Finish;
				gStartUISwitch_ = UISwitch::Off;
				gFinishUISwitch_ = UISwitch::On;
			}

			break;
		case Select::Finish:

			if (DaiEngine::Input::GetInstance()->TriggerKey(DIK_UP)) {
				select_ = Select::Start;
				gStartUISwitch_ = UISwitch::On;
				gFinishUISwitch_ = UISwitch::Off;
			}

			break;
	}

	gameStartUI_->SetTextureArea({ 350.0f * static_cast<float>(gStartUISwitch_),0.0f }, { 350.0f,70 });
	gameFinishUI_->SetTextureArea({ 350.0f * static_cast<float>(gFinishUISwitch_),0.0f }, { 350.0f,70 });
	
	//ライト更新
	pointLight_.Update();
	spotLight_.Update();

	//カメラ更新
	camera_.UpdateViewMatrix();
}

void TitleScene::DrawBackGround(){

	

}

void TitleScene::DrawModel(){

	

}

void TitleScene::DrawParticle(){

	

}

void TitleScene::DrawUI(){

	titleLogo_->Draw();
	gameStartUI_->Draw();
	gameFinishUI_->Draw();
	
}

void TitleScene::DrawPostEffect() {



}

void TitleScene::DrawRenderTexture() {



}

void TitleScene::DebugGUI(){
#ifdef _DEBUG




#endif // _DEBUG
}
