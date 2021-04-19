#include "Mouse.h"

Mouse::Event Mouse::GetEvent() noexcept
{
	if (!IsEventQueueEmpty())
	{
		const Event e = eventQueue.front();
		eventQueue.pop();
		return e;
	}
	else
		return Event();
}

bool Mouse::IsEventQueueEmpty() const noexcept
{
	return eventQueue.empty();
}

bool Mouse::IsLeftPressed() const noexcept
{
	return isLeftPressed;
}

bool Mouse::IsMiddlePressed() const noexcept
{
	return isMiddlePressed;
}

bool Mouse::IsRightPressed() const noexcept
{
	return isRightPressed;
}

bool Mouse::IsInWindow() const noexcept
{
	return isInWindow;
}

void Mouse::ClearEventQueue() noexcept
{
	eventQueue = std::queue<Event>();
}

void Mouse::OnMove(int x, int y) noexcept
{
	this->x = x;
	this->y = y;
	eventQueue.emplace(Event::Type::Move, *this);
	TrimQueue();
}

void Mouse::OnLeftPressed() noexcept
{
	isLeftPressed = true;
	eventQueue.emplace(Event::Type::LPress, *this);
	TrimQueue();
}

void Mouse::OnLeftReleased() noexcept
{
	isLeftPressed = false;
	eventQueue.emplace(Event::Type::LRelease, *this);
	TrimQueue();
}

void Mouse::OnMiddlePressed() noexcept
{
	isMiddlePressed = true;
	eventQueue.emplace(Event::Type::MPress, *this);
	TrimQueue();
}

void Mouse::OnMiddleReleased() noexcept
{
	isMiddlePressed = false;
	eventQueue.emplace(Event::Type::MRelease, *this);
	TrimQueue();
}

void Mouse::OnRightPressed() noexcept
{
	isRightPressed = true;
	eventQueue.emplace(Event::Type::RPress, *this);
	TrimQueue();
}

void Mouse::OnRightReleased() noexcept
{
	isRightPressed = false;
	eventQueue.emplace(Event::Type::RRelease, *this);
	TrimQueue();
}

void Mouse::OnWheelUp() noexcept
{
	eventQueue.emplace(Event::Type::WheelUp, *this);
	TrimQueue();
}

void Mouse::OnWheelDown() noexcept
{
	eventQueue.emplace(Event::Type::WheelDown, *this);
	TrimQueue();
}

void Mouse::OnEnter() noexcept
{
	isInWindow = true;
	eventQueue.emplace(Mouse::Event::Type::Enter, *this);
	TrimQueue();
}

void Mouse::OnLeave() noexcept
{
	isInWindow = false;
	eventQueue.emplace(Mouse::Event::Type::Leave, *this);
	TrimQueue();
}

void Mouse::TrimQueue() noexcept
{
	while (eventQueue.size() > maxQueueSize)
	{
		eventQueue.pop();
	}
}