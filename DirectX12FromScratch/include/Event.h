////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#pragma once

#include <cstdlib>
#include "Keyboard.h"
#include "Mouse.h"

class Event
{
public:

	struct SizeEvent
	{
		unsigned int width;
		unsigned int height;
	};

	struct KeyEvent
	{
		Keyboard::Key code;
		bool          alt;
		bool          control;
		bool          shift;
		bool          system;
	};

	struct TextEvent
	{
		unsigned int unicode;
	};

	struct MouseMoveEvent
	{
		int x;
		int y;
	};

	struct MouseButtonEvent
	{
		Mouse::Button button;
		int           x;
		int           y;
	};


	struct MouseWheelScrollEvent
	{
		Mouse::Wheel wheel;
		float        delta;
		int          x;
		int          y;
	};

	enum EventType
	{
        Unknown,
		Closed,
		Resized,
		LostFocus,
		GainedFocus,
		TextEntered,
		KeyPressed,
		KeyReleased,
		MouseWheelScrolled,
		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
		MouseEntered,
		MouseLeft,
		JoystickButtonPressed,
		JoystickButtonReleased,
		JoystickMoved,
		JoystickConnected,
		JoystickDisconnected,
		TouchBegan,
		TouchMoved,
		TouchEnded,
		SensorChanged,

		Count
	};


	EventType type;

	union
	{
		SizeEvent             size;
		KeyEvent              key;
		TextEvent             text;
		MouseMoveEvent        mouseMove;
		MouseButtonEvent      mouseButton;
		MouseWheelScrollEvent mouseWheelScroll;
	};
};
