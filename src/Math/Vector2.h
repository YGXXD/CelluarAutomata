#pragma once

class Vector2
{
public:
	Vector2();
	Vector2(float nx, float ny);
	Vector2(const Vector2& vector);
	Vector2(Vector2&& vector);
	~Vector2();

	Vector2& operator= (const Vector2& a);
	Vector2& operator= (Vector2&& a);
	Vector2& operator+= (const Vector2& a);
	Vector2& operator+= (Vector2&& a);
	Vector2& operator-= (const Vector2& a);
	Vector2& operator-= (Vector2&& a);
	Vector2& operator*= (float a);
	Vector2& operator/= (float a);

	bool operator== (const Vector2& a);
	bool operator== (Vector2&& a);

	friend Vector2 operator+ (const Vector2& a, const Vector2& b);
	friend Vector2 operator+ (Vector2&& a, const Vector2& b);
	friend Vector2 operator- (const Vector2& a, const Vector2& b);
 	friend Vector2 operator- (Vector2&& a, const Vector2& b);
	
	friend Vector2 operator* (float a, const Vector2& b);
	friend Vector2 operator* (float a, Vector2&& b);
	friend Vector2 operator* (const Vector2& a, float b);
	friend Vector2 operator* (Vector2&& a, float b);

	friend Vector2 operator/ (const Vector2& a, float b);
	friend Vector2 operator/ (Vector2&& a, float b);

	friend float operator* (const Vector2& a, const Vector2& b);
	friend float operator* (Vector2&& a, const Vector2& b);

	static Vector2 Right();
	static Vector2 Up();

	float x;
	float y;

};
