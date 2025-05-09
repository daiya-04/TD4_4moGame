#include "PlayerAttackManager.h"
#include"Player/Player.h"

#pragma region 状態
#include"Player/behavior/Attacks/Combo1/PlayerAttackCombo1.h"
#include"Player/behavior/Attacks/Combo2/PlayerAttackCombo2.h"
#include"Player/behavior/Attacks/Combo3/PlayerAttackCombo3.h"
#pragma endregion


PlayerAttackManager::PlayerAttackManager()
{
	//攻撃の数指定
	attacks_.resize((size_t)Behavior::Count);
	//攻撃の生成
	attacks_[(size_t)Behavior::Attack1] = std::make_unique<PlayerAttackCombo1>();
	attacks_[(size_t)Behavior::Attack2] = std::make_unique<PlayerAttackCombo2>();
	attacks_[(size_t)Behavior::Attack3] = std::make_unique<PlayerAttackCombo3>();

	tree_.name_ = "AttackManager";
	for (auto& behavior : attacks_) {
		tree_.SetTreeData(behavior->tree_);
	}
}

void PlayerAttackManager::Init()
{
	behaviorRequest_ = Attack1;
}

void PlayerAttackManager::Update()
{
	//リクエスト処理
	if (behaviorRequest_){
		behavior_ = behaviorRequest_.value();
		behaviorRequest_ = std::nullopt;
		attacks_[behavior_]->Init();
	}

	//攻撃の更新
	attacks_[behavior_]->Update();

	if (attacks_[behavior_]->GetIsEnd()) {
		if (attacks_[behavior_]->GetIsNextAttack()) {
			//攻撃のリクエストを取得
			behaviorRequest_ = (Behavior)((int)behavior_ + 1);
			//リクエストが範囲外
			if (behaviorRequest_ == Behavior::Count) {
				//移動処理に戻る
				player_->behaviorRequest_ = Player::Behavior::Move;
			}
		}
		else {
			//移動状態に戻る
			player_->behaviorRequest_ = Player::Behavior::Move;
		}

	}
}
