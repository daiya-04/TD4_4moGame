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


void GameScene::Init() {
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
	//追従カメラ処理生成
	followCamera_ = std::make_unique<FollowCamera>(&camera_, player_->GetWorld().translation_);
	
	//ボス生成
	boss_ = std::make_unique<Boss>(followCamera_.get());
	boss_->SetPlayerWorld(&player_->GetWorld());
	//地面生成
	field_ = std::make_unique<Field>();
	field_->Initialize();
	///

	//全ての初期化の後に処理
	globalVariableManager_->LoadAllSaveData();
	globalVariableManager_->SetLoadAllData();

	//セットされたデータで初期化
	player_->Init();
	boss_->Initialize();

	field_->CreateStage();
	field_->StartStage();
}

void GameScene::Update() {
	DebugGUI();

	auto* input = DaiEngine::Input::GetInstance();

#ifdef _DEBUG

	///デバッグ用シーン切り替えコマンド
	//「Ctrl + 1」でタイトルシーンへ
	if (input->PushKey(DIK_LCONTROL) && input->TriggerKey(DIK_1)) {
		DaiEngine::SceneManager::GetInstance()->ChangeScene("Title");
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

	//ライト更新
	pointLight_.Update();
	spotLight_.Update();

	//追従カメラ更新
	followCamera_->Update();

	//カメラ更新
	camera_.UpdateViewMatrix();
	camera_.UpdateCameraPos();

	//ステージ初期化済でプレイヤー更新
	if (!field_->GetStageAnimationFinishedFlag()) {
		//プレイヤー更新
		player_->Update();
		player_->UpdateOnField(field_->GetMassLocationPosY(player_->GetWorld().translation_) + player_->GetWorld().scale_.y);
		player_->SetField(field_.get());

		//ボス更新
		boss_->Update();
	}
	



	//地面更新
	field_->Update();

	for (std::unique_ptr<BossBullet>& bullet : boss_->GetBullets()) {
		Vector2 targetBlock = field_->GetNearestBlockAt(bullet->GetWorld().translation_.x, bullet->GetWorld().translation_.z);
		Block* block = field_->GetBlock(bullet->GetWorld().translation_.x, bullet->GetWorld().translation_.z);

		// Y範囲にあるか判定
		if (block->world.translation_.y >= bullet->GetWorld().translation_.y && block->world.translation_.y <= bullet->GetWorld().translation_.y + bullet->GetWorld().scale_.y) {
			field_->RaiseBlocksAroundWithAttenuation(field_->GetNearestBlockAt(bullet->GetWorld().translation_.x, bullet->GetWorld().translation_.z), bullet->GetWorld().scale_.x * 1.5f, field_->GetDeltaY());
			bullet->OnCollision();
		}
	}

	DaiEngine::InstancingObjData data;
	data.worldTransform_.Init();
	data.worldTransform_.translation_.x = 10;

	//当たり判定処理
	DaiEngine::ColliderManager::GetInstance()->CheckAllCollision();


	//死亡時ゲームおーばーへ
	if (player_->GetIsDead()) {
		DaiEngine::SceneManager::GetInstance()->ChangeScene("GameOver");
	}

	//ボスのHPが0以下になったらクリアへ
	if (boss_->GetIsDead()) {
		DaiEngine::SceneManager::GetInstance()->ChangeScene("Clear");
	}
}

void GameScene::DrawBackGround() {



}

void GameScene::DrawModel() {

	//地面描画
	field_->Draw();

	//ボス描画
	boss_->Draw();

	//プレイヤー描画
	player_->Draw();

}

void GameScene::DrawParticle() {

}

void GameScene::DrawUI() {

}

void GameScene::DrawPostEffect() {



}

void GameScene::DrawRenderTexture() {

}

void GameScene::DebugGUI() {
#ifdef _DEBUG

	//デバッグマネージャの更新
	globalVariableManager_->Update();


#endif // _DEBUG
}
