#include "Blinking.h"

Blinking::Blinking()
{
	tree_.name_ = "Blinking";
	tree_.SetMonitorValue("isActive", &isActive_);
	tree_.SetMonitorValue("currentHitCount", &currentHitCount_);
	tree_.SetMonitorValue("blinkingCount", &blinkingCount_);
	tree_.SetValue("hitCount", &hitCount_);
	tree_.SetValue("maxBlinkingNum", &maxBlinkingNum_);
}

bool Blinking::Update()
{
	//有効時の処理
	if (isActive_) {

		//カウント増加
		currentHitCount_++;

		////時間内での点滅処理
		if (currentHitCount_ >= (hitCount_ / maxBlinkingNum_) * blinkingCount_) {
			blinkingCount_++;

			//透明度を変更
			if (isDraw_) {
				isDraw_ = false;
			}
			else {
				isDraw_ = true;
			}
		}

		//時間経過で終了
		if (currentHitCount_ >= hitCount_) {
			isActive_ = false;
			isDraw_ = true;
		}
	}

	return isActive_;
 }

void Blinking::StartBlinking()
{
	//有効フラグを立てる
	isActive_ = true;
	//カウント初期化
	currentHitCount_ = 0;
	//点滅回数初期化
	blinkingCount_ = 0;
}
