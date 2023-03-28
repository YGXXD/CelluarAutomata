#include "MathLibrary.h"
#include "Matrix4X4.h"
#include "Vector2.h"
#include "Vector3.h"
#include <cmath>

double Math::Sin(double Rad)
{
	return sin(Rad);	
}

float Math::Sin(float Rad)
{
	return sin(Rad);
}

double Math::Cos(double Rad)
{
	return cos(Rad);
}

float Math::Cos(float Rad)
{
	return cos(Rad);
}

double Math::Tan(double Rad)
{
	return tan(Rad);
}

float Math::Tan(float Rad)
{
	return tan(Rad);
}

float Math::InvSqrt(float a)
{
	uint32_t i;
	float x = a;
	
	//将浮点数转化为32位整数
	i = *(uint32_t*)&x;
	//取出IEEE754指数和尾数,原算法u的值为0.0450465
	i = 0x5f3759df - (i >> 1);
	//求出平方根倒数近似解
	x = *(float*)&i; 
	
	//用牛顿迭代法进行迭代增加精度
	x = x * (1.5f - (0.5f * a * x * x));
	//x = x * (1.5f - (0.5f * a * x * x));

	return x;
}

Vector2 Math::Normalize(Vector2& V)
{
	return InvSqrt(V.x * V.x + V.y * V.y) * V; 
}

Vector2 Math::Normalize(Vector2&& V)
{
	return InvSqrt(V.x * V.x + V.y * V.y) * V; 
}

float Math::DotProduct(Vector2& V1, Vector2& V2)
{
	return V1 * V2; 
}

Vector3 Math::Normalize(Vector3& V)
{
	return InvSqrt(V.x * V.x + V.y * V.y + V.z * V.z) * V; 
}

Vector3 Math::Normalize(Vector3&& V)
{
	return InvSqrt(V.x * V.x + V.y * V.y + V.z * V.z) * V; 
}

float Math::DotProduct(Vector3& V1, Vector3& V2)
{
	return V1 * V2; 
}

Vector3 Math::CrossProduct(Vector3& V1, Vector3& V2)
{
	return V1 ^ V2;
}

float Math::MatrixDeterminant(Matrix4X4& Matrix)
{
	return Matrix.m11 * Matrix.m22 * Matrix.m33 * Matrix.m44 +
           Matrix.m11 * Matrix.m23 * Matrix.m34 * Matrix.m42 +
           Matrix.m11 * Matrix.m24 * Matrix.m32 * Matrix.m43 -
           Matrix.m11 * Matrix.m24 * Matrix.m33 * Matrix.m42 -
           Matrix.m11 * Matrix.m23 * Matrix.m32 * Matrix.m44 -
           Matrix.m11 * Matrix.m22 * Matrix.m34 * Matrix.m43 +
		   Matrix.m12 * Matrix.m24 * Matrix.m33 * Matrix.m41 +
           Matrix.m12 * Matrix.m23 * Matrix.m31 * Matrix.m44 +
           Matrix.m12 * Matrix.m21 * Matrix.m34 * Matrix.m43 -
           Matrix.m12 * Matrix.m21 * Matrix.m33 * Matrix.m44 -
           Matrix.m12 * Matrix.m23 * Matrix.m34 * Matrix.m41 -
           Matrix.m12 * Matrix.m24 * Matrix.m31 * Matrix.m43 +
		   Matrix.m13 * Matrix.m21 * Matrix.m32 * Matrix.m44 +
	       Matrix.m13 * Matrix.m22 * Matrix.m34 * Matrix.m41 +
		   Matrix.m13 *	Matrix.m24 * Matrix.m31 * Matrix.m42 -
		   Matrix.m13 * Matrix.m24 * Matrix.m32 * Matrix.m41 - 
		   Matrix.m13 *	Matrix.m22 * Matrix.m31 * Matrix.m44 -
		   Matrix.m13 *	Matrix.m21 * Matrix.m34 * Matrix.m42 +
	       Matrix.m14 * Matrix.m23 * Matrix.m32 * Matrix.m41 +
		   Matrix.m14 *	Matrix.m22 * Matrix.m31 * Matrix.m43 +
		   Matrix.m14 * Matrix.m21 * Matrix.m33 * Matrix.m42 -
		   Matrix.m14 * Matrix.m21 * Matrix.m32 * Matrix.m43 -
		   Matrix.m14 * Matrix.m22 * Matrix.m33 * Matrix.m41 -
		   Matrix.m14 * Matrix.m23 * Matrix.m31 * Matrix.m42;


}

Matrix4X4 Math::MatrixAdjoint(Matrix4X4& Matrix)
{
	float M11 = Matrix.m22 * Matrix.m33 * Matrix.m44 + 
				Matrix.m23 * Matrix.m34 * Matrix.m42 +
				Matrix.m24 * Matrix.m32 * Matrix.m43 - 
				Matrix.m24 * Matrix.m33 * Matrix.m42 -
				Matrix.m23 * Matrix.m32 * Matrix.m44 - 
				Matrix.m22 * Matrix.m34 * Matrix.m43;

	float M12 = Matrix.m24 * Matrix.m33 * Matrix.m41 +
				Matrix.m23 * Matrix.m31 * Matrix.m44 +
				Matrix.m21 * Matrix.m34 * Matrix.m43 -
				Matrix.m21 * Matrix.m33 * Matrix.m44 -
				Matrix.m23 * Matrix.m34 * Matrix.m41 -
				Matrix.m24 * Matrix.m31 * Matrix.m43;

	float M13 = Matrix.m21 * Matrix.m32 * Matrix.m44 +
				Matrix.m22 * Matrix.m34 * Matrix.m41 +
				Matrix.m24 * Matrix.m31 * Matrix.m42 -
				Matrix.m24 * Matrix.m32 * Matrix.m41 -
				Matrix.m22 * Matrix.m31 * Matrix.m44 -
				Matrix.m21 * Matrix.m34 * Matrix.m42;

	float M14 = Matrix.m23 * Matrix.m32 * Matrix.m41 +
				Matrix.m22 * Matrix.m31 * Matrix.m43 +
				Matrix.m21 * Matrix.m33 * Matrix.m42 -
				Matrix.m21 * Matrix.m32 * Matrix.m43 -
				Matrix.m22 * Matrix.m33 * Matrix.m41 -
				Matrix.m23 * Matrix.m31 * Matrix.m42;

	float M21 = Matrix.m14 * Matrix.m33 * Matrix.m42 +
				Matrix.m13 * Matrix.m32 * Matrix.m44 +
				Matrix.m12 * Matrix.m34 * Matrix.m43 -
				Matrix.m12 * Matrix.m33 * Matrix.m44 -
				Matrix.m13 * Matrix.m34 * Matrix.m42 -
				Matrix.m14 * Matrix.m32 * Matrix.m43;

	float M22 = Matrix.m11 * Matrix.m33 * Matrix.m44 +
				Matrix.m13 * Matrix.m34 * Matrix.m41 +
				Matrix.m14 * Matrix.m31 * Matrix.m43 -
				Matrix.m14 * Matrix.m33 * Matrix.m41 -
				Matrix.m13 * Matrix.m31 * Matrix.m44 -
				Matrix.m11 * Matrix.m34 * Matrix.m43;

	float M23 = Matrix.m14 * Matrix.m32 * Matrix.m41 +
				Matrix.m12 * Matrix.m31 * Matrix.m44 +
				Matrix.m11 * Matrix.m34 * Matrix.m42 - 
				Matrix.m11 * Matrix.m32 * Matrix.m44 -
				Matrix.m12 * Matrix.m34 * Matrix.m41 -
				Matrix.m14 * Matrix.m31 * Matrix.m42;

	float M24 = Matrix.m11 * Matrix.m32 * Matrix.m43 +
				Matrix.m12 * Matrix.m33 * Matrix.m41 +
				Matrix.m13 * Matrix.m31 * Matrix.m42 -
				Matrix.m13 * Matrix.m32 * Matrix.m41 -
				Matrix.m12 * Matrix.m31 * Matrix.m43 -
				Matrix.m11 * Matrix.m33 * Matrix.m42;

	float M31 = Matrix.m12 * Matrix.m23 * Matrix.m44 + 
				Matrix.m13 * Matrix.m24 * Matrix.m42 + 
				Matrix.m14 * Matrix.m22 * Matrix.m43 -
				Matrix.m14 * Matrix.m23 * Matrix.m42 -
				Matrix.m13 * Matrix.m22 * Matrix.m44 -
				Matrix.m12 * Matrix.m24 * Matrix.m43;

	float M32 = Matrix.m14 * Matrix.m23 * Matrix.m41 +
				Matrix.m13 * Matrix.m21 * Matrix.m44 +
				Matrix.m11 * Matrix.m24 * Matrix.m43 -
				Matrix.m11 * Matrix.m23 * Matrix.m44 -
				Matrix.m13 * Matrix.m24 * Matrix.m41 -
				Matrix.m14 * Matrix.m21 * Matrix.m43;

	float M33 = Matrix.m11 * Matrix.m22 * Matrix.m44 +
				Matrix.m12 * Matrix.m24 * Matrix.m41 +
				Matrix.m14 * Matrix.m21 * Matrix.m42 -
				Matrix.m14 * Matrix.m22 * Matrix.m41 -
				Matrix.m12 * Matrix.m21 * Matrix.m44 -
				Matrix.m11 * Matrix.m24 * Matrix.m42;

	float M34 = Matrix.m13 * Matrix.m22 * Matrix.m41 +
				Matrix.m12 * Matrix.m21 * Matrix.m43 +
				Matrix.m11 * Matrix.m23 * Matrix.m42 -
				Matrix.m11 * Matrix.m22 * Matrix.m43 -
				Matrix.m12 * Matrix.m23 * Matrix.m41 -
				Matrix.m13 * Matrix.m21 * Matrix.m42;

	float M41 = Matrix.m14 * Matrix.m23 * Matrix.m32 +
				Matrix.m13 * Matrix.m22 * Matrix.m34 +
				Matrix.m12 * Matrix.m24 * Matrix.m33 -
				Matrix.m12 * Matrix.m23 * Matrix.m34 -
				Matrix.m13 * Matrix.m24 * Matrix.m32 -
				Matrix.m14 * Matrix.m22 * Matrix.m33;

	float M42 = Matrix.m11 * Matrix.m23 * Matrix.m34 +
				Matrix.m13 * Matrix.m24 * Matrix.m31 +
				Matrix.m14 * Matrix.m21 * Matrix.m33 -
				Matrix.m14 * Matrix.m23 * Matrix.m31 -
				Matrix.m13 * Matrix.m21 * Matrix.m34 -
				Matrix.m11 * Matrix.m24 * Matrix.m33;

	float M43 = Matrix.m14 * Matrix.m22 * Matrix.m31 +
				Matrix.m12 * Matrix.m21 * Matrix.m34 +
				Matrix.m11 * Matrix.m24 * Matrix.m32 -
				Matrix.m11 * Matrix.m22 * Matrix.m34 -
				Matrix.m12 * Matrix.m24 * Matrix.m31 -
				Matrix.m14 * Matrix.m21 * Matrix.m32;

	float M44 = Matrix.m11 * Matrix.m22 * Matrix.m33 +
				Matrix.m12 * Matrix.m23 * Matrix.m31 +
				Matrix.m13 * Matrix.m21 * Matrix.m32 -
				Matrix.m13 * Matrix.m22 * Matrix.m31 -
				Matrix.m12 * Matrix.m21 * Matrix.m33 - 
				Matrix.m11 * Matrix.m23 * Matrix.m32;

	return Matrix4X4
				(M11, M21, M31, M41,
				 M12, M22, M32, M42,
				 M13, M23, M33, M43,
				 M14, M24, M34, M44);
}

Matrix4X4 Math::MatrixInverse(Matrix4X4& Matrix)
{
	return 1 / MatrixDeterminant(Matrix) * MatrixAdjoint(Matrix);
}

Matrix4X4 Math::MatrixTransport(Matrix4X4& Matrix)
{
	return Matrix4X4
				(Matrix.m11, Matrix.m21, Matrix.m31, Matrix.m41,
				 Matrix.m12, Matrix.m22, Matrix.m32, Matrix.m42,
				 Matrix.m13, Matrix.m23, Matrix.m33, Matrix.m43,
				 Matrix.m14, Matrix.m24, Matrix.m34, Matrix.m44);
}

Matrix4X4 Math::MatrixLookAt(Vector3& EyePosition, Vector3& FocusPosition, Vector3& UpDirection)
{
	Vector3 Z = Normalize(FocusPosition - EyePosition);
	Vector3 X = Normalize(UpDirection ^ Z);
	Vector3 Y = Z ^ X;

	return Matrix4X4
				(X.x, Y.x, Z.x, 0,
				 X.y ,Y.y, Z.y, 0,
				 X.z, Y.z, Z.z, 0,
				 - (EyePosition * X), - (EyePosition * Y), - (EyePosition * Z), 1);
}

Matrix4X4 Math::MatrixPerspectiveFov(float FovAngle, float AspectRatio, float NearZ, float FarZ)
{
	float HalfAngleTan = Tan(0.5f * FovAngle);
	return Matrix4X4
				(1 / (AspectRatio * HalfAngleTan), 0, 0, 0,
				 0, 1 / HalfAngleTan, 0, 0,
				 0, 0, FarZ / (FarZ - NearZ), 1,
				 0, 0, (- NearZ * FarZ) / FarZ - NearZ, 0);
}

Matrix4X4 Math::MatrixTransition(Vector3& V)
{
	return Matrix4X4
				(1, 0, 0, 0,
				 0, 1, 0, 0,
				 0, 0, 1, 0,
				 V.x, V.y, V.z, 1);
}

Matrix4X4 Math::MatrixScale(Vector3& V)
{
	return Matrix4X4
				(V.x, 0, 0, 0,
				 0, V.y, 0, 0,
				 0, 0, V.z, 0,
				 0, 0, 0, 1);	
}
