#pragma once
#include <cmath>

namespace Math
{
	constexpr float PI = 3.14159265f;
	constexpr double PI_D = 3.1415926535897932;

	template <typename T>
	constexpr auto sq(const T& x)
	{
		return x * x;
	}

	template<typename T>
	T wrap_angle(T theta)
	{
		constexpr T twoPi = (T)2 * (T)PI_D;
		const T mod = fmod(theta, twoPi);
		if (mod > (T)PI_D)
		{
			return mod - twoPi;
		}
		else if (mod < (T)PI_D)
		{
			return mod + twoPi;
		}
		return mod;
	}

	template<typename T>
	constexpr T interpolate(const T& src, const T& dst, float alpha)
	{
		return src + (dst - src) * alpha;
	}

	template<typename T>
	constexpr T to_rad(T deg)
	{
		return deg * PI / (T)180.0;
	}


static	float AngleFromXY(float x, float y)
	{
		float theta = 0.0f;

		// Quadrant I or IV
		if (x >= 0.0f)
		{
			// If x = 0, then atanf(y/x) = +pi/2 if y > 0
			//                atanf(y/x) = -pi/2 if y < 0
			theta = atanf(y / x); // in [-pi/2, +pi/2]

			if (theta < 0.0f)
				theta += 2.0f * PI; // in [0, 2*pi).
		}

		// Quadrant II or III
		else
			theta = atanf(y / x) + PI; // in [0, 2*pi).

		return theta;
	}
}