
#if defined(_WIN32)
#include <windows.h>
#elif defined(__APPLE__) && defined(__MACH__)
#include <mach/mach_time.h>
#elif defined(__unix__)
#include <time.h>
#endif

#include "MiniEngine/SysClock.h"

#if defined(_WIN32)
LARGE_INTEGER
getFrequency()
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	return frequency;
}
#endif

using namespace MiniEngine;

Time
SysClock::getCurrentTime()
{
#if defined(_WIN32)

	// Force the following code to run on first core
	// (see http://msdn.microsoft.com/en-us/library/windows/desktop/ms644904(v=vs.85).aspx)
	HANDLE currentThread = GetCurrentThread();
	DWORD_PTR previousMask = SetThreadAffinityMask(currentThread, 1);

	// Get the frequency of the performance counter
	// (it is constant across the program lifetime)
	static LARGE_INTEGER frequency = getFrequency();

	// Get the current time
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);

	// Restore the thread affinity
	SetThreadAffinityMask(currentThread, previousMask);

	// Return the current time as microseconds
	return Time(static_cast<int64_t>(1000000 * time.QuadPart / frequency.QuadPart)); 

#elif defined(__APPLE__) && defined(__MACH__)

	// Mac OS X specific implementation (it doesn't support clock_gettime)
	static mach_timebase_info_data_t frequency = { 0, 0 };
	if (frequency.denom == 0)
		mach_timebase_info(&frequency);
	Uint64 nanoseconds = mach_absolute_time() * frequency.numer / frequency.denom;
	return Time(nanoseconds / 1000);

#elif defined(__unix__)

	// POSIX implementation
	timespec time;
	clock_gettime(CLOCK_MONOTONIC, &time);
	return sf::microseconds(static_cast<Uint64>(time.tv_sec) * 1000000 + time.tv_nsec / 1000); 

#endif

}
