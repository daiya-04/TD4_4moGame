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
	camera_.SetCamera(Vector3{ 0.0f,1.65f,-14.0f }, Vector3{ -0.1f,0.0f,0.0f });
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
	uint32_t titleBackGroundTex = DaiEngine::TextureManager::Load("titleBG.png");
	
	//
	bgm_ = DaiEngine::AudioManager::Load("BGM/Title.mp3");
	bgm_->Play();

	//ゲームオブジェクトにカメラ設定
	GameObject::SetCamera(&camera_);
	//陰士単シングオブジェクトにカメラ設定
	InstancingGameObject::SetCamera(&camera_);

	choiceSE_ = DaiEngine::AudioManager::Load("SE/Choice.mp3");
	doneSE_ = DaiEngine::AudioManager::Load("SE/Done.mp3");

	titleLogo_.reset(DaiEngine::Sprite::Create(titleLogoTex, {}));

	gameStartUI_.reset(DaiEngine::Sprite::Create(gameStartTex, {}));
	gameStartUI_->SetSize({ 350.0f,70.0f });
	gameStartUI_->SetTextureArea({ 350.0f * static_cast<float>(gStartUISwitch_),0.0f }, { 350.0f,80.0f });

	gameFinishUI_.reset(DaiEngine::Sprite::Create(gameFinifhTex, {}));
	gameFinishUI_->SetSize({ 350.0f,70.0f });
	gameFinishUI_->SetTextureArea({ 350.0f * static_cast<float>(gFinishUISwitch_),0.0f }, { 350.0f,80.0f });
	
	titleBG_.reset(DaiEngine::Sprite::Create(titleBackGroundTex, {}));

	titleSceneDirection_ = std::make_unique<TitleSceneDirection>();
	titleSceneDirection_->Initialize();

	//調整項目の追加と代入
	SetGlobalVariables();
	ApplyGlobalVariables();
	
	
}

void TitleScene::Update() {
	DebugGUI();

	auto* input = DaiEngine::Input::GetInstance();

#ifdef _DEBUG
	//調整項目代入
	ApplyGlobalVariables();
	///デバッグ用シーン切り替えコマンド
	//「Ctrl + 2」でゲームシーンへ
	if (input->PushKey(DIK_LCONTROL) && input->TriggerKey(DIK_2)) {
		DaiEngine::SceneManager::GetInstance()->ChangeScene("Game");
	}
	//「Ctrl + 3」でクリアシーンへ
	if (input->PushKey(DIK_LCONTROL) && input->TriggerKey(DIK_3)) {
		DaiEngine::SceneManager::GetInstance()->ChangeScene("Clear");
	}
	//「Ctrl + 4」でゲームオーバーシーンへ
	if (input->PushKey(DIK_LCONTROL) && input->TriggerKey(DIK_4)) {
		DaiEngine::SceneManager::GetInstance()->ChangeScene("GameOver");
	}
	//「Ctrl + 0」でテストシーンへ
	if (input->PushKey(DIK_LCONTROL) && input->TriggerKey(DIK_0)) {
		DaiEngine::SceneManager::GetInstance()->ChangeScene("Debug");
	}
	

#endif // _DEBUG

#ifndef USE_IMGUI
	if (isDeleteCursor_ == false) {
		isDeleteCursor_ = true;
		input->ToggleCursor();
	}
#endif

	

	MenuInput();

	gStartUISwitch_ = (select_ == Select::Start) ? UISwitch::On : UISwitch::Off;
	gFinishUISwitch_ = (select_ == Select::Finish) ? UISwitch::On : UISwitch::Off;

	//TitleLogo揺らす処理
	time_ += 0.016f;
	float angle = sinf(time_ * 2.0f) * 0.15f; //2Hzの揺れ

	titleLogo_->SetRotate(angle);
	gameStartUI_->SetTextureArea({ 350.0f * static_cast<float>(gStartUISwitch_),0.0f }, { 350.0f,70 });
	gameFinishUI_->SetTextureArea({ 350.0f * static_cast<float>(gFinishUISwitch_),0.0f }, { 350.0f,70 });

	titleSceneDirection_->Update();
	
	//ライト更新
	pointLight_.Update();
	spotLight_.Update();

	//カメラ更新
	camera_.DrawImGui();
	camera_.UpdateViewMatrix();
}

void TitleScene::DrawBackGround(){

	titleBG_->Draw();

}

void TitleScene::DrawModel(){
	titleSceneDirection_->Draw();
	

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

void TitleScene::MenuInput() {

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

void TitleScene::ToGame() {
	doneSE_->Play();
	DaiEngine::SceneManager::GetInstance()->ChangeScene("Game");
}

void TitleScene::ToEnd() {
	doneSE_->Play();
	DaiEngine::WinApp::GetInstance()->GameEnd();
}
