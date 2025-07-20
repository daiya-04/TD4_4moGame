#include "Donut.h"

#pragma region 状態
#include"Boss/GingerbreadMan/Behavior/Idle/BossIdle.h"
#include"Boss/Donut/Behavior/AttackSpin/BossSpinAttack.h"
#include"Boss/Donut/Behavior/AttackPFollowBullet/Boss2AttackPFollowBullet.h"
#pragma endregion

Donut::Donut(const std::string& objectName, FollowCamera* camera, const DaiEngine::WorldTransform* playerWorld)
{
	IBoss::Init(objectName, camera, playerWorld);
	//状態を設定
	std::vector<std::string>names = {
		"None",
		"Idle",
		"Spin",
		"ShotBullet"
	};
	IBoss::SetDebugBehaviorName(names);

	//マネージャの生成
	IBoss::SetBulletType(BulletType::None);

	//反転フラグON
	SetReverse(true);

	//状態の生成
	behaviors_.resize((size_t)BossBehavior::Count);
	behaviors_[(size_t)BossBehavior::Idle] = std::make_unique<BossIdle>(&parameters_);
	behaviors_[(size_t)BossBehavior::Attack1] = std::make_unique<Boss2SpinAttack>(&parameters_);
	behaviors_[(size_t)BossBehavior::Attack2] = std::make_unique<Boss2AttackPFollowBullet>(&parameters_);

	//ツリーを追加
	for (auto& behavior : behaviors_) {
		//ツリーを設定
		tree_.SetTreeData(behavior->tree_);
	}

	//ツリー名設定
	tree_.name_ = "Donut";
}
