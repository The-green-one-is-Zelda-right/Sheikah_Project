#pragma once

namespace flt
{
	template<typename T>
	__declspec(dllexport) constexpr T PI = T(3.141592653589793238462643383279502884197169399375105820974944592307816406286);

	__declspec(dllexport) float DegToRad(float degree);

	__declspec(dllexport) float RadToDeg(float radian);

	extern __declspec(dllexport) float FLOAT_EPSILON;
}
