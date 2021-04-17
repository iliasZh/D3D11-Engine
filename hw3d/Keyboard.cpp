#include "Keyboard.h"

bool Keyboard::IsKeyPressed(unsigned char keycode) const noexcept
{
    return keyStates[keycode];
}

Keyboard::Event Keyboard::GetKeyEvent() noexcept
{
    if (!IsKeyEventQueueEmpty())
    {
        const Event e = keyEventQueue.front();
        keyEventQueue.pop();
        return e;
    }
    else
    {
        return Keyboard::Event();
    }
}

bool Keyboard::IsKeyEventQueueEmpty() const noexcept
{
    return keyEventQueue.empty();
}

void Keyboard::ClearKeyEventQueue() noexcept
{
    keyEventQueue = std::queue<Event>();
}

char Keyboard::GetChar() noexcept
{
    if (!IsCharQueueEmpty())
    {
        const char ch = charQueue.front();
        charQueue.pop();
        return ch;
    }
    else
    {
        return 0;
    }
}

bool Keyboard::IsCharQueueEmpty() const noexcept
{
    return charQueue.empty();
}

void Keyboard::ClearCharQueue() noexcept
{
    charQueue = std::queue<char>();
}

void Keyboard::ClearQueues() noexcept
{
    ClearCharQueue();
    ClearKeyEventQueue();
}

void Keyboard::OnKeyPressed(unsigned char keycode) noexcept
{
    keyStates[keycode] = true;
    keyEventQueue.emplace(Event::Type::Press, keycode);
    TrimQueue(keyEventQueue);
}

void Keyboard::OnKeyReleased(unsigned char keycode) noexcept
{
    keyStates[keycode] = false;
    keyEventQueue.emplace(Event::Type::Release, keycode);
    TrimQueue(keyEventQueue);
}

void Keyboard::OnChar(char ch) noexcept
{
    charQueue.emplace(ch);
    TrimQueue(charQueue);
}

template <class T>
void Keyboard::TrimQueue(std::queue<T>& queue) noexcept
{
    while (queue.size() > maxQueueSize)
    {
        queue.pop();
    }
}

void Keyboard::ClearKeyStates() noexcept
{
    keyStates.reset();
}
