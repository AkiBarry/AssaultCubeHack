#pragma once

namespace NMath
{
	constexpr float		FLT_PI =		3.141592653f;
	constexpr float		FLT_INV_PI =	1.f / FLT_PI;
	constexpr float		FLT_E =			2.718281828f;
		
	constexpr double	DBL_PI =		3.141592653589793;
	constexpr double	DBL_INV_PI =	1.0 / DBL_PI;
	constexpr double	DBL_E =		2.718281828459045;
		
	constexpr float		FLT_RAD2DEG =	180.f / FLT_PI;
	constexpr float		FLT_DEG2RAD =	FLT_PI / 180.f;
		
	constexpr double	DBL_RAD2DEG = 180.f / DBL_PI;
	constexpr double	DBL_DEG2RAD = DBL_PI / 180.f;

	template <typename T, size_t dim>
	class CVector;

	using CVec2f = CVector<float, 2>;
	using CVec3f = CVector<float, 3>;
	using CVec4f = CVector<float, 4>;

	using CVec2d = CVector<double, 2>;
	using CVec3d = CVector<double, 3>;
	using CVec4d = CVector<double, 4>;

	using CVec2i = CVector<int, 2>;
	using CVec3i = CVector<int, 3>;
	using CVec4i = CVector<int, 4>;

	template <typename T, size_t dim, bool radian = true>
	class CAngle;

	template <typename T, size_t rows, size_t columns>
	class CMatrix;

	template <typename T>
	class CRandom;

	template <class T>
	T Sign(T val);

	template <class T>
	T Abs(T val);

	template <class T>
	T Min(T a, T b);

	template <class T>
	T Max(T a, T b);

	template <class T>
	T Clamp(T x, T min, T max);

	template <typename T>
	T Lerp(T min, T max, T factor);

	float Mod(float a, float b);
	double Mod(double a, double b);

	float Sin(float val);
	float Cos(float val);
	float Tan(float val);

	void SinCos(float val, float & sin_val, float & cos_val);

	double Sin(double val);
	double Cos(double val);
	double Tan(double val);

	void SinCos(double val, double & sin_val, double & cos_val);

	float ASin(float val);
	float ACos(float val);
	float ATan(float val);
	float ATan2(float y, float x);

	double ASin(double val);
	double ACos(double val);
	double ATan(double val);
	double ATan2(double y, double x);

	float Sqrt(float val);
	float InvSqrt(float val);

	double Sqrt(double val);
	double InvSqrt(double val);

	float Exp(float val);
	float Ln(float val);
	float Pow(float a, float b);

	double Exp(double val);
	double Ln(double val);
	double Pow(double a, double b);

	float Floor(float val);
	float Ceil(float val);
	float Trunc(float val);
	float Round(float val);

	double Floor(double val);
	double Ceil(double val);
	double Trunc(double val);
	double Round(double val);

	bool IsNormal(float val);
	bool IsFinite(float val);
	bool IsNan(float val);
	bool IsInf(float val);

	bool IsNormal(double val);
	bool IsFinite(double val);
	bool IsNan(double val);
	bool IsInf(double val);

	float Hypot(float x, float y);
	double Hypot(double x, double y);

	float RadToDeg(float angle);
	float DegToRad(float angle);

	double RadToDeg(double angle);
	double DegToRad(double angle);
}

template <class T>
T NMath::Sign(const T t)
{
	if (!t)
	{
		return 0;
	}
	return t > T(0) ? T(1) : T(-1);
}

template <class T>
T NMath::Abs(const T val)
{
	return val < 0 ? -val : val;
}

template <class T>
T NMath::Min(const T a, const T b)
{
	return a < b ? a : b;
}

template <class T>
T NMath::Max(const T a, const T b)
{
	return a > b ? a : b;
}

template <class T>
T NMath::Clamp(const T x, const T min, const T max)
{
	return x > max ? max : x < min ? min : x;
}

template<typename T>
T NMath::Lerp(T min, T max, T factor)
{
	return min + (max - min) * factor;
}