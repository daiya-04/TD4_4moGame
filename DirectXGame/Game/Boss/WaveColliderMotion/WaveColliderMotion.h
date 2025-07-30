#pragma once
#include"CylinderCollider.h"
#include"Camera.h"
#include"GlobalVariable/Tree/GlobalVariableTree.h"
#include<memory>

class WaveColliderMotion {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	WaveColliderMotion(const DaiEngine::WorldTransform& pWorld);
	~WaveColliderMotion();

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="center"></param>
	void Emit(const Vector3&center);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(DaiEngine::Camera*camera);

	/// <summary>
	/// 終了フラグ
	/// </summary>
	/// <returns></returns>
	bool isEnd() { return isEnd_; };

	/// <summary>
	/// ツリー取得
	/// </summary>
	/// <returns></returns>
	GvariTree& GetTree() { return tree_; }
private:

	void OnCollision(DaiEngine::Collider* collider);

private:

	//範囲
	float radius_ = 0;

	//時間
	float count_ = 0;

private:

	GvariTree tree_;

	//ワールド
	DaiEngine::WorldTransform world_;
	std::unique_ptr<DaiEngine::CylinderCollider>collider_;

	const DaiEngine::WorldTransform* pWorld_;

	//高さ
	float height_ = 2.0f;

	//最大時間
	float maxCount_ =180;

	//終了フラグ
	bool isEnd_ = true;

	//広がる速度
	float addRadius_ = 1.0f;

	//判定幅
	float hitWide_ = 2.0f;
};