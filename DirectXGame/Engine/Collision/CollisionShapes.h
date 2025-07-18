#pragma once
#include "Vec3.h"
#include "Vec2.h"

namespace Shapes{
	// 線分
	struct Segment {
		Vector3 origin; // 始点
		Vector3 diff;   // 終点との差分ベクトル
	};

	// 直線
	struct StraightLine {
		Vector3 origin; // 始点
		Vector3 diff;   // 終点との差分ベクトル
	};

	// 半直線
	struct Ray {
		Vector3 origin; // 始点
		Vector3 diff;   // 終点との差分ベクトル
	};

	// 球
	struct Sphere {
		Vector3 center;  // 中心点
		float radius; // 半径
	};

	// 平面
	struct Plane {
		Vector3 normal;    // 法線
		float distance; // 原点からの距離
	};

	// 三角形
	struct Triangle {
		Vector3 vertices[3]; // 頂点
	};
	//AABB
	struct AABB {
		Vector3 min;
		Vector3 max;
	};
	//OBB
	struct OBB {
		Vector3 center;
		Vector3 orientation[3];
		Vector3 size;
	};

	//矩形
	struct RectAngle {
		float left;
		float right;
		float top;
		float bottom;
	};

	//円
	struct Circle {
		Vector2 center;
		float radius;
	};

	//円柱
	struct Cylinder {
		Vector3 topCenter;    //円柱の上端の中心
		Vector3 bottomCenter; //円柱の下端の中心
		float radius;         //円柱の半径
	};
}
