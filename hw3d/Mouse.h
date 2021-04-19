#pragma once

#include <queue>

class Mouse
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			Move,
			
			LPress,
			LRelease,
			
			MPress,
			MRelease,
			
			RPress,
			RRelease,
			
			WheelUp,
			WheelDown,
			
			Enter,
			Leave,
			
			Invalid
		};
	public:
		Event() noexcept
			: type{ Type::Invalid }
			, x{ 0 }
			, y{ 0 }
			, isLeftPressed{ false }
			, isRightPressed{ false }
		{}
		Event(Type type, const Mouse& parent)
			: type{ type }
			, x{ parent.x }
			, y{ parent.y }
			, isLeftPressed{ parent.isLeftPressed }
			, isRightPressed{ parent.isRightPressed }
		{}
		Type GetType() const noexcept
		{
			return type;
		}
		int GetX() const noexcept
		{
			return x;
		}
		int GetY() const noexcept
		{
			return y;
		}
		std::pair<int, int> GetPos() const noexcept
		{
			return { x, y };
		}
		bool IsLeftPressed() const noexcept
		{
			return isLeftPressed;
		}
		bool IsRightPressed() const noexcept
		{
			return isRightPressed;
		}
	private:
		Type type;
		int x;
		int y;
		bool isLeftPressed;
		bool isRightPressed;
	};
public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;

	Event GetEvent() noexcept;
	bool IsEventQueueEmpty() const noexcept;
	bool IsLeftPressed() const noexcept;
	bool IsMiddlePressed() const noexcept;
	bool IsRightPressed() const noexcept;
	bool IsInWindow() const noexcept;

	void ClearEventQueue() noexcept;
private:
	void OnMove(int x, int y) noexcept;
	void OnLeftPressed() noexcept;
	void OnLeftReleased() noexcept;
	void OnMiddlePressed() noexcept;
	void OnMiddleReleased() noexcept;
	void OnRightPressed() noexcept;
	void OnRightReleased() noexcept;
	void OnWheelUp() noexcept;
	void OnWheelDown() noexcept;
	void OnEnter() noexcept;
	void OnLeave() noexcept;

	void TrimQueue() noexcept;
private:
	int x;
	int y;
	bool isLeftPressed;
	bool isMiddlePressed;
	bool isRightPressed;
	bool isInWindow;
	static constexpr size_t maxQueueSize = 16;
	std::queue<Event> eventQueue;
};