#include "MiniEngine/Clock.h"

using namespace MiniEngine;

Clock::Clock()
	:	_oldTime(std::chrono::system_clock::now())
{

}

Clock::Clock(Clock & c)
	:	_oldTime(c.getOldTime())
{

}

Clock::~Clock()
{

}

Clock &
Clock::operator=(Clock & c)
{
	_oldTime = c.getOldTime();
}

t_time_point
Clock::getOldTime() const
{
	return _oldTime;
}

void
Clock::update()
{
	_oldTime = std::chrono::system_clock::now();
}

/*
long long
Clock::getTimeAsSeconds() const
{
	return ((float)(std::chrono::duration_cast<std::chrono::microseconds>(_curTime.time_since_epoch()).count()) / 1000000);
}

float
Clock::getTimeAsMilliseconds() const
{
	return ((float)(std::chrono::duration_cast<std::chrono::microseconds>(_curTime.time_since_epoch()).count()) / 1000);
}

float
Clock::getTimeAsMicroseconds() const
{
	return ((float)(std::chrono::duration_cast<std::chrono::microseconds>(_curTime.time_since_epoch()).count()));
}
*/

float
Clock::getElapsed()
{
	return (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - _oldTime).count() / 1000.0f);
}
