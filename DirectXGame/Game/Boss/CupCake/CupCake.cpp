#include"CupCake.h"

#include"Boss/GingerbreadMan/Behavior/Idle/BossIdle.h"
#include"behavior/shotBullet/CupCakeShotBullet.h"
#include"behavior/waveDive/CupCakeWaveDive.h"
#include"behavior/fallAttack/CupCakeFallAttack.h"

CupCake::CupCake(const std::string& objectName, FollowCamera* camera, const DaiEngine::WorldTransform* playerWorld) {
	IBoss::Init(objectName, camera, playerWorld);

	//マネージャの生成
	IBoss::SetBulletType(BulletType::Parabola);

	//状態の生成
	behaviors_.resize((size_t)Behavior::Count);
	behaviors_[(size_t)Behavior::Idle] = std::make_unique<BossIdle>(&parameters_);
	behaviors_[(size_t)Behavior::ShotBullet] = std::make_unique<CupCakeShotBullet>(&parameters_);
	behaviors_[(size_t)Behavior::WaveDive] = std::make_unique<CupCakeWaveDive>(&parameters_);
	behaviors_[(size_t)Behavior::JumpAttack] = std::make_unique<CupCakeFallAttack>(&parameters_);

	//ツリーを追加
	for (auto& behavior : behaviors_) {
		//ツリーを設定
		tree_.SetTreeData(behavior->tree_);
	}

	//ツリー名設定
	tree_.name_ = "Capcake";

}
