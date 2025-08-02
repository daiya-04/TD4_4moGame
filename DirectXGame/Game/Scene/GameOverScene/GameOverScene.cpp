#include "GameOverScene.h"

#include "AudioManager.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "SceneManager.h"
#include "ModelManager.h"
#include "Input.h"
#include "GlobalVariables.h"

GameOverScene::~GameOverScene() {
	bgm_->StopSound();
}

void GameOverScene::SetGlobalVariables() {
	DaiEngine::GlobalVariables* globalVariables = DaiEngine::GlobalVariables::GetInstance();


	//ゲームオーバーテキスト
	std::string groupName = "GameOverText";
	/*globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Translation", gameOverText_->GetPosition());*/
	//
	groupName = "GO_UI_TitleBack";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Translation", titleBackUI_->GetPosition());
	//
	groupName = "GO_UI_ReStart";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Translation", reStartUI_->GetPosition());
	//
	groupName = "GO_UI_Continue";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Translation", continueUI_->GetPosition());


}

void GameOverScene::ApplyGlobalVariables() {
	DaiEngine::GlobalVariables* globalVariables = DaiEngine::GlobalVariables::GetInstance();

	//
	std::string groupName = "GameOverText";
	//gameOverText_->SetPosition(globalVariables->GetVec2Value(groupName, "Translation"));

	//
	groupName = "GO_UI_TitleBack";
	titleBackUI_->SetPosition(globalVariables->GetVec2Value(groupName, "Translation"));

	//
	groupName = "GO_UI_ReStart";
	reStartUI_->SetPosition(globalVariables->GetVec2Value(groupName, "Translation"));

	//
	groupName = "GO_UI_Continue";
	continueUI_->SetPosition(globalVariables->GetVec2Value(groupName, "Translation"));

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

	///
	//uint32_t gameOverTextTex = DaiEngine::TextureManager::Load("gameOver.png");
	uint32_t titleBackTex = DaiEngine::TextureManager::Load("TitleBack.png");
	uint32_t reStartTex = DaiEngine::TextureManager::Load("StartBiging.png");
	uint32_t continueTex = DaiEngine::TextureManager::Load("Continue.png");
	///

	bgm_ = DaiEngine::AudioManager::Load("BGM/GameOver.mp3");
	bgm_->Play();

	choiceSE_ = DaiEngine::AudioManager::Load("SE/Choice.mp3");
	doneSE_ = DaiEngine::AudioManager::Load("SE/Done.mp3");

	//gameOverText_.reset(DaiEngine::Sprite::Create(gameOverTextTex, {}));
	gameOverText_ = std::make_unique<GameOverLogo>();
	gameOverText_->Init(L"ゲームオーバー");

	titleBackUI_.reset(DaiEngine::Sprite::Create(titleBackTex, {}));
	titleBackUI_->SetSize({ 350.0f, 70.0f });
	titleBackUI_->SetTextureArea({ 350.0f * static_cast<float>(gTitleBackUISwitch_),0.0f }, { 350.0f,80.0f });

	reStartUI_.reset(DaiEngine::Sprite::Create(reStartTex, {}));
	reStartUI_->SetSize({ 350.0f, 70.0f });
	reStartUI_->SetTextureArea({ 350.0f * static_cast<float>(gReStartUISwitch_),0.0f }, { 350.0f,80.0f });

	continueUI_.reset(DaiEngine::Sprite::Create(continueTex, {}));
	continueUI_->SetSize({ 350.0f, 70.0f });
	continueUI_->SetTextureArea({ 350.0f * static_cast<float>(gContinueUISwitch_),0.0f }, { 350.0f,80.0f });

	SetGlobalVariables();
	ApplyGlobalVariables();

	bossManager_ = BossManager::GetInstance();

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

	ApplyGlobalVariables();

#endif // _DEBUG


	MenuInput();


	gReStartUISwitch_ = (select_ == Select::ReStart) ? UISwitch::On : UISwitch::Off;
	gTitleBackUISwitch_ = (select_ == Select::TitleBack) ? UISwitch::On : UISwitch::Off;
	gContinueUISwitch_ = (select_ == Select::Continue) ? UISwitch::On : UISwitch::Off;

	titleBackUI_->SetTextureArea({ 350.0f * static_cast<float>(gTitleBackUISwitch_),0.0f }, { 350.0f,80.0f });
	reStartUI_->SetTextureArea({ 350.0f * static_cast<float>(gReStartUISwitch_),0.0f }, { 350.0f,80.0f });
	continueUI_->SetTextureArea({ 350.0f * static_cast<float>(gContinueUISwitch_),0.0f }, { 350.0f,80.0f });

	gameOverText_->Update();

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

	gameOverText_->Draw();
	titleBackUI_->Draw();
	reStartUI_->Draw();
	continueUI_->Draw()
		;
}

void GameOverScene::DrawPostEffect() {



}

void GameOverScene::DrawRenderTexture() {



}

void GameOverScene::DebugGUI() {
#ifdef _DEBUG



#endif // _DEBUG
}

void GameOverScene::MenuInput() {

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

void GameOverScene::ToTitleBack() {
	doneSE_->Play();
	DaiEngine::SceneManager::GetInstance()->ChangeScene("Title");
	bossManager_->SetBossType(BossType::GingerbreadMan);
}

void GameOverScene::ToContinue() {
	doneSE_->Play();
	DaiEngine::SceneManager::GetInstance()->ChangeScene("Game");
}

void GameOverScene::ToReStart() {
	doneSE_->Play();
	DaiEngine::SceneManager::GetInstance()->ChangeScene("Game");
	bossManager_->SetBossType(BossType::GingerbreadMan);
}