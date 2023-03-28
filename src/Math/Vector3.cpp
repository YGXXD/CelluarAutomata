#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"

Vector3::Vector3():x(0),y(0),z(0)
{

}

Vector3::Vector3(float nx, float ny, float nz):x(nx),y(ny),z(nz)
{

}

Vector3::Vector3(const Vector2& vector, float nz):x(vector.x),y(vector.y),z(nz)
{

}

Vector3::Vector3(Vector2&& vector, float nz):x(vector.x),y(vector.y),z(nz)
{

}

Vector3::Vector3(const Vector3& vector):x(vector.x),y(vector.y),z(vector.z)
{

}

Vector3::Vector3(Vector3&& vector):x(vector.x),y(vector.y),z(vector.z)
{

}

Vector3::Vector3(const Vector4& vector):x(vector.x / vector.w),y(vector.y / vector.w),z(vector.z / vector.w)
{

}

Vector3::Vector3(Vector4&& vector):x(vector.x / vector.w),y(vector.y / vector.w),z(vector.z / vector.w)
{

}

Vector3::~Vector3()
{

}

Vector3& Vector3::operator= (const Vector3& a)
{
	x = a.x;
	y = a.y;
	z = a.z;
	return *this;
}

Vector3& Vector3::operator= (Vector3&& a)
{
	x = a.x;
	y = a.y;
	z = a.z;
	return *this;
}

Vector3& Vector3::operator+= (const Vector3& a)
{
	x += a.x;
	y += a.y;
	z += a.z;
	return *this;
}

Vector3& Vector3::operator+= (Vector3&& a)
{
	x += a.x;
	y += a.y;
	z += a.z;
	return *this;
}

Vector3& Vector3::operator-= (const Vector3& a)
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	return *this;
}

Vector3& Vector3::operator-= (Vector3&& a)
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	return *this;
}

Vector3& Vector3::operator*= (float a)
{
	x *= a;
	y *= a;
	z *= a;
	return *this;
}

Vector3& Vector3::operator/= (float a)
{
	x /= a;
	y /= a;
	z /= a;
	return *this;
}

bool Vector3::operator== (const Vector3& a)
{
	return x == a.x && y == a.y && z == a.z;
}

bool Vector3::operator== (Vector3&& a)
{
	return x == a.x && y == a.y && z == a.z;
}

Vector3 operator+ (const Vector3& a, const Vector3& b)
{
	return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector3 operator+ (Vector3&& a, const Vector3& b)
{
	return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector3 operator- (const Vector3& a, const Vector3& b)
{
	return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vector3 operator- (Vector3&& a, const Vector3& b)
{
	return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vector3 operator* (float a, const Vector3& b)
{
	return Vector3(a * b.x, a * b.y, a * b.z);
}

Vector3 operator* (float  a, Vector3&& b)
{
	return Vector3(a * b.x, a * b.y, a * b.z);
}

Vector3 operator* (const Vector3& a, float b)
{
	return Vector3(a.x * b, a.y * b, a.z * b);
}

Vector3 operator* (Vector3&& a, float b)
{
	return Vector3(a.x * b, a.y * b, a.z * b);
}

Vector3 operator/ (const Vector3& a, float b)
{
	return Vector3(a.x / b, a.y / b, a.z / b);
}

Vector3 operator/ (Vector3&& a, float b)
{
	return Vector3(a.x / b, a.y / b, a.z / b);
}

float operator* (const Vector3& a, const Vector3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}	

float operator* (Vector3&& a, const Vector3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 operator^ (const Vector3& a, const Vector3& b)
{
	return Vector3
			   (a.y * b.z - a.z * b.y,
			   	b.x * a.z - a.x * b.z,
				a.x * b.y - b.x * a.y);
}

Vector3 operator^ (Vector3&& a, const Vector3& b)
{
	return Vector3
			   (a.y * b.z - a.z * b.y,
			   	b.x * a.z - a.x * b.z,
				a.x * b.y - b.x * a.y);
}

Vector3 Vector3::Forward()
{
	return Vector3(1, 0, 0);
}

Vector3 Vector3::Right()
{
	return Vector3(0, 1, 0);
}

Vector3 Vector3::Up()
{
	return Vector3(0, 0, 1);
}	
