#pragma once

#include "MiniEngine/Time.h"

namespace MiniEngine
{

	class Clock
	{
	public:
		Clock();
		Clock(Clock &);
		~Clock();

		Time getWallClock(); // Gets current time
		Time getElapsedTime(Time start, Time end); // Returns time passed between two Time variables
	};
}