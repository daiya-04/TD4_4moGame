#include "BossBulletManager.h"

#include"ColliderManager.h"
#include"ShapesDraw.h"

#include"Boss//Bullet//bullets/Fall/BossFallBullet.h"
#include"Boss//Bullet//bullets/None/BossNoneBullet.h"
#include"Boss//Bullet//bullets/Parabola/BossParabolaBullet.h"


BossBulletManager::BossBulletManager(bool isCandy)
{
	//オブジェクト生成
	if (isCandy) {
		InstancingGameObject::Init("Candy", 100);
	}
	else {
		InstancingGameObject::Init("Cream", 100);
	}

	collider_ = std::make_unique<DaiEngine::CylinderCollider>();
	collider_->Init("wave", world_, radius_ , 2.0f);
	DaiEngine::ColliderManager::GetInstance()->AddCollider(collider_.get());
	collider_->SetStayCallback([this](DaiEngine::Collider*colidier) {if(colidier->GetTag()=="player")collider_->ColliderOff(); });
	collider_->ColliderOff();


	//警告円の生成
	dangerZone_ = std::make_unique<InstancingGameObject>();
	dangerZone_->Init("DangerZone", 100);

	tree_.name_ = "FallingBullet";
	tree_.SetValue("spawnHeight", &bulletStartHeight_);
	tree_.SetValue("fallSpeed", &fallSpeed_);
	tree_.SetValue("colliderRadius", &colliderRadius_);

	GvariTree tree;
	tree.name_ = "parabolaBullet";
	tree.SetValue("parabolaHeight", &parabolaHeight_);
	tree.SetValue("arriveCount", &arriveCount_);
	//ツリーに追加
	tree_.SetTreeData(tree);
}

void BossBulletManager::Update()
{

	//更新して座標設定
	for (auto& data : bullets_) {
		data->Update();
		//死んでいない場合データをセット
		if (!data->GetDead()) {
			//専用変数生成
			DaiEngine::InstancingObjData objData;
			//ワールドデータコピー
			objData.worldTransform_ = data->GetWorld();
			//行列更新
			objData.worldTransform_.UpdateMatrix();

			//通常時のみ警告円群の更新
			if (data->GetType() != BulletType::None && data->GetType() != BulletType::Wave) {
				//データセット
				SetData(objData);
				//警告円の更新
				DaiEngine::InstancingObjData dangerData;
				dangerData.worldTransform_ = data->data_.warningWorld;
				dangerData.worldTransform_.UpdateMatrix();
				dangerZone_->SetData(dangerData);
			}

		}
	}

	//死んだ弾を削除
	bullets_.remove_if([&](auto& data) {
		if (data->GetDead()) {
			if (data->GetType() == BulletType::Wave) {
				collider_->ColliderOn();
			}
			return true;
		}
		else {
			return false;
		}
		});


	Matrix4x4 rotateM = MakeRotateXMatrix(0) * MakeRotateYMatrix(0) * MakeRotateZMatrix(0);
	collider_->Update(rotateM);
}

void BossBulletManager::Draw()
{
	//描画
	InstancingGameObject::Draw();

	//警告円の描画
	dangerZone_->Draw();

	//弾コライダー描画
#ifdef _DEBUG
	ShapesDraw::DrawCylinder(std::get<Shapes::Cylinder>(collider_->GetShape()), *camera_, { 1,1,1,1 });
	for (auto& bullet : bullets_) {
		bullet->ColliderDraw();
	}
#endif // _DEBUG

}

void BossBulletManager::SpawnBullet(const DaiEngine::WorldTransform& pos, BulletType type, const DaiEngine::WorldTransform& boss)
{
	//座標作成
	Vector3 position = pos.translation_;
	//渡すパラメータ設定
	BossBulletData data;
	data.type = type;
	data.bossWorld = boss;

	//指定値高くする
	position.y = bulletStartHeight_;
	data.velocity = Vector3{ 0,-1.0f,0 }*fallSpeed_;

	//警告円
	data.warningWorld.Init();
	data.warningWorld = pos;
	//ワールド初期化
	data.world.Init();
	data.radius = pos.scale_.x;
	data.colliderRadius = colliderRadius_;
	//座標設定
	data.world.translation_ = position;

	//放物線用の設定
	data.arriveCount = arriveCount_;
	data.parabolaHeight = parabolaHeight_;

	std::unique_ptr<IBossBullet>bullet;
	//落下弾の場合
	if (type == BulletType::Fall) {
		bullet = std::make_unique<BossFallBullet>(data, camera_);
	}
	else if (type == BulletType::Parabola) {
		bullet = std::make_unique<BossParabolaBullet>(data, camera_);
	}
	else {

		//Diveはボス位置に生成
		if (type == BulletType::Dive) {
			data.world.translation_ = boss.translation_;
		}

		data.world.translation_.y = pos.translation_.y;

		bullet = std::make_unique<BossNoneBullet>(data, camera_);
	}

	
	//配列に追加
	bullets_.emplace_back(std::move(bullet));
}
