#pragma once

#include "MiniEngine/Time.h"

namespace MiniEngine
{

	class Clock
	{
	private:
		Time _startTime;

	public:
		Clock();
		Clock(Clock &);
		~Clock();

		Time getElapsedTime();
		void reset();
	};
}