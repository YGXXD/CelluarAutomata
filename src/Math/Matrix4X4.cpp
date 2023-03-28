#include "Matrix4X4.h"

Matrix4X4::Matrix4X4()
	:m11(0),m12(0),m13(0),m14(0),
	 m21(0),m22(0),m23(0),m24(0),
	 m31(0),m32(0),m33(0),m34(0),
	 m41(0),m42(0),m43(0),m44(0)
{

}

Matrix4X4::Matrix4X4
	(float v11, float v12, float v13, float v14,
	float v21, float v22, float v23, float v24, 
	float v31, float v32, float v33, float v34,
	float v41, float v42, float v43, float v44)
	:m11(v11),m12(v12),m13(v13),m14(v14),
	 m21(v21),m22(v22),m23(v23),m24(v24),
	 m31(v31),m32(v32),m33(v33),m34(v34),
	 m41(v41),m42(v42),m43(v43),m44(v44)
{

}

Matrix4X4::Matrix4X4(const Matrix4X4& m)
	:m11(m.m11),m12(m.m12),m13(m.m13),m14(m.m14),
	 m21(m.m21),m22(m.m22),m23(m.m23),m24(m.m24),
	 m31(m.m31),m32(m.m32),m33(m.m33),m34(m.m34),
	 m41(m.m41),m42(m.m42),m43(m.m43),m44(m.m44)
{

}

Matrix4X4::Matrix4X4(Matrix4X4&& m)
	:m11(m.m11),m12(m.m12),m13(m.m13),m14(m.m14),
	 m21(m.m21),m22(m.m22),m23(m.m23),m24(m.m24),
	 m31(m.m31),m32(m.m32),m33(m.m33),m34(m.m34),
	 m41(m.m41),m42(m.m42),m43(m.m43),m44(m.m44)
{

}

Matrix4X4::~Matrix4X4()
{

}

Matrix4X4& Matrix4X4::operator=(const Matrix4X4& m)
{
	m11 = m.m11; m12 = m.m12; m13 = m.m13; m14 = m.m14;
	m21 = m.m21; m22 = m.m22; m23 = m.m23; m24 = m.m24;
	m31 = m.m31; m32 = m.m32; m33 = m.m33; m34 = m.m34;
	m41 = m.m41; m42 = m.m42; m43 = m.m43, m44 = m.m44;

	return *this;
}

Matrix4X4& Matrix4X4::operator=(Matrix4X4&& m)
{
	m11 = m.m11; m12 = m.m12; m13 = m.m13; m14 = m.m14;
	m21 = m.m21; m22 = m.m22; m23 = m.m23; m24 = m.m24;
	m31 = m.m31; m32 = m.m32; m33 = m.m33; m34 = m.m34;
	m41 = m.m41; m42 = m.m42; m43 = m.m43, m44 = m.m44;

	return *this;
}

Matrix4X4 operator+ (const Matrix4X4& a, const Matrix4X4& b)
{
	return Matrix4X4
			   (a.m11 + b.m11, a.m12 + b.m12, a.m13 + b.m13, a.m14 + b.m14,
				a.m21 + b.m21, a.m22 + b.m22, a.m23 + b.m23, a.m24 + b.m24,
				a.m31 + b.m31, a.m32 + b.m32, a.m33 + b.m33, a.m34 + b.m34,
				a.m41 + b.m41, a.m42 + b.m42, a.m43 + b.m43, a.m44 + b.m44);
}

Matrix4X4 operator+ (Matrix4X4&& a, const Matrix4X4& b)
{
	return Matrix4X4
			   (a.m11 + b.m11, a.m12 + b.m12, a.m13 + b.m13, a.m14 + b.m14,
				a.m21 + b.m21, a.m22 + b.m22, a.m23 + b.m23, a.m24 + b.m24,
				a.m31 + b.m31, a.m32 + b.m32, a.m33 + b.m33, a.m34 + b.m34,
				a.m41 + b.m41, a.m42 + b.m42, a.m43 + b.m43, a.m44 + b.m44);
}

Matrix4X4 operator* (float a, const Matrix4X4& m)
{
	return Matrix4X4
				(a * m.m11, a * m.m12, a * m.m13, a * m.m14,
				 a * m.m21, a * m.m22, a * m.m23, a * m.m24,
				 a * m.m31, a * m.m32, a * m.m33, a * m.m34,
				 a * m.m41, a * m.m42, a * m.m43, a * m.m44);
}

Matrix4X4 operator* (float a, Matrix4X4&& m)
{
	return Matrix4X4
				(a * m.m11, a * m.m12, a * m.m13, a * m.m14,
				 a * m.m21, a * m.m22, a * m.m23, a * m.m24,
				 a * m.m31, a * m.m32, a * m.m33, a * m.m34,
				 a * m.m41, a * m.m42, a * m.m43, a * m.m44);
}

Matrix4X4 operator* (const Matrix4X4& m, float a)
{
	return Matrix4X4
				(m.m11 * a, m.m12 * a, m.m13 * a, m.m14 * a,
				 m.m21 * a, m.m22 * a, m.m23 * a, m.m24 * a,
				 m.m31 * a, m.m32 * a, m.m33 * a, m.m34 * a,
				 m.m41 * a, m.m42 * a, m.m43 * a, m.m44 * a);
}

Matrix4X4 operator* (Matrix4X4&& m, float a)
{
	return Matrix4X4
				(m.m11 * a, m.m12 * a, m.m13 * a, m.m14 * a,
				 m.m21 * a, m.m22 * a, m.m23 * a, m.m24 * a,
				 m.m31 * a, m.m32 * a, m.m33 * a, m.m34 * a,
				 m.m41 * a, m.m42 * a, m.m43 * a, m.m44 * a);
}

Matrix4X4 operator/ (const Matrix4X4& m, float a)
{
	return Matrix4X4
				(m.m11 / a, m.m12 / a, m.m13 / a, m.m14 / a,
				 m.m21 / a, m.m22 / a, m.m23 / a, m.m24 / a,
				 m.m31 / a, m.m32 / a, m.m33 / a, m.m34 / a,
				 m.m41 / a, m.m42 / a, m.m43 / a, m.m44 / a);
}

Matrix4X4 operator/ (Matrix4X4&& m, float a)
{
	return Matrix4X4
				(m.m11 / a, m.m12 / a, m.m13 / a, m.m14 / a,
				 m.m21 / a, m.m22 / a, m.m23 / a, m.m24 / a,
				 m.m31 / a, m.m32 / a, m.m33 / a, m.m34 / a,
				 m.m41 / a, m.m42 / a, m.m43 / a, m.m44 / a);
}

Matrix4X4 operator* (const Matrix4X4& a, const Matrix4X4& b)
{
	return Matrix4X4
				(a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31 + a.m14 * b.m41,
				 a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32 + a.m14 * b.m42,
				 a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33 + a.m14 * b.m43,
				 a.m11 * b.m14 + a.m12 * b.m24 + a.m13 * b.m34 + a.m14 * b.m44,
				 a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31 + a.m24 * b.m41,
				 a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32 + a.m24 * b.m42,
				 a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33 + a.m24 * b.m43,
				 a.m21 * b.m14 + a.m22 * b.m24 + a.m23 * b.m34 + a.m24 * b.m44,
				 a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31 + a.m34 * b.m41,
				 a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32 + a.m34 * b.m42,
				 a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33 + a.m34 * b.m43,
				 a.m31 * b.m14 + a.m32 * b.m24 + a.m33 * b.m34 + a.m34 * b.m44,
				 a.m41 * b.m11 + a.m42 * b.m21 + a.m43 * b.m31 + a.m44 * b.m41,
				 a.m41 * b.m12 + a.m42 * b.m22 + a.m43 * b.m32 + a.m44 * b.m42,
				 a.m41 * b.m13 + a.m42 * b.m23 + a.m43 * b.m33 + a.m44 * b.m43,
				 a.m41 * b.m14 + a.m42 * b.m24 + a.m43 * b.m34 + a.m44 * b.m44);
}

Matrix4X4 operator* (Matrix4X4&& a, const Matrix4X4& b)
{
	return Matrix4X4
				(a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31 + a.m14 * b.m41,
				 a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32 + a.m14 * b.m42,
				 a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33 + a.m14 * b.m43,
				 a.m11 * b.m14 + a.m12 * b.m24 + a.m13 * b.m34 + a.m14 * b.m44,
				 a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31 + a.m24 * b.m41,
				 a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32 + a.m24 * b.m42,
				 a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33 + a.m24 * b.m43,
				 a.m21 * b.m14 + a.m22 * b.m24 + a.m23 * b.m34 + a.m24 * b.m44,
				 a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31 + a.m34 * b.m41,
				 a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32 + a.m34 * b.m42,
				 a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33 + a.m34 * b.m43,
				 a.m31 * b.m14 + a.m32 * b.m24 + a.m33 * b.m34 + a.m34 * b.m44,
				 a.m41 * b.m11 + a.m42 * b.m21 + a.m43 * b.m31 + a.m44 * b.m41,
				 a.m41 * b.m12 + a.m42 * b.m22 + a.m43 * b.m32 + a.m44 * b.m42,
				 a.m41 * b.m13 + a.m42 * b.m23 + a.m43 * b.m33 + a.m44 * b.m43,
				 a.m41 * b.m14 + a.m42 * b.m24 + a.m43 * b.m34 + a.m44 * b.m44);
}

Matrix4X4 Matrix4X4::Identity()
{
	return Matrix4X4
				(1, 0, 0, 0,
				 0, 1, 0, 0,
				 0, 0, 1, 0,
				 0, 0, 0, 1);
}
