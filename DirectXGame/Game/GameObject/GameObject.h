#pragma once
#include"SkinningObject.h"
#include"Camera.h"

//モデル描画に必要なデータを持つクラス
class GameObject {

protected:

	static DaiEngine::Camera* camera_;

public:

	/// <summary>
	/// カメラのポインタ取得
	/// </summary>
	/// <param name="camera"></param>
	static void GetCamera(DaiEngine::Camera* camera) { camera_ = camera; }


	GameObject(std::string modelName);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	std::unique_ptr<DaiEngine::SkinningObject> model_;

	DaiEngine::Skeleton skeleton_;

	std::unique_ptr<DaiEngine::SkinCluster> skinCluster_ = nullptr;
};