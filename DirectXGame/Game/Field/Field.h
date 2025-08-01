/**
 * @file Field.h
 * @brief ステージ全般の機能を管理
 * @author KATO
 * @date 2025/04/18
 */

#pragma once
#include"InstancingGameObject/InstancingGameObject.h"

#include <list>
#include <cassert>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>
#include <unordered_map>
#include <unordered_set>

#include"Input.h"

namespace std {
	template <>
	struct hash<Vector2> {
		size_t operator()(const Vector2& v) const {
			size_t hx = std::hash<float>{}(v.x);
			size_t hy = std::hash<float>{}(v.y);
			return hx ^ (hy << 1); // XOR + シフトで合成
		}
	};
}

struct Vector2Hash {
	std::size_t operator()(const Vector2& v) const noexcept {
		std::hash<float> hasher;
		std::size_t h1 = hasher(v.x);
		std::size_t h2 = hasher(v.y);
		return h1 ^ (h2 << 1);
	}
};

struct Block {
	DaiEngine::WorldTransform world;
	Vector4 color;
	Vector2 massLocation;
	float baseY;
	float tempYOffset;
	bool isDrawing;
};

struct WaveInfo {
	Vector2 center;
	float radius;
	float amplitude;
	int waveCount;
	float speed;
	float time = 0.0f;
	bool active = true;
	int currentWave = 0;
	std::unordered_map<Vector2, float> baseHeights;
	std::unordered_set<Vector2> finishedBlocks;
};

struct TargetInfo {
	Vector2 massLocation;  // 対象ブロックのマス座標（識別用）
	Vector3 targetPosition; // ブロックを移動させたい最終ワールド座標
};


class Field {
public:

	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	//ステージの開始
	void StartStage();

	//ステージの状態リセット
	void ResetStage();

	//ステージの削除
	void DeleteStage();

	//ステージの生成
	void CreateStage();

	//各ブロックの高さの限界値を設定
	void SetBlockHeightLimit(float heightLimit);

	//座標を基にブロック位置を取得する
	Vector2 GetBlockAt(float x, float z);

	Vector2 GetNearestBlockAt(float x, float z);

	Block* GetBlock(float x, float z);

	//現在位置のブロックを基にそのブロックのY座標を計算して返す
	float GetMassLocationPosY(Vector3 translate);

	//ステージの開始演出が終わっているか(Falseなら終了済み)
	bool GetStageAnimationFinishedFlag() { return isAnimationReset_; }

	/// <summary>
	/// 指定ブロックを中心に周囲の高さを変える(減衰なし)
	/// 第一引数:中心地点
	/// 第二引数:下がる範囲
	/// 第三引数:中心地点の下がる量
	/// </summary>
	void RaiseBlocksAround(const Vector2& center, float radius, float deltaY);

	/// <summary>
	/// 指定ブロックを中心に周囲の高さを変える(減衰あり)
	/// 第一引数:中心地点
	/// 第二引数:下がる範囲
	/// 第三引数:中心地点の下がる量
	/// </summary>
	void RaiseBlocksAroundWithAttenuation(const Vector2& center, float radius, float deltaY);

	/// <summary>
	/// 指定ブロックを中心に波を発生させる
	/// 第一引数:発生地点
	/// 第二引数:発生範囲
	/// 第三引数:波の高さ
	/// 第四引数：波の回数
	/// 第五引数：波の速さ
	/// </summary>
	void AddWave(const Vector2& center, float radius, float amplitude, int waveCount, float speed);

	//プレイヤーが歩けるかどうかの判定
	bool IsWalkable(const Vector3& worldPos);

	//近くの歩ける足場を探す関数
	std::optional<Vector3> FindNearestWalkable(const Vector3& from);

	//下がる量の取得
	float GetDeltaY() { return deltaY_; }

	float GetBlockWidth() { return blockWidth_; }

	//指定した座標へ動かす
	void MoveBlocksToTargets(const std::vector<TargetInfo>& targets, float deltaTime);
	std::vector<TargetInfo> GetBlockPositions() const;
	void MoveStage(const std::vector<TargetInfo>& targets);

private:
	//各ブロックの生成
	void CreateBlocks(const int x, const int z);

	//ステージの開始/リセット演出
	void PlayStageIntroAnimation(float deltaTime);

	//各ブロックの高さを限界値内に修正
	void FixedHeightCorrection();

	//波の更新処理
	void WaveUpdate();

	/// <summary>
	/// 各ブロックの高さで色を変える
	/// 第一引数:Yが高い時の色の最大値
	/// 第二引数:Yが低い時の色の最大値
	/// 第三引数:中心Y座標
	/// 第四引数:+-範囲を色が最大となる値
	/// </summary>
	void ColorAdjustmentByHeight(const Vector4& highColor, const Vector4& lowColor, float centerY, float rangeY);

private:
	DaiEngine::WorldTransform worldTransform_;

	DaiEngine::Input* input_ = nullptr;

	std::string tag_ = "Block";

	std::unique_ptr<InstancingGameObject> instancingObj_ = nullptr;
	std::unique_ptr<InstancingGameObject> instancingObj2_ = nullptr;

	Vector3 centerBlockPos_ = { 0.0f,0.0f,0.0f };//基準となる中心ブロックの座標
	float blockSize_ = 1.0f;//ブロックのサイズ

	std::list<Block> blocks_;//各種ブロック
	int verticalSize_ = 20;//縦のブロック数
	int horizontalSize_ = 20;//横のブロック数
	float blockWidth_ = 2.0f;//ブロック間隔
	float prevBlockWidth_ = blockWidth_;//前フレームのブロック間隔

	bool isAnimationReset_ = false;//リセット用のアニメーションフラグ
	float elapsedTime_ = 0.0f;//アニメーション経過時間
	float deltaTime_ = 0.0f;//フレーム時間間隔
	float deltaPlusTime_ = 0.0f;//deltaTime_に加算する値設定用

	//テスト用変数
	Vector2 nowPos_ = { 0.0f,0.0f };//現在の指定位置
	float radius_ = 1.0f;//中心からの範囲
	float deltaY_ = -0.5f;//一度に下がる値
	float heightLimit_ = 2.5f;//ブロックの高さ限界値+heightLimit ~ -heightLimitの範囲
	Vector4 highColor_ = { 1.0f, 0.0f, 0.0f, 1.0f };//Yが高い時の色
	Vector4 lowColor_ = { 0.0f, 0.0f, 1.0f, 1.0f };//Yが低い時の色
	int waveCount_ = 1;//波の発生回数
	float waveSpeed_ = 0.01f;//波の速度

	std::vector<WaveInfo> waves_; //現在アクティブな波のリスト

	bool targetMoveFinished_ = false;   // ブロック移動完了したか
	bool isTargets_ = false;

	std::vector<TargetInfo> targets_;   // 移動先の座標群
	std::vector<TargetInfo> targets2_;   // 移動先の座標群
};