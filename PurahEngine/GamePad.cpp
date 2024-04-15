#include <algorithm>
#include <cmath>
#include "Timer.h"
#include "GamePad.h"

#include <cassert>


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

	GamePad::State GamePad::IsKeyValue(ePad _input)
	{
		return inputMap[_input];
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

	int GamePad::GetTriggerRawValue(ePadTrigger _index) const
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

	void GamePad::ApplyDeadZone(int& _value, float _deadZone) const
	{
		if (std::abs(_value) < deadZone)
		{
			_value = 0;
		}
	}

	void GamePad::StickValueNormalize(int _xValue, int _yValue, float& _outX, float& _outY, float _deadZone)
	{
		const float LX = static_cast<float>(_xValue);
		const float LY = static_cast<float>(_yValue);

		float magnitude = sqrtf(LX * LX + LY * LY);
		if (magnitude > deadZone) 
		{
			if (magnitude > 32767) 
			{
				magnitude = 32767;
			}

			magnitude -= deadZone;

			const float normalizedLX = LX / magnitude;
			const float normalizedLY = LY / magnitude;

			_outX = normalizedLX;
			_outY = normalizedLY;
		}
		else {
			magnitude = 0.0;
			_outX = 0.f;
			_outY = 0.f;
		}
	}

	void GamePad::InputQueueClear(ePad _keyCode)
	{
		while (!inputQueue[_keyCode].empty())
		{
			inputQueue[_keyCode].pop();
		}
	}

	int GamePad::GetStickInput(ePadStick _index)
	{
		return 0;
	}

	int GamePad::GetStickValue(ePadStick _index, int& _outX, int& _outY) const
	{
		// X, Y ���� ���� ������ ����
		int xValue = 0, yValue = 0;

		GetStickRawValue(_index, xValue, yValue);

		const auto LX = static_cast<float>(xValue);
		const auto LY = static_cast<float>(yValue);

		const float distanceSquared = LX * LX + LY * LY;

		float magnitude = 0.0f;

		if (distanceSquared > static_cast<float>(deadZone * deadZone))
		{
			// �Ÿ� Ŭ��
			magnitude = std::sqrtf(distanceSquared);

			_outX = LX / magnitude;
			_outY = LX / magnitude;

			// ũ�� Ŭ��
			if (magnitude > 32767)
				magnitude = 32767;

			// ���������� �Է��� �ޱ� �����ϴ� ũ�⵵ ������.
			magnitude -= deadZone;
		}
		else
		{
			magnitude = 0.0;
			_outY = 0.f;
			_outX = 0.f;
		}

		return magnitude;
	}

	void GamePad::GetStickRawValue(ePadStick _index, int& _outX, int& _outY) const
	{
		// ���� ��ƽ�� ���
		if (_index == ePadStick::ePAD_STICK_L)
		{
			_outX = state.Gamepad.sThumbLX;
			_outY = state.Gamepad.sThumbLY;
		}
		// ���� ��ƽ�� ���
		else if (_index == ePadStick::ePAD_STICK_R)
		{
			_outX = state.Gamepad.sThumbRX;
			_outY = state.Gamepad.sThumbRY;
		}
		else
		{
			assert(0);
		}
	}

	float GamePad::GetStickRatio(ePadStick _index, float& _outX, float& _outY) const
	{
		int xValue = 0, yValue = 0;

		GetStickRawValue(_index, xValue, yValue);

		const auto LX = static_cast<float>(xValue);
		const auto LY = static_cast<float>(yValue);

		const float distanceSquared = LX * LX + LY * LY;

		float magnitude = 0.0f;
		float normalizedMagnitude = 0.0f;

		// �Ÿ��� ������ ���������� ū�� ���Ͽ� �б� ����ȭ
		if (distanceSquared > deadZone * deadZone)
		{
			// �Ÿ� Ŭ��
			magnitude = std::sqrtf(distanceSquared);

			_outX = LX / magnitude;
			_outY = LY / magnitude;

			if (magnitude > 32767.0f)
				magnitude = 32767.0f;

			magnitude -= static_cast<float>(deadZone);

			// 1 / (32767 - deadZone) ���� �� ���� ���
			const float inv = 1.0f / (32767.0f - static_cast<float>(deadZone));

			normalizedMagnitude = magnitude * inv;
		}
		else // ��Ʈ�ѷ��� ������ �ȿ� �ִ� ���
		{
			_outX = 0.0f;
			_outY = 0.0f;
		}

		return normalizedMagnitude;
	}

	void GamePad::GetStickRawRatio(ePadStick _index, float& _outX, float& _outY) const
	{
		// X, Y ���� ���� ������ ����
		int xValue = 0, yValue = 0;

		GetStickRawValue(_index, xValue, yValue);

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

	void GamePad::VibrateOff() const
	{
		VibrateRatio(0.f, 0.f);
	}

	void GamePad::SetDeadZone(unsigned int _value)
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
