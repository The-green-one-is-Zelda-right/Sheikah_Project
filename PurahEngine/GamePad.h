#pragma once
#include <unordered_map>
#include <queue>
#include <set>

// 순서 중요함.
#include <Windows.h>
#include <Xinput.h>
//

#include "IGamePad.h"
#include "PurahEngineAPI.h"
#pragma comment(lib, "Xinput.lib")

namespace PurahEngine
{
	class GamePadManager;

	constexpr short				SHORT_MAX = 32767;
	constexpr unsigned short	USHORT_MAX = 65535;
	constexpr float				inv32767 = 1.f / 32767.0f;
	constexpr float				inv255 = 1.f / 255.f;

	class PURAHENGINE_API GamePad : public IGamePad
	{
	private:

		void Initialize(int _id, ePad* _inputArr, int _size);
		void Update();

		void UpdateInputMap(const XINPUT_STATE& _state);
		void UpdateVibration();

		/// 게임패드
		XINPUT_STATE		GetState();

	public:
		friend class UnifiedInputManager;
		friend GamePadManager;

		struct VibrateData
		{
			float power;
			float time;

			VibrateData() : power(), time() {}
			VibrateData(float _p, float _t) : power(_p), time(_t) {}
			VibrateData(const VibrateData& _data) : power(_data.power), time(_data.time) {}
			VibrateData(VibrateData&& _data) noexcept : power(_data.power), time(_data.time) {}

			VibrateData& operator=(const VibrateData& _data)
			{
				if (this != &_data)
				{
					power = _data.power;
					time = _data.time;
				}

				return *this;
			}
			VibrateData& operator=(VibrateData&& _data) noexcept
			{
				if (this != &_data)
				{
					power = _data.power;
					time = _data.time;
				}

				return *this;
			}
		};

	private:
		void				ApplyDeadZone(int& _value, float _deadZone) const;
		void				StickValueNormalize(int _xValue, int _yValue, float& _outX, float& _outY, float _deadZone);

		void				GetStickRawValue(ePadStick _index, int& _outX, int& _outY) const;
		void				GetStickRawRatio(ePadStick _index, float& _outX, float& _outY) const;

		bool				Vibrate(int _left, int _right) const;

	public:
		/// 키 입력
		bool				GetKey(ePad _input) override;
		bool				IsKeyDown(ePad _input) override;
		bool				IsKeyPressed(ePad _input) override;
		bool				IsKeyUp(ePad _input) override;
		bool				IsKeyReleased(ePad _input) override;
		ePadState			IsKeyValue(ePad _input) override;

		/// 트리거 값
		int					GetTriggerRawValue(ePadTrigger _index) const override;
		float				GetTriggerRatio(ePadTrigger _index) const override;

		/// 스틱 값
		int					GetStickValue(ePadStick _index, int& _outX, int& _outY) const override;
		float				GetStickRatio(ePadStick _index, float& _outX, float& _outY) const override;

		/// 진동
		bool				VibrateRatio(float _left, float _right) const override;
		void				VibrateRatio(float _left, float _right, float _time) override;

		void				VibrateStop() override;
		void				VibrateResume() override;
		void				VibrateOff() const override;

		/// 데드존 설정
		void				SetDeadZone(unsigned int) override;
		void				SetDeadZoneRatio(float _ratio) override;

		/// 연결 여부
		bool				IsConnected() override;

	private:
		bool					enable;
		int						id;
		XINPUT_STATE			state;
		XINPUT_KEYSTROKE		stroke;
		bool					lastConnected = false;
		bool					hasConnectionSnapshot = false;
		DWORD					lastConnectionResult = ERROR_DEVICE_NOT_CONNECTED;
		int						deadZone = 1000;
		std::unordered_map<ePad, ePadState>	inputMap;
		std::unordered_map<ePad, ePadState>	prevInputMap;
		std::unordered_map<ePad, float>	keyDownElapsedMap;
		std::unordered_map<ePad, bool>	keyMap;

		// std::map<> 진동 관련된 체널을 관리하는 뭔가가 있으면 좋겠음.
		bool stopVibe = false;
		std::vector<VibrateData> leftVibeCommend;
		std::vector<VibrateData> rightVibeCommend;

	private:
		const static float firstInputDelay;
		const static float continuousInputCycles;
	};
}
