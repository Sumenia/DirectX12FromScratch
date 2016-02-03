#pragma once

#include <chrono>

namespace MiniEngine
{

	typedef std::chrono::system_clock::time_point t_time_point;
//	typedef std::chrono::system_clock::duration t_duration;

	class Clock
	{
	private:
		t_time_point	_oldTime;

	public:
		Clock();
		Clock(Clock &);
		~Clock();
		Clock & operator=(Clock &);

		t_time_point	getOldTime() const;

		void	update();
		float	getElapsed();
//		long long	getTimeAsSeconds() const;
//		float	getTimeAsMilliseconds() const;
//		float	getTimeAsMicroseconds() const;
	};
}