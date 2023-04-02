#ifndef MATRIX_4X4_H
#define MATRIX_4X4_H

class Matrix4X4
{
public:
	Matrix4X4();
	Matrix4X4(float v11, float v12, float v13, float v14,
		   	float v21, float v22, float v23, float v24, 
			float v31, float v32, float v33, float v34,
			float v41, float v42, float v43, float v44);
	Matrix4X4(const Matrix4X4& m);
	Matrix4X4(Matrix4X4&& m);
	~Matrix4X4();

	Matrix4X4& operator= (const Matrix4X4& m);
	Matrix4X4& operator= (Matrix4X4&& m);

	friend Matrix4X4 operator+ (const Matrix4X4& a, const Matrix4X4& b);
	friend Matrix4X4 operator+ (Matrix4X4&& a, const Matrix4X4& b);

	friend Matrix4X4 operator* (float a, const Matrix4X4& m);
	friend Matrix4X4 operator* (float a, Matrix4X4&& m);
	friend Matrix4X4 operator* (const Matrix4X4& m, float a);
	friend Matrix4X4 operator* (Matrix4X4&& m, float a);

	friend Matrix4X4 operator/ (const Matrix4X4& m, float a);
	friend Matrix4X4 operator/ (Matrix4X4&& m, float a);

	friend Matrix4X4 operator* (const Matrix4X4& a, const Matrix4X4& b);
	friend Matrix4X4 operator* (Matrix4X4&& a, const Matrix4X4& b);

	static Matrix4X4 Identity();

	float m11, m12, m13, m14;
	float m21, m22, m23, m24;
	float m31, m32, m33, m34;
	float m41, m42, m43, m44;

};

#endif
