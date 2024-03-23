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

		// 입력 맵 초기화
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

			// 현재 상태 업데이트
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

	int GamePad::GetStickInput(ePadStick _index)
	{
		return 0;
	}

	int GamePad::GetStickValue(ePadStick _index, int& _outX, int& _outY) const
	{
		// X, Y 값을 각각 변수로 설정
		int xValue = 0, yValue = 0;

		GetStickRawValue(_index, xValue, yValue);

		const auto LX = static_cast<float>(xValue);
		const auto LY = static_cast<float>(yValue);

		const float distanceSquared = LX * LX + LY * LY;

		float magnitude = 0.0f;

		if (distanceSquared > static_cast<float>(deadZone * deadZone))
		{
			// 거리 클립
			magnitude = std::sqrtf(distanceSquared);

			_outX = LX / magnitude;
			_outY = LX / magnitude;

			// 크기 클립
			if (magnitude > 32767)
				magnitude = 32767;

			// 데드존부터 입력을 받기 시작하니 크기도 조절함.
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
		// 좌측 스틱의 경우
		if (_index == ePadStick::ePAD_STICK_L)
		{
			_outX = state.Gamepad.sThumbLX;
			_outY = state.Gamepad.sThumbLY;
		}
		// 우측 스틱의 경우
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

		// 거리의 제곱이 데드존보다 큰지 비교하여 분기 최적화
		if (distanceSquared > deadZone * deadZone)
		{
			// 거리 클립
			magnitude = std::sqrtf(distanceSquared);

			_outX = LX / magnitude;
			_outY = LY / magnitude;

			if (magnitude > 32767.0f)
				magnitude = 32767.0f;

			magnitude -= static_cast<float>(deadZone);

			// 1 / (32767 - deadZone) 값을 한 번만 계산
			const float inv = 1.0f / (32767.0f - static_cast<float>(deadZone));

			normalizedMagnitude = magnitude * inv;
		}
		else // 컨트롤러가 데드존 안에 있는 경우
		{
			_outX = 0.0f;
			_outY = 0.0f;
		}

		return normalizedMagnitude;
	}

	void GamePad::GetStickRawRatio(ePadStick _index, float& _outX, float& _outY) const
	{
		// X, Y 값을 각각 변수로 설정
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

	void GamePad::SetDeadZone(unsigned int _value)
	{
		deadZone = _value; // 데드존 값을 설정합니다.
	}

	void GamePad::SetDeadZoneRatio(float _ratio)
	{
		_ratio = std::clamp(_ratio, 0.f, 1.f);

		// 스틱 값의 범위: -32768 ~ 32767
		// 데드존 값을 비율로 정의합니다.
		// 예를 들어, 0.2의 경우 -32768 * 0.2 = -6553.6 이므로,
		// -6553.6에서 6553.6 사이의 값을 데드존으로 설정할 수 있습니다.
		deadZone = static_cast<int>(32767.0f * _ratio);
	}

	bool GamePad::IsConnected()
	{
		return XInputGetState(id, &state) == ERROR_SUCCESS;
	}
}
