#include "ChiliTimer.h"

// initializes mark when constructed
ChiliTimer::ChiliTimer() noexcept
{
	last = std::chrono::steady_clock::now();
}

// time since last mark, updates the mark
float ChiliTimer::Mark() noexcept
{
	const auto old = last;
	last = std::chrono::steady_clock::now();
	const std::chrono::duration<float> frameTime = last - old;
	return frameTime.count();
}

// time since last mark, does not update the mark
float ChiliTimer::Peek() const noexcept
{
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - last).count();
}
