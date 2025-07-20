#include "GingerbreadMan.h"

#pragma region 状態
#include"Boss/GingerbreadMan/Behavior/Idle/BossIdle.h"
#include"Boss/GingerbreadMan/Behavior/AreaAttack/BossAreaAttack.h"
#include"Boss/GingerbreadMan/Behavior/WeaponRollAttack/BossWeaponRollAttack.h"
#pragma endregion

GingerbreadMan::GingerbreadMan(const std::string& objectName, FollowCamera* camera, const DaiEngine::WorldTransform* playerWorld)
{
	IBoss::Init(objectName, camera, playerWorld);

	//状態を設定
	std::vector<std::string>names = {
		"None",
		"Idle",
		"AreaAttack",
		"WeaponRollAttack"
	};
	IBoss::SetDebugBehaviorName(names);

	//マネージャの生成
	IBoss::SetBulletType(BulletType::Fall);

	//状態の生成
	behaviors_.resize((size_t)BossBehavior::Count);
	behaviors_[(size_t)BossBehavior::Idle] = std::make_unique<BossIdle>(&parameters_);
	behaviors_[(size_t)BossBehavior::Attack1] = std::make_unique<BossAreaAttack>(&parameters_);
	behaviors_[(size_t)BossBehavior::Attack2] = std::make_unique<BossWeaponRollAttack>(&parameters_);

	if (auto* attack2 = dynamic_cast<BossWeaponRollAttack*>(behaviors_[(size_t)BossBehavior::Attack2].get())) {
		attack2->SetGingerbreadMan(this); // GingerbreadMan の this を渡す
	}


	//ツリーを追加
	for (auto& behavior : behaviors_) {

		//ツリーを設定
		tree_.SetTreeData(behavior->tree_);
	}

	//ツリー名設定
	tree_.name_ = "GingerbreadMan";
};