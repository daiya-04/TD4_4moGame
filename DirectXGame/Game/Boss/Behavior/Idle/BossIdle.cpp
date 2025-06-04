#include "BossIdle.h"
#include"Boss/Boss.h"
#include<memory>
#include <random>

int GetRandomInt(int min, int max) {
	static std::random_device rd;  
	static std::mt19937 gen(rd()); 
	std::uniform_int_distribution<> dist(min, max);
	return dist(gen);
}

BossIdle::BossIdle()
{
	tree_.name_ = "Idle";
	tree_.SetValue("second", &sec_);

}

void BossIdle::Init()
{
}

void BossIdle::Update()
{
	//プレイヤー方向に移動
	boss_->Move2Player();

	//時間経過で次の状態に
	if (boss_->parameters_.currentSec >= sec_) {

		//ランダムな値取得
		int num = GetRandomInt(1, Boss::Behavior::Count - 1);
		//値の状態に変更
		boss_->behaviorRequest_ = Boss::Behavior(num);
	}
}
