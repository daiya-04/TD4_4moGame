#include "ClearScene.h"

#include "AudioManager.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "SceneManager.h"
#include "ModelManager.h"
#include "Input.h"
#include "GlobalVariables.h"

ClearScene::~ClearScene() {
	bgm_->StopSound();
}

void ClearScene::SetGlobalVariables() {
	DaiEngine::GlobalVariables* globalVariables = DaiEngine::GlobalVariables::GetInstance();


	//AボタンUIの調整項目追加
	std::string groupName = "ClearText";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Translation", clearText_->GetPosition());
	//ワープホールの調整項目追加
	groupName = "UI_TitleBack";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Translation", titleBackUI_->GetPosition());
	//ゲーム開始演出の調整項目追加
	groupName = "UI_ReStart";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Translation", reStartUI_->GetPosition());


}

void ClearScene::ApplyGlobalVariables() {
	DaiEngine::GlobalVariables* globalVariables = DaiEngine::GlobalVariables::GetInstance();

	//AボタンUIのパラメータ設定
	std::string groupName = "ClearText";
	clearText_->SetPosition(globalVariables->GetVec2Value(groupName, "Translation"));

	//ワープホールのパラメータ設定
	groupName = "UI_TitleBack";
	titleBackUI_->SetPosition(globalVariables->GetVec2Value(groupName, "Translation"));

	//ゲーム開始演出のパラメータ設定
	groupName = "UI_ReStart";
	reStartUI_->SetPosition(globalVariables->GetVec2Value(groupName, "Translation"));


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

	///
	uint32_t clearTextTex = DaiEngine::TextureManager::Load("gameClear.png");
	uint32_t titleBackTex = DaiEngine::TextureManager::Load("TitleBack.png");
	uint32_t reStartTex = DaiEngine::TextureManager::Load("StartBiging.png");
	///

	bgm_ = DaiEngine::AudioManager::Load("BGM/GameClear.mp3");
	bgm_->Play();

	clearText_.reset(DaiEngine::Sprite::Create(clearTextTex, {}));

	titleBackUI_.reset(DaiEngine::Sprite::Create(titleBackTex, {}));
	titleBackUI_->SetSize({ 350.0f, 70.0f });
	titleBackUI_->SetTextureArea({ 350.0f * static_cast<float>(gTitleBackUISwitch_),0.0f }, { 350.0f,70.0f });

	reStartUI_.reset(DaiEngine::Sprite::Create(reStartTex, {}));
	reStartUI_->SetSize({ 350.0f, 70.0f });
	reStartUI_->SetTextureArea({ 350.0f * static_cast<float>(gReStartUISwitch_),0.0f }, { 350.0f,70.0f });

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

	
	switch (select_) {
	case Select::TitleBack:

		if (DaiEngine::Input::GetInstance()->TriggerKey(DIK_DOWN)) {
			select_ = Select::ReStrat;
			gTitleBackUISwitch_ = UISwitch::Off;
			gReStartUISwitch_ = UISwitch::On;
		}

		break;
	case Select::ReStrat:

		if (DaiEngine::Input::GetInstance()->TriggerKey(DIK_UP)) {
			select_ = Select::TitleBack;
			gTitleBackUISwitch_ = UISwitch::On;
			gReStartUISwitch_ = UISwitch::Off;
		}

		break;
	}

	titleBackUI_->SetTextureArea({ 350.0f * static_cast<float>(gTitleBackUISwitch_),0.0f }, { 350.0f,70 });
	reStartUI_->SetTextureArea({ 350.0f * static_cast<float>(gReStartUISwitch_),0.0f }, { 350.0f,70 });


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

	clearText_->Draw();
	titleBackUI_->Draw();
	reStartUI_->Draw();

}

void ClearScene::DrawPostEffect() {



}

void ClearScene::DrawRenderTexture() {



}

void ClearScene::DebugGUI() {
#ifdef _DEBUG



#endif // _DEBUG
}
