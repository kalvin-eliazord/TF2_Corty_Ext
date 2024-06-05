#pragma once
struct Vector3
{
	float x, y, z;
	Vector3 operator+(const Vector3& v2);
	Vector3 operator-(const Vector3& v2);
	float operator*(const Vector3& v2);
	bool operator<(const Vector3& v2);
};

