#pragma once
class Vector3
{
public:
	float x; //　x成分
	float y; //  y成分
	float z; //  z成分

	//　コンストラクタ
	Vector3();
	Vector3(float x, float y, float z);

	//　メンバ関数
	float length() const;
	Vector3& normalize();
	float dot(const Vector3& v) const;
	float cross(const Vector3& v) const;

	//　単項演算子オーバーロード
	Vector3 operator+() const;
	Vector3 operator-() const;

	//　代入演算子オーバーロード
	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);
};

