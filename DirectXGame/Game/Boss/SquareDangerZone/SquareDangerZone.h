#pragma once
#include"GameObject/GameObject.h"
#include"GlobalVariable/Tree/GlobalVariableTree.h"

class SquareDangerZone :public GameObject {

public:

	SquareDangerZone(const DaiEngine::WorldTransform* world);
	~SquareDangerZone()=default;

	void Init();

	void Update();

	void Draw();

	bool isEnd() { return isEnd_; };

	GvariTree& GetTree() { return tree_; }

private:

	enum class State {
		WARNING,
		FINALWARNING,
		Count
	}state_;

	//ツリー
	GvariTree tree_;

	//終了フラグ
	bool isEnd_ = false;
	//描画フラグ
	bool isDraw_ = true;
	//カウント
	float count_ = 0;
	//最大サイズ
	float maxScale_ = 30.0f;
	//警告時間
	float warningCount_ = 60.0f;
	//最終警告時間
	float finalWarningCount_ = 30.0f;
	//点滅回数
	int currentBlink_ = 0;
	//最大点滅回数
	int blinkNum_ = 3;
};