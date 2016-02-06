#pragma once

#include <stdint.h> //contains various typedefs (int64_t, int32_t)

namespace MiniEngine
{

	class Time
	{
	private:
		int64_t _microseconds;

	public:
		Time();
		Time(Time &);
		Time(int64_t);
		~Time();

		int64_t getMicroseconds() const;
		int32_t getMilliseconds() const;
		float	getSeconds() const;
	};

}