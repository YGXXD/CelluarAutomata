#include "Vector4.h"
#include "Vector3.h"
#include "Matrix4X4.h"

Vector4::Vector4():x(0),y(0),z(0),w(0)
{

}

Vector4::Vector4(float nx, float ny, float nz, float nw):x(nx),y(ny),z(nz),w(nw)
{

}

Vector4::Vector4(const Vector3& vector, float nw):x(vector.x),y(vector.y),z(vector.z),w(nw)
{

}

Vector4::Vector4(Vector3&& vector, float nw):x(vector.x),y(vector.y),z(vector.z),w(nw)
{

}

Vector4::Vector4(const Vector4& vector):x(vector.x),y(vector.y),z(vector.z)
{
}

Vector4::Vector4(Vector4&& vector):x(vector.x),y(vector.y),z(vector.z)
{
}

Vector4::~Vector4()
{

}

Vector4& Vector4::operator= (const Vector4& a)
{
	x = a.x;
	y = a.y;
	z = a.z;
	w = a.w;
	return *this;
}

Vector4& Vector4::operator= (Vector4&& a)
{
	x = a.x;
	y = a.y;
	z = a.z;
	w = a.w;
	return *this;
}

Vector4& Vector4::operator+= (const Vector4& a)
{
	x += a.x;
	y += a.y;
	z += a.z;
	w += a.w;
	return *this;
}

Vector4& Vector4::operator+= (Vector4&& a)
{
	x += a.x;
	y += a.y;
	z += a.z;
	w += a.w;
	return *this;
}

Vector4& Vector4::operator-= (const Vector4& a)
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	w -= a.w;
	return *this;
}

Vector4& Vector4::operator-= (Vector4&& a)
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	w -= a.w;
	return *this;
}

Vector4& Vector4::operator*= (float a)
{
	x *= a;
	y *= a;
	z *= a;
	w *= a;
	return *this;
}

Vector4& Vector4::operator/= (float a)
{
	x /= a;
	y /= a;
	z /= a;
	w /= a;
	return *this;
}

Vector4 operator+ (const Vector4& a, const Vector4& b)
{
	return Vector4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

Vector4 operator+ (Vector4&& a, const Vector4& b)
{
	return Vector4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

Vector4 operator- (const Vector4& a, const Vector4& b)
{
	return Vector4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

Vector4 operator- (Vector4&& a, const Vector4& b)
{
	return Vector4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

Vector4 operator* (float a, const Vector4& b)
{
	return Vector4(a * b.x, a * b.y, a * b.z, a * b.w);
}

Vector4 operator* (float  a, Vector4&& b)
{
	return Vector4(a * b.x, a * b.y, a * b.z, a * b.w);
}

Vector4 operator* (const Vector4& a, float b)
{
	return Vector4(a.x * b, a.y * b, a.z * b, a.w * b);
}

Vector4 operator* (Vector4&& a, float b)
{
	return Vector4(a.x * b, a.y * b, a.z * b, a.w * b);
}

Vector4 operator/ (const Vector4& a, float b)
{
	return Vector4(a.x / b, a.y / b, a.z / b, a.w / b);
}

Vector4 operator/ (Vector4&& a, float b)
{
	return Vector4(a.x / b, a.y / b, a.z / b, a.w / b);
}

Vector4 operator* (const Vector4& a, const Matrix4X4& m)
{
	return Vector4
				(a.x * m.m11 + a.y * m.m21 + a.z * m.m31 + a.w * m.m41,
				 a.x * m.m12 + a.y * m.m22 + a.z * m.m32 + a.w * m.m42,
				 a.x * m.m13 + a.y * m.m23 + a.z * m.m33 + a.w * m.m43,
				 a.x * m.m14 + a.y * m.m24 + a.z * m.m34 + a.w * m.m44);
}

Vector4 operator* (Vector4&& a, const Matrix4X4& m)
{
	return Vector4
				(a.x * m.m11 + a.y * m.m21 + a.z * m.m31 + a.w * m.m41,
				 a.x * m.m12 + a.y * m.m22 + a.z * m.m32 + a.w * m.m42,
				 a.x * m.m13 + a.y * m.m23 + a.z * m.m33 + a.w * m.m43,
				 a.x * m.m14 + a.y * m.m24 + a.z * m.m34 + a.w * m.m44);
}
