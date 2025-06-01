#pragma once
///---------------------------------------------------------------------------------------------
//
// Vector4
//
///---------------------------------------------------------------------------------------------

#include "Matrix44.h"

#include <cmath>
#include <algorithm>

//Vector4クラス
class Vector4 {
public:

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;

public:
	//コンストラクタ
	Vector4() {};
	Vector4(float x, float y, float z, float w) :x(x), y(y), z(z), w(w) {};
	Vector4(const Vector4& vector) :x(vector.x), y(vector.y), z(vector.z), w(vector.w) {};

	friend inline Vector4 operator*(const Matrix4x4& m, const Vector4& v) {
		Vector4 result;
		result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w;
		result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w;
		result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w;
		result.w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w;
		return result;
	}

};