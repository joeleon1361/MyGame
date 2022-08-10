#pragma once
class Vector3
{
public:
	float x; //�@x����
	float y; //  y����
	float z; //  z����

	//�@�R���X�g���N�^
	Vector3();
	Vector3(float x, float y, float z);

	//�@�����o�֐�
	float length() const;
	Vector3& normalize();
	float dot(const Vector3& v) const;
	float cross(const Vector3& v) const;

	//�@�P�����Z�q�I�[�o�[���[�h
	Vector3 operator+() const;
	Vector3 operator-() const;

	//�@������Z�q�I�[�o�[���[�h
	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);
};

