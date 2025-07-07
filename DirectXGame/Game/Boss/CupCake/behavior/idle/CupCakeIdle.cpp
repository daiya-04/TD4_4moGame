#include "CupCakeIdle.h"


CupCakeIdle::CupCakeIdle(BossParameters* param)
{
	param_ = param;
	tree_.name_ = "Idle";
	tree_.SetValue("second", &count_);
}

void CupCakeIdle::Init()
{
}

void CupCakeIdle::Update()
{
	//プレイヤー方向に移動
	param_->isMoveToPlayer_ = true;

	//時間経過で次の状態に
	if (param_->currentSec >= count_) {

		//ランダムな値取得
		int num = GetRandomInt(1, 3);
		if (num == 3)num = 2;

		if (savedATK_.size() != 3) {
			savedATK_.push_back(num);
		}
		else {
			//三連続同じ値なら違うものに変更
			if (savedATK_[0] == num && savedATK_[1] == num) {
				while (num == savedATK_[0] || num == savedATK_[1]) {
					num = GetRandomInt(1, 3);
					if (num == 3)num = 2;
				}
				savedATK_.clear();
				savedATK_.push_back(num);
			}
			else {
				//先頭の値を削除して追加
				savedATK_.erase(savedATK_.begin());
				savedATK_.push_back(num);
			}
		}

		//値の状態に変更
		param_->behaviorRequest_ = (num);
	}
}
