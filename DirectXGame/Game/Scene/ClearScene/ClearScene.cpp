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

	choiceSE_ = DaiEngine::AudioManager::Load("SE/Choice.mp3");
	doneSE_ = DaiEngine::AudioManager::Load("SE/Done.mp3");

	clearText_.reset(DaiEngine::Sprite::Create(clearTextTex, {}));

	titleBackUI_.reset(DaiEngine::Sprite::Create(titleBackTex, {}));
	titleBackUI_->SetSize({ 350.0f, 70.0f });
	titleBackUI_->SetTextureArea({ 350.0f * static_cast<float>(gTitleBackUISwitch_),0.0f }, { 350.0f,70.0f });

	reStartUI_.reset(DaiEngine::Sprite::Create(reStartTex, {}));
	reStartUI_->SetSize({ 350.0f, 70.0f });
	reStartUI_->SetTextureArea({ 350.0f * static_cast<float>(gReStartUISwitch_),0.0f }, { 350.0f,70.0f });


	SetGlobalVariables();
	ApplyGlobalVariables();

}

void ClearScene::Update() {

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
	//「Ctrl + 4」でゲームオーバーシーンへ
	if (input->PushKey(DIK_LCONTROL) && input->TriggerKey(DIK_4)) {
		DaiEngine::SceneManager::GetInstance()->ChangeScene("GameOver");
	}
	//「Ctrl + 0」でテストシーンへ
	if (input->PushKey(DIK_LCONTROL) && input->TriggerKey(DIK_0)) {
		DaiEngine::SceneManager::GetInstance()->ChangeScene("Debug");
	}

	ApplyGlobalVariables();

#endif // _DEBUG

	
	switch (select_) {
	case Select::TitleBack:

		if (input->TriggerKey(DIK_DOWN) || input->TriggerButton(DaiEngine::Input::Button::DPAD_DOWN) || input->TriggerLStick(DaiEngine::Input::Stick::Down)) {
			select_ = Select::ReStrat;
			gTitleBackUISwitch_ = UISwitch::Off;
			gReStartUISwitch_ = UISwitch::On;
			choiceSE_->Play();
		}

		if (input->TriggerKey(DIK_SPACE) || input->TriggerButton(DaiEngine::Input::Button::A)) {
			doneSE_->Play();
			DaiEngine::SceneManager::GetInstance()->ChangeScene("Title");
		}

		break;
	case Select::ReStrat:

		if (input->TriggerKey(DIK_UP) || input->TriggerButton(DaiEngine::Input::Button::DPAD_UP) || input->TriggerLStick(DaiEngine::Input::Stick::Up)) {
			select_ = Select::TitleBack;
			gTitleBackUISwitch_ = UISwitch::On;
			gReStartUISwitch_ = UISwitch::Off;
			choiceSE_->Play();
		}

		if (input->TriggerKey(DIK_SPACE) || input->TriggerButton(DaiEngine::Input::Button::A)) {
			doneSE_->Play();
			DaiEngine::SceneManager::GetInstance()->ChangeScene("Game");
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
