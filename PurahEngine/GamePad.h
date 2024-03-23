#pragma once
#include <map>

// 순서 중요함.
#include <Windows.h>
#include <Xinput.h>
//

#include "ePad.h"
#include "PurahEngineAPI.h"
#pragma comment(lib, "Xinput.lib")

namespace PurahEngine
{
	class GamePadManager;

	constexpr short				SHORT_MAX = 32767;
	constexpr unsigned short	USHORT_MAX = 65535;
	constexpr float				inv32767 = 1.f / 32767.0f;
	constexpr float				inv255 = 1.f / 255.f;

	class PURAHENGINE_API GamePad
	{
	private:
		enum class State
		{
			NONE,
			DOWN,
			PRESSED,
			UP,
		};

		void Initialize(int _id, ePad* _inputArr, int _size);
		void Update();

		void UpdateInputMap(const XINPUT_STATE& _state);

		/// 게임패드
		XINPUT_STATE		GetState();

	public:
		friend GamePadManager;

		/// 키 입력
		bool				IsKeyDown(ePad _input);
		bool				IsKeyPressed(ePad _input);
		bool				IsKeyUp(ePad _input);

		/// 트리거 값
		int					GetTriggerValue(ePadTrigger _index) const;
		int					GetTriggerRawValue(ePadTrigger _index) const;
		float				GetTriggerRatio(ePadTrigger _index) const;

		/// 스틱 값
	private:
		void				ApplyDeadZone(int& _value, float _deadZone) const;
		void				StickValueNormalize(int _xValue, int _yValue, float& _outX, float& _outY, float _deadZone);

	public:
		int					GetStickInput(ePadStick _index);
		int					GetStickValue(ePadStick _index, int&, int&) const;
		void				GetStickRawValue(ePadStick _index, int& _outX, int& _outY) const;
		float				GetStickRatio(ePadStick _index, float& _outX, float& _outY) const;
		void				GetStickRawRatio(ePadStick _index, float& _outX, float& _outY) const;

		/// 진동
		bool				Vibrate(int _left, int _right) const;
		bool				VibrateRatio(float _left, float _right) const;
		void				Vibrate(int, int, float _time) const;
		void				VibrateRatio(float, float, float _time) const;

		/// 데드존 설정
		void				SetDeadZone(unsigned int);
		void				SetDeadZoneRatio(float _ratio);

		/// 연결 여부
		bool				IsConnected();

	private:
		bool					enable;
		int						id;
		XINPUT_STATE			state;
		XINPUT_KEYSTROKE		stroke;
		int						deadZone = 1000;
		std::map<ePad, State>	inputMap;
		// std::map<> 진동 관련된 체널을 관리하는 뭔가가 있으면 좋겠음.
	};
}
