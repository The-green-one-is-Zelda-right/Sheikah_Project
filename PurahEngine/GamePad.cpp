#include <algorithm>
#include <cmath>
#include "Timer.h"
#include "GamePad.h"


namespace PurahEngine
{
	void GamePad::Initialize(int _id, ePad* _inputArr, int _size)
	{
		id = _id;

		ZeroMemory(&state, sizeof(XINPUT_STATE));

		// �Է� �� �ʱ�ȭ
		for (auto i = 0; i < _size; i++)
		{
			inputMap.insert(std::make_pair(_inputArr[i], State::NONE));
		}
	}

	void GamePad::Update()
	{
		const auto newState = GetState();

		UpdateInputMap(newState);

	}

	void GamePad::UpdateInputMap(const XINPUT_STATE& _state)
	{
		_state.dwPacketNumber;

		for (auto& [keyCode, currState] : inputMap)
		{
			const WORD button = static_cast<WORD>(keyCode);

			// ���� ���� ������Ʈ
			if (_state.Gamepad.wButtons & button && currState == State::NONE)
			{
				currState = State::DOWN;
			}
			else if (_state.Gamepad.wButtons & button && currState == State::DOWN)
			{
				currState = State::PRESSED;
			}
			else if (!(_state.Gamepad.wButtons & button) && currState == State::DOWN)
			{
				currState = State::UP;
			}
			else if (!(_state.Gamepad.wButtons & button) && currState == State::PRESSED)
			{
				currState = State::UP;
			}
			else if (!(_state.Gamepad.wButtons & button) && currState == State::UP)
			{
				currState = State::NONE;
			}
		}
	}

	XINPUT_STATE GamePad::GetState()
	{
		XInputGetState(id, &state);
		XInputGetKeystroke(id, 0, &stroke);
		return state;
	}

	bool GamePad::IsKeyDown(ePad _input)
	{
		return inputMap[_input] == State::DOWN;
	}

	bool GamePad::IsKeyPressed(ePad _input)
	{
		return inputMap[_input] == State::PRESSED;
	}

	bool GamePad::IsKeyUp(ePad _input)
	{
		return inputMap[_input] == State::UP;
	}

	int GamePad::GetTriggerValue(ePadTrigger _index) const
	{
		if (_index == ePadTrigger::ePAD_TRIGGER_L)
		{
			return state.Gamepad.bLeftTrigger;
		}
		else
		{
			return state.Gamepad.bRightTrigger;
		}
	}

	float GamePad::GetTriggerRatio(ePadTrigger _index) const
	{
		return inv255 * static_cast<float>(GetTriggerValue(_index));
	}

	void GamePad::GetStickValue(ePadStick _index, int& _outX, int& _outY) const
	{
		int xValue = 0, yValue = 0;

		// ���� ��ƽ�� ���
		if (_index == ePadStick::ePAD_STICK_L)
		{
			xValue = state.Gamepad.sThumbLX;
			yValue = state.Gamepad.sThumbLY;
		}
		// ���� ��ƽ�� ���
		else if (_index == ePadStick::ePAD_STICK_R)
		{
			xValue = state.Gamepad.sThumbRX;
			yValue = state.Gamepad.sThumbRY;
		}

		// ������ üũ �� �� �Ҵ�
		_outX = std::abs(xValue) < deadZone ? 0 : xValue;
		_outY = std::abs(yValue) < deadZone ? 0 : yValue;
	}

	void GamePad::GetStickRatio(ePadStick _index, float& _outX, float& _outY) const
	{
		// X, Y ���� ���� ������ ����
		int xValue = 0, yValue = 0;

		GetStickValue(_index, xValue, yValue);

		if (xValue != 0)
		{
			_outX = static_cast<float>(xValue) * inv32767;
		}
		if (yValue != 0)
		{
			_outY = static_cast<float>(yValue) * inv32767;
		}
	}

	bool GamePad::Vibrate(int _left, int _right) const
	{
		XINPUT_VIBRATION vibration;
		ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
		vibration.wLeftMotorSpeed = static_cast<unsigned short>(_left);
		vibration.wRightMotorSpeed = static_cast<unsigned short>(_right);
		return XInputSetState(id, &vibration) == ERROR_SUCCESS;
	}

	bool GamePad::VibrateRatio(float _left, float _right) const
	{
		_left = std::clamp(_left, 0.f, 1.f);
		_right = std::clamp(_right, 0.f, 1.f);

		const auto left = static_cast<int>(_left * static_cast<float>(USHORT_MAX));
		const auto right = static_cast<int>(_right * static_cast<float>(USHORT_MAX));

		return Vibrate(left, right);
	}

	void GamePad::Vibrate(int _left, int _right, float _time) const
	{
		Timer::Delay(
			_time, 
			false, 
			[this]()
			{
				Vibrate(0, 0);
			}
		);
		Vibrate(_left, _right);
	}

	void GamePad::VibrateRatio(float _left, float _right, float _time) const
	{
		Timer::Delay(
			_time,
			false,
			[this]()
			{
				VibrateRatio(0.f, 0.f);
			}
		);
		VibrateRatio(_left, _right);
	}

	void GamePad::SetDeadZone(int _value)
	{
		deadZone = _value; // ������ ���� �����մϴ�.
	}

	void GamePad::SetDeadZoneRatio(float _ratio)
	{
		_ratio = std::clamp(_ratio, 0.f, 1.f);

		// ��ƽ ���� ����: -32768 ~ 32767
		// ������ ���� ������ �����մϴ�.
		// ���� ���, 0.2�� ��� -32768 * 0.2 = -6553.6 �̹Ƿ�,
		// -6553.6���� 6553.6 ������ ���� ���������� ������ �� �ֽ��ϴ�.
		deadZone = static_cast<int>(32767.0f * _ratio);
	}

	bool GamePad::IsConnected()
	{
		return XInputGetState(id, &state) == ERROR_SUCCESS;
	}
}
