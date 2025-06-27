#include "BossIdle.h"
#include"Boss/Boss1/Boss.h"
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
	param_->isMoveToPlayer_ = true;

	//時間経過で次の状態に
	if (param_->currentSec >= sec_) {

		//ランダムな値取得
		int num = GetRandomInt(1, Boss::Behavior::Count - 1);

		if (savedATK_.size() != 2) {
			savedATK_.push_back(num);
		}
		else {
			//三連続同じ値なら違うものに変更
			if(savedATK_[0] == num && savedATK_[1] == num) {
				while (num == savedATK_[0] || num == savedATK_[1]) {
					num = GetRandomInt(1, Boss::Behavior::Count - 1);
				}
				savedATK_.clear();
				savedATK_.push_back(num);
			}
		}

		//値の状態に変更
		param_->behaviorRequest_ = Boss::Behavior(num);
	}
}
