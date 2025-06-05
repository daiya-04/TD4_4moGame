#pragma once


class BaseUI {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

};

