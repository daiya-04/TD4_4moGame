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

	bool isEnd_ = false;

	bool isDraw_ = true;

	float count_ = 0;

	float maxScale_ = 30.0f;

	float warningCount_ = 60.0f;

	float finalWarningCount_ = 30.0f;

	int currentBlink_ = 0;
	int blinkNum_ = 3;
};