#include "Math.hpp"
#include <corecrt_math.h>


float NMath::Mod(const float a, const float b)
{
	return fmodf(a, b);
}

double NMath::Mod(const double a, const double b)
{
	return fmod(a, b);
}

float NMath::Sin(const float val)
{
	return sinf(val);
}

float NMath::Cos(const float val)
{
	return cosf(val);
}

float NMath::Tan(const float val)
{
	return tanf(val);
}

void NMath::SinCos(float val, float & sin_val, float & cos_val)
{
	sin_val = Sin(val);
	cos_val = Cos(val);
}

double NMath::Sin(const double val)
{
	return sin(val);
}

double NMath::Cos(const double val)
{
	return cos(val);
}

double NMath::Tan(const double val)
{
	return tan(val);
}

void NMath::SinCos(double val, double & sin_val, double & cos_val)
{
	sin_val = Sin(val);
	cos_val = Cos(val);
}

float NMath::ASin(const float val)
{
	return asinf(val);
}

float NMath::ACos(const float val)
{
	return acosf(val);
}

float NMath::ATan(const float val)
{
	return atanf(val);
}

float NMath::ATan2(const float y, const float x)
{
	return atan2f(y, x);
}

double NMath::ASin(const double val)
{
	return asin(val);
}

double NMath::ACos(const double val)
{
	return acos(val);
}

double NMath::ATan(const double val)
{
	return atan(val);
}

double NMath::ATan2(const double y, const double x)
{
	return atan2(y, x);
}

float NMath::Sqrt(const float val)
{
	return sqrtf(val);
}

float NMath::InvSqrt(const float val)
{
	return 1.f / sqrtf(val);
}

double NMath::Sqrt(const double val)
{
	return sqrt(val);
}

double NMath::InvSqrt(const double val)
{
	return 1.0 / sqrt(val);
}

float NMath::Exp(const float val)
{
	return expf(val);
}

float NMath::Ln(const float val)
{
	return logf(val);
}

float NMath::Pow(const float a, const float b)
{
	return powf(a, b);
}

double NMath::Exp(const double val)
{
	return exp(val);
}

double NMath::Ln(const double val)
{
	return log(val);
}

double NMath::Pow(const double a, const double b)
{
	return pow(a, b);
}

float NMath::Floor(const float val)
{
	return floorf(val);
}

float NMath::Ceil(const float val)
{
	return ceilf(val);
}

float NMath::Trunc(const float val)
{
	return truncf(val);
}

float NMath::Round(const float val)
{
	return roundf(val);
}

double NMath::Floor(const double val)
{
	return floor(val);
}

double NMath::Ceil(const double val)
{
	return ceil(val);
}

double NMath::Trunc(const double val)
{
	return trunc(val);
}

double NMath::Round(const double val)
{
	return round(val);
}

bool NMath::IsNormal(const float val)
{
	return isnormal(val);
}

bool NMath::IsFinite(const float val)
{
	return isfinite(val);
}

bool NMath::IsNan(const float val)
{
	return isnan(val);
}

bool NMath::IsInf(const float val)
{
	return isinf(val);
}

bool NMath::IsNormal(const double val)
{
	return isnormal(val);
}

bool NMath::IsFinite(const double val)
{
	return isfinite(val);
}

bool NMath::IsNan(const double val)
{
	return isnan(val);
}

bool NMath::IsInf(const double val)
{
	return isinf(val);
}

float NMath::Hypot(const float x, const float y)
{
	return hypotf(x, y);
}

double NMath::Hypot(const double x, const double y)
{
	return hypot(x, y);
}

float NMath::RadToDeg(float angle)
{
	return angle * 180.f / FLT_PI;
}

float NMath::DegToRad(float angle)
{
	return angle * FLT_PI / 180.f;
}

double NMath::RadToDeg(double angle)
{
	return angle * 180.0 / DBL_PI;
}

double NMath::DegToRad(double angle)
{
	return angle * DBL_PI / 180.0;
}
