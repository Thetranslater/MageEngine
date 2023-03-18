#pragma once

#include<cmath>
#include<algorithm>
#include<limits>
#include<cassert>

namespace Mage {
	class Vector3;

	class Mathf {
	public:
		static inline float Abs(float value) { return std::fabs(value); }
		static inline bool  Approximately(float a, float b) { return std::fabs(b - a) <= Episilon; }
		static inline float Acos(float value) { return std::acosf(std::clamp(value, -1.f, 1.f)); }//return in radians
		static inline float Asin(float value) { return std::asinf(std::clamp(value, -1.f, 1.f)); }//return in radians
		static inline float Atan(float value) { return std::atan(value); }										   //return in radians
		static inline float Atan2(float y, float x) { return std::atan2(y, x); }								   //return in radians
		static inline float Cos(float value) { return std::cos(value); }                                          //use in radians
		static inline float Ceil(float value) { return std::ceilf(value); }
		static inline int   CeilToInt(float value) { return int(std::ceilf(value)); }
		static inline float Clamp(float value, float min, float max) { return std::clamp(value, min, max); }
		static inline float Clamp01(float value) { return std::clamp(value, 0.f, 1.f); }
		static inline int   RoundUp(int value, int alighment) {
			int temp = value + alighment - 1;
			return temp - temp % alighment - (temp < 0 ? alighment : 0);
		}
		//find the closest power of two of the given value, if value is greater than 2^30,the result will always shrink to 2^30
		static inline int   ClosestPowerOfTwo(int value) {
			if (value <= 0) return 0;
			int ori = value;
			value = (value & 0xffff0000) ? value & 0xffff0000 : value & 0x0000ffff;
			value = (value & 0xff00ff00) ? value & 0xff00ff00 : value & 0x00ff00ff;
			value = (value & 0xf0f0f0f0) ? value & 0xf0f0f0f0 : value & 0x0f0f0f0f;
			value = (value & 0xcccccccc) ? value & 0xcccccccc : value & 0x33333333;
			value = (value & 0xaaaaaaaa) ? value & 0xaaaaaaaa : value & 0x55555555;
			if ((value << 1) - ori < (ori - value)) {
				value = (value << 1) < 0 ? value : value << 1;
			}
			return value;
		}
		//use in degree, the result always in the range of [0,180]
		static inline float DeltaAngle(float current, float target) {
			float delta = std::fmod(current, 360.f);
			current = delta < 0.f ? 360.f + delta : delta;
			delta = std::fmod(target, 360.f);
			target = delta < 0.f ? 360.f + delta : delta;

			delta = std::fabs(target - current);
			if (delta > 180.f) delta = 360.f - delta;
			return delta;
		}
		static inline float Exp(float power) { return std::expf(power); }
		static inline float Floor(float value) { return std::floorf(value); }
		static inline int   FloorToInt(float value) { return int(std::floorf(value)); }

		//将范围转化为百分比用来插值
		static inline float InverseLerp(float a, float b, float value) {
			if (value <= a) return 0.f;
			if (value >= b) return 1.f;
			return (value - a) / (b - a);
		}
		static inline bool  IsPowerOfTwo(int value) {
			if (value < 0) return false;
			return (value & (value - 1)) == 0;
		}
		//with clamp;
		static inline float Lerp(float a, float b, float alpha) {
			alpha = Clamp01(alpha);
			return a + alpha * (b - a);
		}
		//without clamp
		static inline float LerpUnclamped(float a, float b, float alpha) {
			return a + alpha * (b - a);
		}
		static inline float Log(float f, float p) {
			return std::logf(f) / std::logf(p);
		}
		static inline float Log10(float value) { return std::log10(value); }

		template<typename Arg>
		static Arg      Max(Arg farg, Arg sarg) {
			return std::max(farg, sarg);
		}
		static Vector3  Max(const Vector3& l, const Vector3& r);
		template<typename Arg>
		static Arg      Max(Arg farg, Arg sarg, Arg targ) {
			return std::max({ farg,sarg,targ });
		}
		template<typename FirstArg,typename SecArg,typename... Args>
		static FirstArg Max(FirstArg farg, SecArg sarg, Args... args) {
			return Max(std::max(farg, sarg), args...);
		}

		template<typename Arg>
		static Arg      Min(Arg farg, Arg sarg) { return std::min(farg, sarg); }
		static Vector3  Min(const Vector3& l, const Vector3& r);
		template<typename Arg>
		static Arg      Min(Arg farg, Arg sarg, Arg targ) { return std::min({ farg,sarg,targ }); }
		template<typename FirstArg,typename SecArg,typename... Args>
		static FirstArg Min(FirstArg farg, SecArg sarg, Args... args) { return Min(std::min(farg, sarg), args...); }

		static inline float MoveTowards(float current, float target, float maxDelta) {
			return (current + maxDelta < target) ? current + maxDelta : target;
		}
		//TODO: use in degree and maxDelta is unsupported to negative
		static float MoveTowardAngle(float current, float target, float maxDelta);
		//if value is greater than 2^30, it will have error
		static inline int   NextPowerOfTwo(int value) {
			if (value <= 0) return 0;
			int ori = value;
			value = (value & 0xffff0000) ? value & 0xffff0000 : value & 0x0000ffff;
			value = (value & 0xff00ff00) ? value & 0xff00ff00 : value & 0x00ff00ff;
			value = (value & 0xf0f0f0f0) ? value & 0xf0f0f0f0 : value & 0x0f0f0f0f;
			value = (value & 0xcccccccc) ? value & 0xcccccccc : value & 0x33333333;
			value = (value & 0xaaaaaaaa) ? value & 0xaaaaaaaa : value & 0x55555555;
			return ori == value ? value : value << 1;
		}
		//reqiures that t is a self-increment value
		static inline float PingPong(float t, float length) {
			float double_length = length + length;
			t = std::fmod(t, double_length);
			if (t < 0.f) t += double_length;
			return (t > length) ? double_length - t : t;
		}
		//reqiures that t is a self-increment value
		static inline float Repeat(float t, float length) {
			t = std::fmod(t, length);
			return t < 0.f ? t + length : t;
		}
		static inline float Round(float value) { return std::rint(value); }
		static inline int   RoundToInt(float value) { return int(std::rint(value)); }
		static inline float Sign(float value) { return value < 0.f ? -1.f : 1.f; }
		static inline float Sin(float value) { return std::sin(value); }										   //use in radians
		static inline float Sqrt(float value) { return std::sqrt(value); }
		static inline float Tan(float value) { return std::tan(value); }										   //use in radians

		//properties
		static const float PI;
		static const float Rad2Deg;
		static const float Episilon;
		static const float Deg2Rad;
		static const float Infinity;
		static const float NegativeInfinity;
	};
}