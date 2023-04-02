#ifndef VECTOR_4_H
#define VECTOR_4_H

class Vector3;
class Matrix4X4;
class Vector4
{
public:
	Vector4();
	Vector4(float nx, float ny, float nz, float nw);
	Vector4(const Vector3& vector, float nw);
	Vector4(Vector3&& vector, float nw);
	Vector4(const Vector4& vector);
	Vector4(Vector4&& vector);
	~Vector4();

	Vector4& operator= (const Vector4& a);
	Vector4& operator= (Vector4&& a);
	Vector4& operator+= (const Vector4& a);
	Vector4& operator+= (Vector4&& a);
	Vector4& operator-= (const Vector4& a);
	Vector4& operator-= (Vector4&& a);
	Vector4& operator*= (float a);
	Vector4& operator/= (float a);

	friend Vector4 operator+ (const Vector4& a, const Vector4& b);
	friend Vector4 operator+ (Vector4&& a, const Vector4& b);
	friend Vector4 operator- (const Vector4& a, const Vector4& b);
 	friend Vector4 operator- (Vector4&& a, const Vector4& b);
	
	friend Vector4 operator* (float a, const Vector4& b);
	friend Vector4 operator* (float a, Vector4&& b);
	friend Vector4 operator* (const Vector4& a, float b);
	friend Vector4 operator* (Vector4&& a, float b);

	friend Vector4 operator/ (const Vector4& a, float b);
	friend Vector4 operator/ (Vector4&& a, float b);

	friend Vector4 operator* (const Vector4& a, const Matrix4X4& m);
	friend Vector4 operator* (Vector4&& a, const Matrix4X4& m);

	float x;
	float y;
	float z;
	float w;
};

#endif
