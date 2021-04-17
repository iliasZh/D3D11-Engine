#pragma once

#include <bitset>
#include <queue>

class Keyboard
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			Press,
			Release,
			Invalid
		};
	public:
		Event() noexcept
			: type{ Type::Invalid }
			, keycode{ 0u }				// no key has code 0
		{}
		Event(Type type, unsigned char keycode) noexcept
			: type{ type }
			, keycode{ keycode }
		{}
		bool IsPress() const noexcept
		{
			return type == Type::Press;
		}
		bool IsRelease() const noexcept
		{
			return type == Type::Release;
		}
		bool IsValid() const noexcept
		{
			return type != Type::Invalid;
		}
	private:
		Type type;
		unsigned char keycode;
	};
public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	Event GetKeyEvent() noexcept;
	bool IsKeyPressed(unsigned char keycode) const noexcept;
	bool IsKeyEventQueueEmpty() const noexcept;
	void ClearKeyEventQueue() noexcept;

	char GetChar() noexcept;
	bool IsCharQueueEmpty() const noexcept;
	void ClearCharQueue() noexcept;

	void ClearQueues() noexcept;

	void EnableAutorepeat() noexcept
	{
		autorepeatEnabled = true;
	}
	void DisableAutorepeat() noexcept
	{
		autorepeatEnabled = false;
	}
	bool IsAutorepeatEnabled() const noexcept
	{
		return autorepeatEnabled;
	}
private:
	void OnKeyPressed(unsigned char keycode) noexcept;
	void OnKeyReleased(unsigned char keycode) noexcept;
	void OnChar(char ch) noexcept;

	template <class T>
	void TrimQueue(std::queue<T>& queue) noexcept;

	void ClearKeyStates() noexcept;
private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int maxQueueSize = 16u;
	bool autorepeatEnabled = false;
	std::bitset<nKeys> keyStates;
	std::queue<Event> keyEventQueue;
	std::queue<char> charQueue;
};