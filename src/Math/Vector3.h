#pragma once

class Vector2;
class Vector4;
class Vector3
{
public:
	Vector3();
	Vector3(float nx, float ny, float nz);
	Vector3(const Vector2& vector, float nz);
	Vector3(Vector2&& vector, float nz);
	Vector3(const Vector3& vector);
	Vector3(Vector3&& vector);
	Vector3(const Vector4& vector);
	Vector3(Vector4&& vector);
	~Vector3();

	Vector3& operator= (const Vector3& a);
	Vector3& operator= (Vector3&& a);
	Vector3& operator+= (const Vector3& a);
	Vector3& operator+= (Vector3&& a);
	Vector3& operator-= (const Vector3& a);
	Vector3& operator-= (Vector3&& a);
	Vector3& operator*= (float a);
	Vector3& operator/= (float a);

	bool operator== (const Vector3& a);
	bool operator== (Vector3&& a);

	friend Vector3 operator+ (const Vector3& a, const Vector3& b);
	friend Vector3 operator+ (Vector3&& a, const Vector3& b);
	friend Vector3 operator- (const Vector3& a, const Vector3& b);
 	friend Vector3 operator- (Vector3&& a, const Vector3& b);
	
	friend Vector3 operator* (float a, const Vector3& b);
	friend Vector3 operator* (float a, Vector3&& b);
	friend Vector3 operator* (const Vector3& a, float b);
	friend Vector3 operator* (Vector3&& a, float b);

	friend Vector3 operator/ (const Vector3& a, float b);
	friend Vector3 operator/ (Vector3&& a, float b);
	
	friend float operator* (const Vector3& a, const Vector3& b);
	friend float operator* (Vector3&& a, const Vector3& b);

	friend Vector3 operator^ (const Vector3& a, const Vector3& b);
	friend Vector3 operator^ (Vector3&& a, const Vector3& b);

	static Vector3 Forward();
	static Vector3 Right();
	static Vector3 Up();

	float x;
	float y;
	float z;

};
