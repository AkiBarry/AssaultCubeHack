#include "Colour.hpp"
#include "Math.hpp"

CColour CColour::Black(0, 0, 0);
CColour CColour::White(255, 255, 255);

CColour CColour::Red(255, 0, 0);
CColour CColour::Orange(255, 127, 0);
CColour CColour::Yellow(255, 255, 0);
CColour CColour::Green(0, 255, 0);
CColour CColour::Blue(0, 0, 255);
CColour CColour::Purple(127, 0, 127);

CColour CColour::Cyan(0, 255, 255);
CColour CColour::Magenta(255, 0, 255);

CColour::CColour(const uint32_t c)
{
	*reinterpret_cast<uint32_t *>(this) = c;
}

uint8_t & CColour::operator[](const int i)
{
	return reinterpret_cast<uint8_t *>(this)[i];
}

uint8_t CColour::operator[](const int i) const
{
	return reinterpret_cast<const uint8_t *>(this)[i];
}

uint8_t * CColour::Base()
{
	return reinterpret_cast<uint8_t *>(this);
}

uint8_t const * CColour::Base() const
{
	return reinterpret_cast<uint8_t const *>(this);
}

void CColour::CopyToArray(uint8_t* c) const
{
	*reinterpret_cast<uint32_t *>(c) = *reinterpret_cast<uint32_t const *>(this);
}

CColour & CColour::operator=(const CColour c)
{
	*reinterpret_cast<uint32_t *>(this) = *reinterpret_cast<const uint32_t *>(&c);

	return *this;
}

bool CColour::operator==(const CColour c) const
{
	return *reinterpret_cast<const uint32_t *>(this) == *reinterpret_cast<const uint32_t *>(&c);
}

bool CColour::operator!=(const CColour c) const
{
	return *reinterpret_cast<const uint32_t *>(this) != *reinterpret_cast<const uint32_t*>(&c);
}

uint32_t CColour::ToD3DColour() const
{
	return static_cast<uint32_t>(((a & 0xff) << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff));
}

CHSB CColour::ToHSB() const
{
	CHSB temp;
	const float _r = r / 255.0f;
	const float _g = g / 255.0f;
	const float _b = b / 255.0f;

	const float max = _r > _g ? _r : _g > _b ? _g : _b,
		min = _r < _g ? _r : _g < _b ? _g : _b;

	temp.b = (max + min) / 2.0f;

	if (max != min)
	{
		if (temp.b <= 0.5f)
			temp.s = (max - min) / (max + min);
		else
			temp.s = (max - min) / (2 - max - min);
	} else
		temp.s = 0;

	const float delta = max - min;
	float hue = 0.0f;

	if (_r == max)
		hue = (_g - _b) / delta;
	else if (_g == max)
		hue = 2 + (_b - _r) / delta;
	else if (_b == max)
		hue = 4 + (_r - _g) / delta;

	hue *= 60;

	if (hue < 0.0f)
		hue += 360.0f;

	temp.h = hue;

	return temp;
}

NMath::CVector<float, 4> CColour::ToVector() const
{
	return NMath::CVector<float, 4>(r/255.f, g/255.f, b/255.f);
}

uint32_t & CColour::AsRawColour()
{
	return *reinterpret_cast<uint32_t *>(this);
}

const uint32_t & CColour::AsRawColour() const
{
	return *reinterpret_cast<const uint32_t *>(this);
}

CColour CColour::WithAlpha(int alpha)
{
	CColour temp = *this;
	temp.a = alpha;

	return temp;
}

float CColour::Hue() const
{
	if (r == g && g == b)
		return 0.0f;

	float _r = r / 255.0f;
	float _g = g / 255.0f;
	float _b = b / 255.0f;

	float max = _r > _g ? _r : _g > _b ? _g : _b,
		min = _r < _g ? _r : _g < _b ? _g : _b;
	float delta = max - min;
	float hue = 0.0f;

	if (_r == max)
	{
		hue = (_g - _b) / delta;
	} else if (_g == max)
	{
		hue = 2 + (_b - _r) / delta;
	} else if (_b == max)
	{
		hue = 4 + (_r - _g) / delta;
	}
	hue *= 60;

	if (hue < 0.0f)
	{
		hue += 360.0f;
	}
	return hue;
}

float CColour::Saturation() const
{
	float _r = r / 255.0f;
	float _g = g / 255.0f;
	float _b = b / 255.0f;

	float max = _r > _g ? _r : _g > _b ? _g : _b,
		min = _r < _g ? _r : _g < _b ? _g : _b;
	float l, s = 0;

	if (max != min)
	{
		l = (max + min) / 2;
		if (l <= 0.5f)
			s = (max - min) / (max + min);
		else
			s = (max - min) / (2 - max - min);
	}
	return s;
}

float CColour::Brightness() const
{
	float _r = r / 255.0f;
	float _g = g / 255.0f;
	float _b = b / 255.0f;

	float max = _r > _g ? _r : _g > _b ? _g : _b,
		min = _r < _g ? _r : _g < _b ? _g : _b;
	return (max + min) / 2.0f;
}

float & CHSB::operator[](const int i)
{
	return reinterpret_cast<float *>(this)[i];
}

float CHSB::operator[](const int i) const
{
	return reinterpret_cast<const float *>(this)[i];
}

float * CHSB::Base()
{
	return reinterpret_cast<float*>(this);
}

float const * CHSB::Base() const
{
	return reinterpret_cast<float const*>(this);
}

void CHSB::CopyToArray(float* f) const
{
	f[0] = h;
	f[1] = s;
	f[2] = b;
}

CHSB & CHSB::operator=(const CHSB hsb)
{
	h = hsb.h;
	s = hsb.s;
	b = hsb.b;

	return *this;
}

bool CHSB::operator==(const CHSB hsb) const
{
	return h == hsb.h
		&& s == hsb.s
		&& b == hsb.b;
}

bool CHSB::operator!=(const CHSB hsb) const
{
	return h != hsb.h
		|| s != hsb.s
		|| b != hsb.b;
}

CColour CHSB::ToColour() const
{
	float hue = h == 1.0f ? 0.0f : h * 6.0f;
	float f = hue - static_cast<int>(hue);
	float _r = b * 255.0f;
	uint8_t r = static_cast<uint8_t>(_r);
	uint8_t p = static_cast<uint8_t>(_r * (1.0f - s));
	uint8_t q = static_cast<uint8_t>(_r * (1.0f - s * f));
	uint8_t t = static_cast<uint8_t>(_r * (1.0f - (s * (1.0f - f))));

	if (hue < 1.0f)
		return CColour(r, t, p);

	if (hue < 2.0f)
		return CColour(q, r, p);

	if (hue < 3.0f)
		return CColour(p, r, t);

	if (hue < 4.0f)
		return CColour(p, q, r);

	if (hue < 5.0f)
		return CColour(t, p, r);

	return CColour(r, p, q);
}
