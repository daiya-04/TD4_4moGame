#pragma once
#include"InstancingObject3d.h"


//インスタンシングゲームオブジェクト
class InstancingGameObject {

protected:

	static DaiEngine::Camera* camera_;

public:

	/// <summary>
	/// カメラのポインタ設定
	/// </summary>
	/// <param name="camera"></param>
	static void SetCamera(DaiEngine::Camera* camera) { camera_ = camera; }

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="modelName"></param>
	/// <param name="drawNum"></param>
	InstancingGameObject(const std::string& modelName, int drawNum);
	~InstancingGameObject() = default;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// データをセット
	/// </summary>
	/// <param name="data"></param>
	void SetData(DaiEngine::InstancingObjData& data) {
		datas_.emplace_back(data);
	}

private:

	std::unique_ptr<DaiEngine::InstancingObject3d> model_;

	std::list<DaiEngine::InstancingObjData> datas_;

};