/**
 * @file Field.h
 * @brief ステージ全般の機能を管理
 * @author KATO
 * @date 2025/04/18
 */

#include "Field.h"
#include"ImGuiManager.h"

#include"GlobalVariable/Group/GlobalVariableGroup.h"

#include <limits>

void Field::Initialize() {
	//Input
	input_ = DaiEngine::Input::GetInstance();

	//Instancingゲームオブジェクト
	instancingObj_ = std::make_unique<InstancingGameObject>();
	instancingObj_->Init("Cube", 10000);

	instancingObj2_ = std::make_unique<InstancingGameObject>();
	instancingObj2_->Init("Field", 10000);

	worldTransform_.Init();

	//
	std::unique_ptr<GVariGroup>gvg = std::make_unique<GVariGroup>("Field");
	gvg->SetValue("BlockWidth", &blockWidth_);
	gvg->SetValue("Radius", &radius_);
	gvg->SetValue("DeltaY", &deltaY_);
	gvg->SetValue("WaveCount", &waveCount_);
	gvg->SetValue("WaveSpeed", &waveSpeed_);
	gvg->SetValue("HeightLimit", &heightLimit_);
	gvg->SetValue("VerticalSize", &verticalSize_);
	gvg->SetValue("HorizontalSize", &horizontalSize_);
	gvg->SetValue("BlockSize", &blockSize_);

	//ステージを生成
	CreateStage();

	//ステージ開始演出
	StartStage();
}

void Field::Update() {

#ifdef _DEBUG
	ImGui::Begin("TestOperate");
	if (ImGui::Button("TestRaiseBlocksAround")) {
		//現在のnowPos_の位置からradius_範囲をdeltaY_分下げる
		RaiseBlocksAround(GetBlockAt(nowPos_.x, nowPos_.y), radius_, deltaY_);
	}
	if (ImGui::Button("TestRaiseBlocksAroundWithAttenuation")) {
		//現在のnowPos_の位置からradius_範囲をdeltaY_分下げる(距離減衰付き)
		RaiseBlocksAroundWithAttenuation(GetBlockAt(nowPos_.x, nowPos_.y), radius_, deltaY_);
	}
	if (ImGui::Button("WaveBlocks")) {
		AddWave(GetBlockAt(nowPos_.x, nowPos_.y), radius_, heightLimit_, waveCount_ , waveSpeed_);
	}
	if (ImGui::Button("TestSetBlockHeightLimit")) {
		//各ブロックの高さを限界値で固定
		SetBlockHeightLimit(heightLimit_);
	}
	if (ImGui::Button("StartStage")) {
		//ステージ開始演出
		Finalize();

		CreateStage();

		StartStage();
	}
	if (ImGui::Button("ResetStage")) {
		//ステージ状態をリセット
		ResetStage();
	}
	ImGui::End();

#endif // _DEBUG

	//ステージ開始/リセット演出
	if (isAnimationReset_ == true) {
		deltaTime_ += deltaPlusTime_;
		PlayStageIntroAnimation(deltaTime_);
	}

	//変数呼び出し
	DaiEngine::InstancingObjData data2;
	data2.worldTransform_.translation_.y = -15000.0f;
	//データ追加
	instancingObj2_->SetData(data2);

	//高さを限界値内に修正
	FixedHeightCorrection();

	//各ブロックの高さに応じて色を変更
	ColorAdjustmentByHeight(highColor_, lowColor_, 0.0f, heightLimit_);

#ifdef _DEBUG
	//現在のnowPos_に対応するブロックを赤くする
	Vector2 selected = GetBlockAt(nowPos_.x, nowPos_.y);
	for (Block& block : blocks_) {
		if (block.massLocation.x == selected.x && block.massLocation.y == selected.y) {
			block.color = { 1.0f, 0.0f, 0.0f, 1.0f };//赤
		}
	}
#endif

	//ブロック間隔の更新
	if (blockWidth_ != prevBlockWidth_) {
		for (Block& block : blocks_) {
			//ステージの中央からのオフセットを計算
			float offsetX = (horizontalSize_ - 1) * 0.5f * blockWidth_;
			float offsetZ = (verticalSize_ - 1) * 0.5f * blockWidth_;

			//ブロックの配置座標を計算
			block.world.translation_ = {
				centerBlockPos_.x + (block.massLocation.x * blockWidth_) - offsetX,
				centerBlockPos_.y,
				centerBlockPos_.z + (block.massLocation.y * blockWidth_) - offsetZ
			};
		}

		//更新
		prevBlockWidth_ = blockWidth_;
	}

	//波の更新
	WaveUpdate();

	//各ブロックの行列更新
	for (Block& block : blocks_) {
		block.world.UpdateMatrix();
	}

	//専用描画構造体に切り換えて設定
	for (Block& block : blocks_) {
		
		//変数呼び出し
		DaiEngine::InstancingObjData data;
		data.worldTransform_ = block.world;
		data.color_ = block.color;

		if (block.world.translation_.y <= -heightLimit_) continue;

		//データ追加
		instancingObj_->SetData(data);
	}
}

void Field::Draw() {
	//セットされたデータ分描画してセットデータ削除
	instancingObj_->Draw();

	instancingObj2_->Draw();
}

void Field::Finalize() {
	blocks_.clear();
}

void Field::StartStage() {
	isAnimationReset_ = true;
	deltaTime_ = 0.0f;
	elapsedTime_ = 0.0f;
	deltaPlusTime_ = 1.0f / 2400.0f;
}

void Field::ResetStage() {
	isAnimationReset_ = true;
	deltaTime_ = 0.0f;
	elapsedTime_ = 0.0f;
	deltaPlusTime_ = 1.0f / 600.0f;
}

void Field::DeleteStage() {
	//ブロックを削除
	blocks_.clear();
}

void Field::SetBlockHeightLimit(float heightLimit) {
	//高さの限界値を設定
	heightLimit_ = heightLimit;
}

float Field::GetMassLocationPosY(Vector3 translate) {
	//現在のマスを確認する
	Vector2 selected = GetNearestBlockAt(translate.x, translate.z);
	for (Block& block : blocks_) {
		if (block.massLocation.x == selected.x && block.massLocation.y == selected.y) {
			//プレイヤーの位置をnowPos_に記録
			nowPos_ = { translate.x,translate.z };
			//現在のブロック座標Y + ブロックのサイズ(半径)を返す
			return block.world.translation_.y + blockSize_;
		}
	}

	//何もなければそのままを返す
	return translate.y;
}

void Field::CreateBlocks(const int x, const int z) {
	Block block;

	block.world.Init();

	//ステージの中央からのオフセットを計算
	float offsetX = (horizontalSize_ - 1) * 0.5f * blockWidth_;
	float offsetZ = (verticalSize_ - 1) * 0.5f * blockWidth_;

	//ブロックの配置座標を計算
	block.world.translation_ = {
		centerBlockPos_.x + (x * blockWidth_) - offsetX,
		centerBlockPos_.y,
		centerBlockPos_.z + (z * blockWidth_) - offsetZ
	};

	block.world.rotation_ = { 0.0f,0.0f,0.0f };
	block.world.scale_ = { 0.0f,0.0f,0.0f };

	block.color = { 1.0f,1.0f,1.0f,1.0f };

	//マス位置xとzを保存
	block.massLocation = { (float)x,(float)z };

	//最初のY座標を記録
	block.baseY = block.world.translation_.y;

	blocks_.push_back(block);
}

void Field::CreateStage() {
	//各ブロックの生成
	for (int i = 0; i < verticalSize_; i++) {
		for (int j = 0; j < horizontalSize_; j++) {
			CreateBlocks(i, j);
		}
	}

	//限界値の設定
	SetBlockHeightLimit(heightLimit_);
}

Vector2 Field::GetBlockAt(float x, float z) {
	for (Block& block : blocks_) {
		const Vector3& blockPos = block.world.translation_;

		//各軸面上でブロックの中心からBlockSize_の範囲か確認
		if (std::abs(blockPos.x - x) <= blockSize_ &&
			std::abs(blockPos.z - z) <= blockSize_) {
			return block.massLocation;
		}
	}

	return Vector2{ -1,-1 };//見つからなければ適当な値
}

Vector2 Field::GetNearestBlockAt(float x, float z) {
	float minDistanceSq = (std::numeric_limits<float>::max)();
	Block* nearestBlock = nullptr;

	for (Block& block : blocks_) {
		const Vector3& pos = block.world.translation_;

		float dx = pos.x - x;
		float dz = pos.z - z;
		float distSq = dx * dx + dz * dz;

		if (distSq < minDistanceSq) {
			minDistanceSq = distSq;
			nearestBlock = &block;
		}
	}

	if (nearestBlock) {
		return nearestBlock->massLocation;
	}
	else {
		return Vector2{ -1, -1 }; // 念のためfallback
	}
}

Block* Field::GetBlock(float x, float z) {
	Block* nearestBlock = nullptr;
	float minDistanceSquared = (std::numeric_limits<float>::max)();

	for (Block& block : blocks_) {
		const Vector3& blockPos = block.world.translation_;
		float dx = blockPos.x - x;
		float dz = blockPos.z - z;
		float distanceSquared = dx * dx + dz * dz;

		if (distanceSquared < minDistanceSquared) {
			minDistanceSquared = distanceSquared;
			nearestBlock = &block;
		}
	}

	return nearestBlock;
}

void Field::PlayStageIntroAnimation(float deltaTime) {
	if (!isAnimationReset_) return;

	elapsedTime_ += deltaTime;

	bool allFinished = true;

	for (Block& block : blocks_) {
		Vector3& pos = block.world.translation_;
		Vector3& rot = block.world.rotation_;
		Vector3& scale = block.world.scale_;

		//中心ブロックとの距離を使ってディレイを計算
		float dx = pos.x - centerBlockPos_.x;
		float dz = pos.z - centerBlockPos_.z;
		float distanceFromCenter = std::sqrt(dx * dx + dz * dz);

		float startDelay = distanceFromCenter * 0.2f;//距離に応じた遅延
		float localTime = elapsedTime_ - startDelay;

		if (localTime < 0.0f) {
			allFinished = false;//まだ開始してないブロックあり
			continue;
		}

		//**std::minに変更、問題なければこのコメントを削除

		float animDuration = 1.0f;//各ブロックのアニメ時間
		float t = std::min(localTime / animDuration, 1.0f);

		//イージング（smoothstep風）
		float easedT = t * t * (3.0f - 2.0f * t);

		//回転：最終的に0に戻す（0 → 最大回転 → 0）
		float angle = std::sin(easedT * 3.14159f) * 3.14159f;//πラジアン（180°）回転して戻る
		rot = { 0.0f, angle, 0.0f };

		//アニメーション適用
		pos.y = -5.0f + easedT * 5.0f;
		scale = { easedT * blockSize_, easedT * blockSize_, easedT * blockSize_ };

		if (t < 1.0f) {
			allFinished = false;//このブロックはまだ完了してない
		}
	}

	if (allFinished) {
		isAnimationReset_ = false;//全ブロック完了で演出終了
	}
}

void Field::RaiseBlocksAround(const Vector2& center, float radius, float deltaY) {
	//基準となる中心ブロックのワールド座標を取得
	Vector3 centerPos{};
	bool found = false;

	for (const Block& block : blocks_) {
		if (block.massLocation.x == center.x && block.massLocation.y == center.y) {
			centerPos = block.world.translation_;
			found = true;
			break;
		}
	}

	//中心ブロックが見つからない場合は何もしない
	if (!found) {
		return;
	}

	//半径範囲内にあるブロックを下げる
	for (Block& block : blocks_) {
		Vector3 pos = block.world.translation_;

		//中心座標からの距離を計算（XZ平面）
		float dx = pos.x - centerPos.x;
		float dz = pos.z - centerPos.z;
		float distance = std::sqrt(dx * dx + dz * dz);

		if (distance <= radius) {
			block.world.translation_.y += deltaY;
		}
	}
}

void Field::RaiseBlocksAroundWithAttenuation(const Vector2& center, float radius, float deltaY) {
	//基準となる中心ブロックのワールド座標を取得
	Vector3 centerPos{};
	bool found = false;

	//
	for (const Block& block : blocks_) {
		if (block.massLocation.x == center.x && block.massLocation.y == center.y) {
			centerPos = block.world.translation_;
			found = true;
			break;
		}
	}

	//中心ブロックが見つからない場合は何もしない
	if (!found) {
		return;
	}

	//半径範囲内にあるブロックを下げる（中心に近いほど多く動かす）
	for (Block& block : blocks_) {
		Vector3 pos = block.world.translation_;

		// 中心座標からの距離を計算（XZ平面）
		float dx = pos.x - centerPos.x;
		float dz = pos.z - centerPos.z;
		float distance = std::sqrt(dx * dx + dz * dz);

		if (distance <= radius) {
			//線形減衰（距離が大きいほど減少量は少なくなる）
			float attenuation = 1.0f - (distance / radius);//1.0〜0.0f
			float adjustedDeltaY = deltaY * attenuation;
			block.world.translation_.y += adjustedDeltaY;
		}
	}
}

void Field::AddWave(const Vector2& center, float radius, float amplitude, int waveCount, float speed) {
	WaveInfo wave;
	wave.center = center;
	wave.radius = radius;
	wave.amplitude = amplitude;
	wave.waveCount = waveCount;
	wave.speed = speed;
	wave.time = 0.0f;
	wave.currentWave = 0;
	wave.active = true;

	Vector3 centerPos{};
	bool found = false;
	for (const Block& block : blocks_) {
		if (block.massLocation == center) {
			centerPos = block.world.translation_;
			found = true;
			break;
		}
	}
	if (!found) return;

	// 波が影響するブロックとその高さを保存
	for (const Block& block : blocks_) {
		float dx = block.world.translation_.x - centerPos.x;
		float dz = block.world.translation_.z - centerPos.z;
		float distance = std::sqrt(dx * dx + dz * dz);

		if (distance <= radius) {
			wave.baseHeights[block.massLocation] = block.world.translation_.y;
		}
	}

	waves_.push_back(wave);
}

bool Field::IsWalkable(const Vector3& worldPos) {
	Vector2 gridPos = GetBlockAt(worldPos.x, worldPos.z);

	for (const Block& block : blocks_) {
		if (block.massLocation == gridPos) {
			float blockY = block.world.translation_.y - blockSize_ / 2;

			// 最低高度のブロックは乗れない
			if (blockY <= -heightLimit_) {
				return false;
			}
		}
	}

	return true;
}

std::optional<Vector3> Field::FindNearestWalkable(const Vector3& from) {
	Vector2 baseGrid = GetBlockAt(from.x, from.z);

	int searchRange = 10; // 半径3 (7x7範囲)
	for (int dx = -searchRange; dx <= searchRange; dx++) {
		for (int dz = -searchRange; dz <= searchRange; dz++) {
			Vector2 grid = { baseGrid.x + dx, baseGrid.y + dz };

			for (const Block& block : blocks_) {
				if ((block.massLocation - grid).Length() < 0.01f) {
					float blockY = block.world.translation_.y - blockSize_ / 2;
					if (blockY > -heightLimit_) {
						Vector3 pos;
						pos.x = block.world.translation_.x;
						pos.y = block.world.translation_.y + blockSize_ / 2;
						pos.z = block.world.translation_.z;
						return pos;
					}
				}
			}
		}
	}
	return std::nullopt;
}

void Field::FixedHeightCorrection() {
	for (Block& block : blocks_) {
		if (block.world.translation_.y >= heightLimit_) {//+heightlimit_より大きければ
			block.world.translation_.y = heightLimit_;
		}
		else if (block.world.translation_.y <= -heightLimit_) {//-heightlimit_より小さければ
			block.world.translation_.y = -heightLimit_;

		}
		else {//範囲内なら

		}
	}
}

void Field::WaveUpdate() {
	// 各ブロックの一時的オフセットを初期化
	for (Block& block : blocks_) {
		block.tempYOffset = 0.0f;
	}

	// 各波の影響を加算
	for (WaveInfo& wave : waves_) {
		if (!wave.active) continue;

		wave.time += wave.speed;
		float wavePhase = wave.time * 2.0f * 3.14159f;

		if (wavePhase >= (wave.currentWave + 1) * 2.0f * 3.14159f) {
			wave.currentWave++;
			if (wave.currentWave >= wave.waveCount) {
				wave.active = false;

				// 終了した波の影響ブロックを記録
				for (const auto& [massLoc, baseY] : wave.baseHeights) {
					wave.finishedBlocks.insert(massLoc);
				}
				continue;
			}
		}

		Vector3 centerPos{};
		bool found = false;
		for (const Block& block : blocks_) {
			if (block.massLocation == wave.center) {
				centerPos = block.world.translation_;
				found = true;
				break;
			}
		}
		if (!found) continue;

		for (Block& block : blocks_) {
			auto it = wave.baseHeights.find(block.massLocation);
			if (it == wave.baseHeights.end()) continue;

			float dx = block.world.translation_.x - centerPos.x;
			float dz = block.world.translation_.z - centerPos.z;
			float distance = std::sqrt(dx * dx + dz * dz);

			float delay = std::pow(distance / wave.radius, 1.5f) * 3.14159f;
			float baseWave = std::sin(wavePhase - delay);
			baseWave = (std::max)(0.0f, baseWave);
			baseWave = std::pow(baseWave, 3.0f);

			float attenuation = (std::max)(0.0f, 1.0f - distance / wave.radius);
			float yOffset = baseWave * wave.amplitude * attenuation;

			block.tempYOffset += yOffset;
		}
	}

	// 波の影響 or 終了後リセット対象のブロックだけ補正
	for (Block& block : blocks_) {
		bool affected = (block.tempYOffset > 0.0f);
		bool needsReset = false;

		// どの波からもリセット対象になっているか確認
		for (const WaveInfo& wave : waves_) {
			if (!wave.active && wave.finishedBlocks.count(block.massLocation)) {
				needsReset = true;
				break;
			}
		}

		if (affected) {
			block.world.translation_.y = block.baseY + block.tempYOffset;
		}
		else if (needsReset) {
			// ゆっくり戻す
			block.world.translation_.y += (block.baseY - block.world.translation_.y) * 0.2f;

			// 戻ったらリセット対象解除（閾値を設ける）
			if (std::abs(block.world.translation_.y - block.baseY) < 0.01f) {
				for (WaveInfo& wave : waves_) {
					wave.finishedBlocks.erase(block.massLocation);
				}
			}
		}
	}
}

void Field::ColorAdjustmentByHeight(const Vector4& highColor, const Vector4& lowColor, float centerY, float rangeY) {
	for (Block& block : blocks_) {
		float y = block.world.translation_.y;

		//上方向
		if (y > centerY + rangeY) {
			block.color = highColor;
		}
		else if (y > centerY) {
			float t = (y - centerY) / rangeY;//0.0～1.0
			block.color = {
				1.0f * (1.0f - t) + highColor.x * t,
				1.0f * (1.0f - t) + highColor.y * t,
				1.0f * (1.0f - t) + highColor.z * t,
				1.0f * (1.0f - t) + highColor.w * t
			};
		}

		//下方向
		else if (y < centerY - rangeY) {
			block.color = lowColor;
		}
		else if (y < centerY) {
			float t = (centerY - y) / rangeY;//0.0～1.0
			block.color = {
				1.0f * (1.0f - t) + lowColor.x * t,
				1.0f * (1.0f - t) + lowColor.y * t,
				1.0f * (1.0f - t) + lowColor.z * t,
				1.0f * (1.0f - t) + lowColor.w * t
			};
		}

		//中心値：白
		else {
			block.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		}
	}
}