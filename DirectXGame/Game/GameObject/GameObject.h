#pragma once
#include"SkinningObject.h"
#include"Camera.h"

#include<string>

//モデル描画に必要なデータを持つクラス
class GameObject {

protected:

	static DaiEngine::Camera* camera_;

public:

	DaiEngine::WorldTransform* world_ = nullptr;

public:

	/// <summary>
	/// カメラのポインタを設定
	/// </summary>
	/// <param name="camera"></param>
	static void SetCamera(DaiEngine::Camera* camera) { camera_ = camera; }


	GameObject() = default;
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
	DaiEngine::WorldTransform& GetWorld() { return *world_; }

	/// <summary>
	/// アニメーションをセット
	/// </summary>
	/// <param name="name"></param>
	void SetAnimationName(const std::string& name, bool isLoop=true);

private:

	std::unique_ptr<DaiEngine::SkinningObject> model_;

};