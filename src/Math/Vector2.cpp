#include "Vector2.h"

Vector2::Vector2():x(0),y(0)
{

}

Vector2::Vector2(float nx, float ny):x(nx),y(ny)
{

}

Vector2::Vector2(const Vector2& vector):x(vector.x),y(vector.y)
{
}

Vector2::Vector2(Vector2&& vector):x(vector.x),y(vector.y)
{
}

Vector2::~Vector2()
{

}

Vector2& Vector2::operator= (const Vector2& a)
{
	x = a.x;
	y = a.y;
	return *this;
}

Vector2& Vector2::operator= (Vector2&& a)
{
	x = a.x;
	y = a.y;
	return *this;
}

Vector2& Vector2::operator+= (const Vector2& a)
{
	x += a.x;
	y += a.y;
	return *this;
}

Vector2& Vector2::operator+= (Vector2&& a)
{
	x += a.x;
	y += a.y;
	return *this;
}

Vector2& Vector2::operator-= (const Vector2& a)
{
	x -= a.x;
	y -= a.y;
	return *this;
}

Vector2& Vector2::operator-= (Vector2&& a)
{
	x -= a.x;
	y -= a.y;
	return *this;
}

Vector2& Vector2::operator*= (float a)
{
	x *= a;
	y *= a;
	return *this;
}

Vector2& Vector2::operator/= (float a)
{
	x /= a;
	y /= a;
	return *this;
}

bool Vector2::operator== (const Vector2& a)
{
	return x == a.x && y == a.y;
}

bool Vector2::operator== (Vector2&& a)
{
	return x == a.x && y == a.y;
}

Vector2 operator+ (const Vector2& a, const Vector2& b)
{
	return Vector2(a.x + b.x, a.y + b.y);
}

Vector2 operator+ (Vector2&& a, const Vector2& b)
{
	return Vector2(a.x + b.x, a.y + b.y);
}

Vector2 operator- (const Vector2& a, const Vector2& b)
{
	return Vector2(a.x - b.x, a.y - b.y);
}

Vector2 operator- (Vector2&& a, const Vector2& b)
{
	return Vector2(a.x - b.x, a.y - b.y);
}

Vector2 operator* (float a, const Vector2& b)
{
	return Vector2(a * b.x, a * b.y);
}

Vector2 operator* (float  a, Vector2&& b)
{
	return Vector2(a * b.x, a * b.y);
}

Vector2 operator* (const Vector2& a, float b)
{
	return Vector2(a.x * b, a.y * b);
}

Vector2 operator* (Vector2&& a, float b)
{
	return Vector2(a.x * b, a.y * b);
}

Vector2 operator/ (const Vector2& a, float b)
{
	return Vector2(a.x / b, a.y / b);
}

Vector2 operator/ (Vector2&& a, float b)
{
	return Vector2(a.x / b, a.y / b);
}

float operator* (const Vector2& a, const Vector2& b)
{
	return a.x * b.x + a.y * b.y;
}

float operator* (Vector2&& a, const Vector2& b)
{
	return a.x * b.x + a.y * b.y;
}

Vector2 Vector2::Right()
{
	return Vector2(1, 0);
}

Vector2 Vector2::Up()
{
	return Vector2(0, 1);
}
