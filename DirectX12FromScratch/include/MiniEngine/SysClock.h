#pragma once

#include "MiniEngine/Time.h"

namespace MiniEngine
{
	class SysClock
	{
	public:
		static Time getCurrentTime();
	};
}