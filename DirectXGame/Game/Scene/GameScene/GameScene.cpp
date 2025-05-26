#include "GameScene.h"

#include "DirectXCommon.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "ParticleManager.h"
#include "ImGuiManager.h"
#include "Audio.h"
#include "Input.h"
#include "SceneManager.h"
#include "Hit.h"
#include <random>
#include <algorithm>
#include "ColliderManager.h"
#include "AudioManager.h"



GameScene::GameScene() {
	globalVariableManager_ = globalVariableManager_->GetInstance();
}

GameScene::~GameScene() {
	bgm_->StopSound();
}


void GameScene::Init(){
	//カメラ初期化
	camera_.Init();
	//ライト初期化
	pointLight_.Init();
	spotLight_.Init();
	//object3dクラスにライトセット
	DaiEngine::Object3d::SetPointLight(&pointLight_);
	DaiEngine::Object3d::SetSpotLight(&spotLight_);

	///

	bgm_ = DaiEngine::AudioManager::Load("BGM/Game.mp3");
	bgm_->Play();

	//ゲームオブジェクトにカメラ設定
	GameObject::SetCamera(&camera_);
	//陰士単シングオブジェクトにカメラ設定
	InstancingGameObject::SetCamera(&camera_);

	//プレイヤー生成
	player_ = std::make_unique<Player>();
	//ボス生成
	boss_ = std::make_unique<Boss>();
	boss_->SetPlayerWorld(&player_->GetWorld());
	//追従カメラ処理生成
	followCamera_ = std::make_unique<FollowCamera>(&camera_, player_->GetWorld().translation_);
	//地面生成
	field_ = std::make_unique<Field>();
	field_->Initialize();
	///

	//全ての初期化の後に処理
	globalVariableManager_->LoadAllSaveData();
	globalVariableManager_->SetLoadAllData();
}

void GameScene::Update() {
	DebugGUI();

#ifdef _DEBUG
	
	//デバッグ用シーンの切り替えコマンド
	if (DaiEngine::Input::GetInstance()->PushKey(DIK_LCONTROL) && DaiEngine::Input::GetInstance()->TriggerKey(DIK_1)) {
		DaiEngine::SceneManager::GetInstance()->ChangeScene("Title");
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

	

	//ライト更新
	pointLight_.Update();
	spotLight_.Update();

	//追従カメラ更新
	followCamera_->Update();

	//カメラ更新
	camera_.UpdateViewMatrix();
	camera_.UpdateCameraPos();
	
	//プレイヤー更新
	player_->Update();

	//ボス更新
	boss_->Update();

	//地面更新
	field_->Update();

	DaiEngine::InstancingObjData data;
	data.worldTransform_.Init();
	data.worldTransform_.translation_.x = 10;

}

void GameScene::DrawBackGround(){

	

}

void GameScene::DrawModel(){

	//地面描画
	field_->Draw();

	//ボス描画
	boss_->Draw();

	//プレイヤー描画
	player_->Draw();

}

void GameScene::DrawParticle(){

}

void GameScene::DrawUI(){
	
}

void GameScene::DrawPostEffect() {

	

}

void GameScene::DrawRenderTexture() {
	
}

void GameScene::DebugGUI(){
#ifdef _DEBUG
  
	//デバッグマネージャの更新
	globalVariableManager_->Update();


#endif // _DEBUG
}
