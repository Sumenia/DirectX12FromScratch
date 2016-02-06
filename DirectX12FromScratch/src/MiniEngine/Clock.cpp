#include "MiniEngine/SysClock.h"
#include "MiniEngine/Clock.h"

using namespace MiniEngine;

Clock::Clock()
	: _startTime(SysClock::getCurrentTime())
{

}

Clock::Clock(Clock & c)
	: _startTime(c.getElapsedTime())
{

}

Clock::~Clock()
{

}

Time
Clock::getElapsedTime()
{
	return Time(SysClock::getCurrentTime().getMicroseconds() - _startTime.getMicroseconds());
}

void
Clock::reset()
{
	_startTime = SysClock::getCurrentTime();
}