#include "MiniEngine/Time.h"

using namespace MiniEngine;

Time::Time()
	: _microseconds(0)
{

}

Time::Time(Time & t)
	: _microseconds(t.getMicroseconds())
{

}

Time::Time(int64_t time)
	: _microseconds(time)
{

}

Time::~Time()
{

}

int64_t
Time::getMicroseconds() const
{
	return _microseconds;
}

int32_t
Time::getMilliseconds() const
{
	return static_cast<int32_t>(_microseconds / 1000);
}

float
Time::getSeconds() const
{
	return _microseconds / 1000000.0f;
}