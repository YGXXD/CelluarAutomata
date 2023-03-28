#pragma once

class Matrix4X4;
class Vector2;
class Vector3;
class Vector4;
namespace Math
{
	double Sin(double Rad);
	float Sin(float Rad);
	double Cos(double Rad);
	float Cos(float Rad);
	double Tan(double Rad);
	float Tan(float Rad);
	
	float InvSqrt(float a);

	template<typename T>
	inline int Round(T a);

	template<typename T>
	inline T Max(T a, T b);

	template <typename T>
	inline T Min(T a, T b);

	template<typename T>
	inline T Abs(T a);

	template<typename T>
	inline T Clamp(T Min, T Max, T Curr);
	
	template<typename T>
	inline T Linear(T a, T b, float Value);

	Vector2 Normalize(Vector2& V);
	Vector2 Normalize(Vector2&& V);
	float DotProduct(Vector2& V1, Vector2& V2);

	Vector3 Normalize(Vector3& V);
	Vector3 Normalize(Vector3&& V);
	float DotProduct(Vector3& V1, Vector3& V2);
	Vector3 CrossProduct(Vector3& V1, Vector3& V2);

	float MatrixDeterminant(Matrix4X4& Matrix); //行列式值
	Matrix4X4 MatrixAdjoint(Matrix4X4& Matrix); //伴随矩阵
	Matrix4X4 MatrixInverse(Matrix4X4& Matrix); //逆矩阵
	Matrix4X4 MatrixTransport(Matrix4X4& Matrix); //转置矩阵
	Matrix4X4 MatrixLookAt(Vector3& EyePosition, Vector3& FocusPosition, Vector3& UpDirection);
	Matrix4X4 MatrixPerspectiveFov(float FovAngle, float AspectRatio, float NearZ, float FarZ);
	Matrix4X4 MatrixTransition(Vector3& V); //平移矩阵
	Matrix4X4 MatrixScale(Vector3& V); //缩放矩阵
}

template<typename T>
inline int Math::Round(T a)
{
	return (int)(a > 0 ? (a + 0.5) : (a - 0.5));
}

template<typename T>
inline T Math::Max(T a, T b)
{
	return a > b ? a : b;
}

template<typename T>
inline T Math::Min(T a, T b)
{
	return a < b ? a : b;
}

template<typename T>
inline T Math::Abs(T a)
{
	return a > 0 ? a : -a;
}

template<typename T>
inline T Math::Clamp(T Min, T Max, T Curr)
{
	return Curr < Min ? Min : Curr > Max ? Max : Curr;
}

template<typename T>
inline T Math::Linear(T a, T b, float Value)
{
	return (b - a) * Clamp<float>(0.0, 1.0, Value) + a;
}
