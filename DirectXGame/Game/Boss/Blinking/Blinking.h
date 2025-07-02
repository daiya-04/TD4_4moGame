#pragma once
#include"../../GlobalVariable/Tree/GlobalVariableTree.h"

class Blinking {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Blinking();
	~Blinking() = default;

	/// <summary>
	/// 更新
	/// </summary>
	bool Update();

	bool& GetIsDrawFlag() { return isDraw_; }

	/// <summary>
	/// 点滅の開始処理
	/// </summary>
	void StartBlinking();

	GvariTree& GetTree() { return tree_; }

private://**パラメータ**//

	//有効フラグ
	bool isActive_ = false;

	//無敵時間カウント
	float currentHitCount_ = 0;

	//点滅回数カウント
	int blinkingCount_ = 0;

	//点滅時間
	float hitCount_ = 1.0f;

	//点滅回数
	int maxBlinkingNum_ = 3;

	//描画フラグ
	bool isDraw_=true;

	//つりー
	GvariTree tree_;

};