#pragma once

enum class BulletType {
	Fall,		//落下する
	None,		//動かず判定のみ
	Parabola,	//放物線を描く
	Wave,		//波を起こす判定
	Dive,		//飛び込む攻撃
	Count
};