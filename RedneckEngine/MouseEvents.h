#pragma once
#include "Event.h"
#include "InputSystem.h"

// Called when mouse moves
struct MouseMovedEvent
{
	POINT GetDeltaPosition() const 
	{ 
		return { pos.x - old_pos.x, pos.y - old_pos.y };
	}

	// New mouse position
	POINT pos;
	// Mouse position before it was moved
	POINT old_pos;
};

// Called when mouse wheel is scrolled
struct MouseScrolledEvent
{
	// Mouse position when scroll wheel moved
	POINT pos;
	// How much the scroll wheel was moved
	float delta;
};

// Called when a mouse button is pressed
struct MouseButtonPressedEvent
{
	// Mouse position when button was pressed
	POINT pos;
	// Mouse button that was pressed
	InputSystem::MouseButton button;
};

// Called when a mouse button is released
struct MouseButtonReleasedEvent
{
	// Mouse position when button was released
	POINT pos;
	// Mouse button that was released
	InputSystem::MouseButton button;
};

// Called when a mouse button 
struct MouseButtonDoubleClickEvent
{
	// Mouse position when button was double clicked
	POINT pos;
	// Mouse button that was double clicked
	InputSystem::MouseButton button;
};

// Called when mouse enters window client area
struct MouseEnterEvent {};

// Called when mouse exits window client area
struct MouseLeaveEvent {};