#include <iostream>
#include "MiniEngine/Application.h"
#include "MiniEngine/Clock.h"

using namespace MiniEngine;

Application::Application()
{
	_root = new Root;
}

Application::~Application()
{
	delete _root;
}

void Application::run()
{

	// Set the Windows scheduler granularity to 1ms
	// so that our Sleep() will be more "precise".
	UINT DesiredSchedulerMS = 1;
	float SleepIsGranular = (timeBeginPeriod(DesiredSchedulerMS) == TIMERR_NOERROR);

	Clock clock;
	int monitorRefreshHz = 30;

	// TODO(Platy): Find the window's HWND to find the monitor's refresh rate
	//HDC refreshDC = GetDC(/* HWND */);
	//int refreshRate = GetDeviceCaps(refreshDC, VREFRESH);
	//ReleaseDC(/* HWND */, refreshDC);
	//if (refreshRate > 1)
	//{
	//	monitorRefreshHz = refreshRate;
	//}
	
	float gameUpdateHz = static_cast<float>(monitorRefreshHz / 2.0f);
	float targetSecondsPerFrame = 1.0f / gameUpdateHz;
	Time lastCounter = clock.getWallClock();

	while (true)
	{
		// Time
		float secondsElapsedForFrame = clock.getElapsedTime(lastCounter, clock.getWallClock()).getSeconds();;
		if (SleepIsGranular)
		{
			uint16_t SleepMS = static_cast<uint64_t>(1000.0f * (targetSecondsPerFrame - secondsElapsedForFrame));
			if (SleepMS > 0)
			{
				Sleep(SleepMS);
			}
		}

		while (secondsElapsedForFrame < targetSecondsPerFrame)
		{
			secondsElapsedForFrame = clock.getElapsedTime(lastCounter, clock.getWallClock()).getSeconds();
		}
		// !Time

		if (!update(secondsElapsedForFrame * 1000000))
			return;

		if (!renderOneFrame())
			return;
		lastCounter = clock.getWallClock();
	}
}

bool Application::renderOneFrame()
{
	if (_root->getRenderSystem())
		return (_root->getRenderSystem()->render());

	return (false);
}