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

#include "Vector.h"

////////////////////////////////////////////////////////////
/// \brief Give access to the real-time state of the mouse
///
////////////////////////////////////////////////////////////
class Mouse
{
public:

	////////////////////////////////////////////////////////////
	/// \brief Mouse buttons
	///
	////////////////////////////////////////////////////////////
	enum Button
	{
		Left,       ///< The left mouse button
		Right,      ///< The right mouse button
		Middle,     ///< The middle (wheel) mouse button
		XButton1,   ///< The first extra mouse button
		XButton2,   ///< The second extra mouse button

		ButtonCount ///< Keep last -- the total number of mouse buttons
	};

	////////////////////////////////////////////////////////////
	/// \brief Mouse wheels
	///
	////////////////////////////////////////////////////////////
	enum Wheel
	{
		VerticalWheel,  ///< The vertical mouse wheel
		HorizontalWheel ///< The horizontal mouse wheel
	};

	////////////////////////////////////////////////////////////
	/// \brief Check if a mouse button is pressed
	///
	/// \param button Button to check
	///
	/// \return True if the button is pressed, false otherwise
	///
	////////////////////////////////////////////////////////////
	static bool isButtonPressed(Button button);

	////////////////////////////////////////////////////////////
	/// \brief Get the current position of the mouse in desktop coordinates
	///
	/// This function returns the global position of the mouse
	/// cursor on the desktop.
	///
	/// \return Current position of the mouse
	///
	////////////////////////////////////////////////////////////
	static Vector2i getPosition();

	////////////////////////////////////////////////////////////
	/// \brief Get the current position of the mouse in window coordinates
	///
	/// This function returns the current position of the mouse
	/// cursor, relative to the given window.
	///
	/// \param relativeTo Reference window
	///
	/// \return Current position of the mouse
	///
	////////////////////////////////////////////////////////////
	//static Vector2i getPosition(const Window& relativeTo);

	////////////////////////////////////////////////////////////
	/// \brief Set the current position of the mouse in desktop coordinates
	///
	/// This function sets the global position of the mouse
	/// cursor on the desktop.
	///
	/// \param position New position of the mouse
	///
	////////////////////////////////////////////////////////////
	static void setPosition(const Vector2i& position);

	////////////////////////////////////////////////////////////
	/// \brief Set the current position of the mouse in window coordinates
	///
	/// This function sets the current position of the mouse
	/// cursor, relative to the given window.
	///
	/// \param position New position of the mouse
	/// \param relativeTo Reference window
	///
	////////////////////////////////////////////////////////////
	//static void setPosition(const Vector2i& position, const Window& relativeTo);
};

