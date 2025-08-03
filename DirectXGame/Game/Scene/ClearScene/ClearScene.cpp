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


	////AボタンUIの調整項目追加
	std::string groupName = "ClearText";
	//globalVariables->CreateGroup(groupName);
	//globalVariables->AddItem(groupName, "Translation", clearText_->GetPosition());
	//ワープホールの調整項目追加
	groupName = "UI_TitleBack";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Translation", titleBackUI_->GetPosition());
	//ゲーム開始演出の調整項目追加
	groupName = "UI_ReStart";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Translation", reStartUI_->GetPosition());

	groupName = "UI_Icon1";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Translation", bossUIs_["Icon1"]->GetPosition());

	groupName = "UI_Icon2";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Translation", bossUIs_["Icon2"]->GetPosition());

	groupName = "UI_Icon3";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Translation", bossUIs_["Icon3"]->GetPosition());

	groupName = "UI_Gauge1";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Translation", bossUIs_["Gauge1"]->GetPosition());

	groupName = "UI_Gauge2";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Translation", bossUIs_["Gauge2"]->GetPosition());


	groupName = "ClearScene_BUIScale";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Scale", bossUIScale_);

}

void ClearScene::ApplyGlobalVariables() {
	DaiEngine::GlobalVariables* globalVariables = DaiEngine::GlobalVariables::GetInstance();

	////AボタンUIのパラメータ設定
	std::string groupName = "ClearText";
	//clearText_->SetPosition(globalVariables->GetVec2Value(groupName, "Translation"));

	//ワープホールのパラメータ設定
	groupName = "UI_TitleBack";
	titleBackUI_->SetPosition(globalVariables->GetVec2Value(groupName, "Translation"));

	//ゲーム開始演出のパラメータ設定
	groupName = "UI_ReStart";
	reStartUI_->SetPosition(globalVariables->GetVec2Value(groupName, "Translation"));

	groupName = "UI_Icon1";
	bossUIs_["Icon1"]->SetPosition(globalVariables->GetVec2Value(groupName, "Translation"));

	groupName = "UI_Icon2";
	bossUIs_["Icon2"]->SetPosition(globalVariables->GetVec2Value(groupName, "Translation"));

	groupName = "UI_Icon3";
	bossUIs_["Icon3"]->SetPosition(globalVariables->GetVec2Value(groupName, "Translation"));

	groupName = "UI_Gauge1";
	bossUIs_["Gauge1"]->SetPosition(globalVariables->GetVec2Value(groupName, "Translation"));

	groupName = "UI_Gauge2";
	bossUIs_["Gauge2"]->SetPosition(globalVariables->GetVec2Value(groupName, "Translation"));

	groupName = "ClearScene_BUIScale";
	bossUIScale_ = globalVariables->GetFloatValue(groupName, "Scale");

	for (auto& [tag, ui] : bossUIs_) {
		ui->SetScale(bossUIScale_);
	}

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
	//uint32_t clearTextTex = DaiEngine::TextureManager::Load("gameClear.png");
	uint32_t titleBackTex = DaiEngine::TextureManager::Load("TitleBack.png");
	uint32_t reStartTex = DaiEngine::TextureManager::Load("StartBiging.png");
	///

	bgm_ = DaiEngine::AudioManager::Load("BGM/GameClear.mp3");
	bgm_->Play();

	choiceSE_ = DaiEngine::AudioManager::Load("SE/Choice.mp3");
	doneSE_ = DaiEngine::AudioManager::Load("SE/Done.mp3");

	//clearText_.reset(DaiEngine::Sprite::Create(clearTextTex, {}));
	clearText_ = std::make_unique<ClearLogo>();
	clearText_->Init(L"ゲームクリア");

	titleBackUI_.reset(DaiEngine::Sprite::Create(titleBackTex, {}));
	titleBackUI_->SetSize({ 350.0f, 70.0f });
	titleBackUI_->SetTextureArea({ 350.0f * static_cast<float>(gTitleBackUISwitch_),0.0f }, { 350.0f,80.0f });

	reStartUI_.reset(DaiEngine::Sprite::Create(reStartTex, {}));
	reStartUI_->SetSize({ 350.0f, 70.0f });
	reStartUI_->SetTextureArea({ 350.0f * static_cast<float>(gReStartUISwitch_),0.0f }, { 350.0f,80.0f });

	bossUIs_["Icon1"].reset(DaiEngine::Sprite::Create(DaiEngine::TextureManager::Load("bossIcon1.png"), {}));
	bossUIs_["Icon2"].reset(DaiEngine::Sprite::Create(DaiEngine::TextureManager::Load("bossIcon2.png"), {}));
	bossUIs_["Icon3"].reset(DaiEngine::Sprite::Create(DaiEngine::TextureManager::Load("bossIcon3.png"), {}));
	bossUIs_["Gauge1"].reset(DaiEngine::Sprite::Create(DaiEngine::TextureManager::Load("enemyHPGaugeFram.png"), {}));
	bossUIs_["Gauge2"].reset(DaiEngine::Sprite::Create(DaiEngine::TextureManager::Load("enemyHPGaugeFram.png"), {}));

	stateRequest_ = EffectState::BossDefeat;

	SetGlobalVariables();
	ApplyGlobalVariables();

	crossMarks_.resize(3);
	for (auto& mark : crossMarks_) {
		mark = std::make_unique<CrossMark>();
	}
	crossMarks_[0]->Init(bossUIs_["Icon1"]->GetPosition());
	crossMarks_[1]->Init(bossUIs_["Icon2"]->GetPosition());
	crossMarks_[2]->Init(bossUIs_["Icon3"]->GetPosition());

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

	if (stateRequest_) {
		state_ = stateRequest_.value();

		stateInitTable_[state_]();

		stateRequest_ = std::nullopt;
	}
	
	stateUpdateTable_[state_]();


	clearText_->Update();

	for (auto& mark : crossMarks_) {
		mark->Update();
	}

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
	if (state_ == EffectState::MenuReady) {
		titleBackUI_->Draw();
		reStartUI_->Draw();
	}

	for (auto& [tag, ui] : bossUIs_) {
		ui->Draw();
	}

	for (auto& mark : crossMarks_) {
		mark->Draw();
	}

}

void ClearScene::DrawPostEffect() {



}

void ClearScene::DrawRenderTexture() {



}

void ClearScene::DebugGUI() {
#ifdef _DEBUG



#endif // _DEBUG
}

void ClearScene::BossDefeatInit() {

	timer_ = 0.0f;
	stampedIndex_ = 0;

}

void ClearScene::BossDefeatUpdate() {

	timer_ += kDeltaTime_;

	if (stampedIndex_ < crossMarks_.size()) {
		if (timer_ >= stampInterval_) {
			crossMarks_[stampedIndex_]->StartStamp(stampStartDatas_[stampedIndex_].speed, stampStartDatas_[stampedIndex_].startScale);

			stampedIndex_++;
			timer_ = 0.0f;
		}
	}
	else {
		if (timer_ >= 2.0f) {
			stateRequest_ = EffectState::LogoApper;
		}
	}
	

}

void ClearScene::LogoApperInit() {

	clearText_->StartApper();

}

void ClearScene::LogoApperUpdate() {

	if (clearText_->IsAppered()) {
		stateRequest_ = EffectState::MenuReady;
	}

}

void ClearScene::MenuReadyInit() {

	clearText_->StartBounce();

}

void ClearScene::MenuReadyUpdate() {

	MenuInput();

	gTitleBackUISwitch_ = (select_ == Select::TitleBack) ? UISwitch::On : UISwitch::Off;
	gReStartUISwitch_ = (select_ == Select::ReStart) ? UISwitch::On : UISwitch::Off;

	titleBackUI_->SetTextureArea({ 350.0f * static_cast<float>(gTitleBackUISwitch_),0.0f }, { 350.0f,80.0f });
	reStartUI_->SetTextureArea({ 350.0f * static_cast<float>(gReStartUISwitch_),0.0f }, { 350.0f,80.0f });

}

void ClearScene::MenuInput() {

	int currentIndex = static_cast<int>(std::distance(order_.begin(), std::find(order_.begin(), order_.end(), select_)));

	auto* input = DaiEngine::Input::GetInstance();

	if (input->TriggerKey(DIK_UP) || input->TriggerButton(DaiEngine::Input::Button::DPAD_UP) || input->TriggerLStick(DaiEngine::Input::Stick::Up)) {
		if (currentIndex > 0) {
			select_ = order_[currentIndex - 1];
			choiceSE_->Play();
		}
	}

	if (input->TriggerKey(DIK_DOWN) || input->TriggerButton(DaiEngine::Input::Button::DPAD_DOWN) || input->TriggerLStick(DaiEngine::Input::Stick::Down)) {
		if (currentIndex < order_.size() - 1) {
			select_ = order_[currentIndex + 1];
			choiceSE_->Play();
		}
	}

	if (input->TriggerKey(DIK_SPACE) || input->TriggerButton(DaiEngine::Input::Button::A)) {
		onSelect_[select_]();
	}

}

void ClearScene::ToTitle() {
	doneSE_->Play();
	DaiEngine::SceneManager::GetInstance()->ChangeScene("Title");
}

void ClearScene::ToGame() {
	doneSE_->Play();
	DaiEngine::SceneManager::GetInstance()->ChangeScene("Game");
}
