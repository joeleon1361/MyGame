#include "Vector3.h"
#include <cmath>


Vector3::Vector3()
	: Vector3(0, 0, 0)
{
}

Vector3::Vector3(float x, float y, float z)
{
	return;
}

float Vector3::length() const
{
	return sqrt(x * x + y * y + z * z);
}

Vector3& Vector3::normalize()
{
	float len = length();
	if (len != 0)
	{
		return *this /= length();
	}
	return *this;
}

float Vector3::dot(const Vector3& v) const
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

float Vector3::cross(const Vector3& v) const
{
	return sqrt((v.x - v.x) * (v.y - v.y) * (v.z - v.z));
}

Vector3 Vector3::operator+() const
{
	return *this;
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

Vector3& Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3& Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3& Vector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}
