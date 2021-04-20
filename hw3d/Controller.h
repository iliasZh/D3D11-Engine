#pragma once

#include "ChiliWin.h"
#include <Xinput.h>
#include <cassert>
#include <cmath>
#include "Utilities.h"

#define BUTTON_A		XINPUT_GAMEPAD_A
#define BUTTON_B		XINPUT_GAMEPAD_B
#define BUTTON_X		XINPUT_GAMEPAD_X
#define BUTTON_Y		XINPUT_GAMEPAD_Y
#define SHOULDER_LB		XINPUT_GAMEPAD_LEFT_SHOULDER
#define SHOULDER_RB		XINPUT_GAMEPAD_RIGHT_SHOULDER
#define DPAD_UP			XINPUT_GAMEPAD_DPAD_UP
#define DPAD_DOWN		XINPUT_GAMEPAD_DPAD_DOWN
#define DPAD_LEFT		XINPUT_GAMEPAD_DPAD_LEFT
#define DPAD_RIGHT		XINPUT_GAMEPAD_DPAD_RIGHT
#define BUTTON_START	XINPUT_GAMEPAD_START
#define BUTTON_BACK		XINPUT_GAMEPAD_BACK
#define STICK_LS		XINPUT_GAMEPAD_LEFT_THUMB
#define STICK_RS		XINPUT_GAMEPAD_RIGHT_THUMB

class Controller
{
public:
	Controller()
	{
		ZeroMemory(&state, sizeof(XINPUT_STATE));
	}
	bool IsConnected()
	{
		return XInputGetState(0, &state) == ERROR_SUCCESS;
	}
	bool IsButtonPressed(WORD buttonCode) const
	{
		return (state.Gamepad.wButtons & buttonCode) != 0;
	}
	float LeftTriggerState() const
	{
		const auto lt = (float)state.Gamepad.bLeftTrigger / triggerMaxVal;
		assert(lt >= 0.0f && lt <= 1.0f);
		return lt;
	}
	float RightTriggerState() const
	{
		const auto rt = (float)state.Gamepad.bRightTrigger / triggerMaxVal;
		assert(rt >= 0.0f && rt <= 1.0f);
		return rt;
	}
	float LeftStickX() const
	{
		return GetNormalizedStickInput(state.Gamepad.sThumbLX, deadzoneX);
	}
	float LeftStickY() const
	{
		return GetNormalizedStickInput(state.Gamepad.sThumbLY, deadzoneY);
	}
	float RightStickX() const
	{
		return GetNormalizedStickInput(state.Gamepad.sThumbRX, deadzoneX);
	}
	float RightStickY() const
	{
		return GetNormalizedStickInput(state.Gamepad.sThumbRY, deadzoneY);
	}
	void SetDeadzone(float dx, float dy)
	{
		SetDeadzoneX(dx);
		SetDeadzoneY(dy);
	}
	void SetDeadzoneX(float dx)
	{
		// 0.0f <= deadzoneX <= deadzoneXMax
		deadzoneX = Clamp(dx, 0.0f, deadzoneXMax);
	}
	void SetDeadzoneY(float dy)
	{
		// 0.0f <= deadzoneY <= deadzoneYMax
		deadzoneY = Clamp(dy, 0.0f, deadzoneYMax);
	}
private:
	float Clamp(float val, float min, float max) const
	{
		if (val < min)
			return min;
		else if (val > max)
			return max;
		else
			return val;
	}
	float GetNormalizedStickInput(SHORT rawStickVal, const float& deadzone) const
	{
		const float stickRaw = Clamp((float)rawStickVal / stickMaxVal, -1.0f, 1.0f);
		assert(stickRaw >= -1.0f && stickRaw <= 1.0f);
		return std::abs(stickRaw) < deadzone ? 
			0.0f 
			:
			sign(stickRaw) * (std::abs(stickRaw) - deadzone) / (1.0f - deadzone);
	}
private:
	XINPUT_STATE state;
	float deadzoneX = 0.15f;
	float deadzoneXMax = 0.5f;
	float deadzoneY = 0.15f;
	float deadzoneYMax = 0.5f;
	static constexpr float stickMaxVal = 32767.0f;	// -stickMaxVal - 1 <= stickVal <= stickMaxVal
	static constexpr float triggerMaxVal = 255.0f;	// 0 <= triggerVal <= triggerMaxVal
};