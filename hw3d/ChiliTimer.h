#pragma once

#include <chrono>

class ChiliTimer
{
public:
	ChiliTimer() noexcept;			// initializes mark when constructed
	float Mark() noexcept;			// time since last mark, updates the mark
	float Peek() const noexcept;	// time since last mark, does not update the mark
private:
	
	std::chrono::steady_clock::time_point last;
};