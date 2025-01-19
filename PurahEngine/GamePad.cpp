#define NOMINMAX
#include <cmath>
#include <algorithm>
#include "Timer.h"
#include "GamePad.h"
#include "TimeController.h"

#include <cassert>


namespace PurahEngine
{
	const float GamePad::firstInputDelay = 0.5f;
	const float GamePad::continuousInputCycles = 0.05f;

	void GamePad::Initialize(int _id, ePad* _inputArr, int _size)
	{
		id = _id;

		ZeroMemory(&state, sizeof(XINPUT_STATE));

		// 입력 맵 초기화
		for (auto i = 0; i < _size; i++)
		{
			inputMap.insert(std::make_pair(_inputArr[i], State::NONE));
		}

		// 이전 입력 맵 초기화
		for (auto i = 0; i < _size; i++)
		{
			prevInputMap.insert(std::make_pair(_inputArr[i], State::NONE));
		}

		// keyDownElapsedMap 초기화
		for (auto i = 0; i < _size; i++)
		{
			keyDownElapsedMap.insert(std::make_pair(_inputArr[i], -1.0f));
		}

		for (auto i = 0; i < _size; i++)
		{
			keyMap.insert(std::make_pair(_inputArr[i], false));
		}
	}

	void GamePad::Update()
	{
		const auto newState = GetState();

		for (auto& [keyCode, currState] : keyMap)
		{
			currState = false;
		}

		UpdateVibration();
		UpdateInputMap(newState);
	}

	void GamePad::UpdateInputMap(const XINPUT_STATE& _state)
	{
		float deltaTime = PurahEngine::TimeController::GetInstance().GetDeltaTime();

		_state.dwPacketNumber;

		for (auto& [keyCode, currState] : inputMap)
		{
			const WORD button = static_cast<WORD>(keyCode);

			// 현재 상태를 prevInputMap에 저장
			auto prevState = currState;
			prevInputMap[keyCode] = prevState;

			// 누름
			if (_state.Gamepad.wButtons & button)
			{
				currState = State::DOWN;

				// 키가 방금 눌렸다면 키가 눌린시간을 초기화 한다.
				if (prevState == State::UP)
				{
					keyDownElapsedMap[keyCode] = 0.0f;
					keyMap[keyCode] = true;
				}
				// 키가 눌렸다면 시간을 누적한다.
				else
				{
					keyDownElapsedMap[keyCode] += deltaTime;
					if (keyDownElapsedMap[keyCode] >= firstInputDelay)
					{
						keyDownElapsedMap[keyCode] -= continuousInputCycles;
						keyMap[keyCode] = true;
					}
				}
			}
			// 뗌
			else
			{
				currState = State::UP;
			}
		}
	}

	void GamePad::UpdateVibration()
	{
		float deltaTime = PurahEngine::TimeController::GetInstance().GetDeltaTime();
		float leftPower = 0.f;
		float rightPower = 0.f;

		// 남아있는 진동 중에서 가장 쎈 진동 명령을 찾음
		for (auto it = leftVibeCommend.begin(); it != leftVibeCommend.end();) 
		{
			it->time -= deltaTime; // 남은 시간을 델타 타임만큼 감소

			// 남은 시간이 있는가?
			if (it->time > 0) 
			{
				// 가장 강한 진동 강도를 찾음
				leftPower = std::max(leftPower, it->power);

				++it;  // for문에 넣으면 end을 넘어갈려고 해서 수정함
			}
			else
				// 시간이 다 된 명령은 삭제
				it = leftVibeCommend.erase(it);
		}

		for (auto it = rightVibeCommend.begin(); it != rightVibeCommend.end();)
		{
			it->time -= deltaTime;

			if (it->time > 0)
			{
				rightPower = std::max(rightPower, it->power);

				++it;
			}
			else
				it = rightVibeCommend.erase(it);
		}

		VibrateRatio(leftPower, rightPower);
	}

	XINPUT_STATE GamePad::GetState()
	{
		XInputGetState(id, &state);
		XInputGetKeystroke(id, 0, &stroke);
		return state;
	}

	bool GamePad::GetKey(ePad _input)
	{
		return keyMap[_input];
	}

	bool GamePad::IsKeyDown(ePad _input)
	{
		return (prevInputMap[_input] == State::UP) && (inputMap[_input] == State::DOWN);
	}

	bool GamePad::IsKeyPressed(ePad _input)
	{
		return (prevInputMap[_input] == State::DOWN) && (inputMap[_input] == State::DOWN);
	}

	bool GamePad::IsKeyUp(ePad _input)
	{
		return (prevInputMap[_input] == State::DOWN) && (inputMap[_input] == State::UP);
	}

	bool GamePad::IsKeyReleased(ePad _input)
	{
		return (prevInputMap[_input] == State::UP) && (inputMap[_input] == State::UP);
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

	void GamePad::VibrateStop()
	{
		stopVibe = true;
	}

	void GamePad::VibrateResume()
	{
		stopVibe = false;
	}

	void GamePad::Vibrate(int _left, int _right, float _time)
	{
		float left = 0.f;
		float right = 0.f;
		
		if (_left > 0)
		{
			left = static_cast<float>(_left) / static_cast<float>(USHORT_MAX);
		}
		if (_right > 0)
		{
			right = static_cast<float>(_right) / static_cast<float>(USHORT_MAX);
		}

		VibrateRatio(left, right, _time);
	}

	void GamePad::VibrateRatio(float _left, float _right, float _time)
	{
		_left = std::clamp(_left, 0.f, 1.f);
		if (_left > 0.f)
		{
			VibrateData leftData(_left, _time);
			leftVibeCommend.push_back(leftData);
		}

		_right = std::clamp(_right, 0.f, 1.f);
		if (_right > 0.f)
		{
			VibrateData rightData(_right, _time);
			rightVibeCommend.push_back(rightData);
		}
	}

	void GamePad::VibrateOff() const
	{
		VibrateRatio(0.f, 0.f);
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
