#pragma once
#include"SkinningObject.h"
#include"Camera.h"

//モデル描画に必要なデータを持つクラス
class GameObject {

protected:

	static DaiEngine::Camera* camera_;

public:

	DaiEngine::WorldTransform* world_=nullptr;

public:

	/// <summary>
	/// カメラのポインタ取得
	/// </summary>
	/// <param name="camera"></param>
	static void GetCamera(DaiEngine::Camera* camera) { camera_ = camera; }


	GameObject()=default;
	~GameObject() = default;

	void Init(const std::string& modelName);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// ワールド座標取得
	/// </summary>
	/// <returns></returns>
	DaiEngine::WorldTransform GetWorld() {return model_->worldTransform_; }

private:

	std::unique_ptr<DaiEngine::SkinningObject> model_;

	DaiEngine::Skeleton skeleton_;

	std::unique_ptr<DaiEngine::SkinCluster> skinCluster_ = nullptr;
};