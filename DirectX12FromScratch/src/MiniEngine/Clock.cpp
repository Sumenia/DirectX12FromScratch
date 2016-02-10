#include "MiniEngine/SysClock.h"
#include "MiniEngine/Clock.h"

using namespace MiniEngine;

Clock::Clock()
{

}

Clock::Clock(Clock & c)
{

}

Clock::~Clock()
{

}

Time
Clock::getWallClock()
{
	return Time(SysClock::getCurrentTime());
}

Time
Clock::getElapsedTime(Time start, Time end)
{
	return Time(end.getMicroseconds() - start.getMicroseconds());
}
