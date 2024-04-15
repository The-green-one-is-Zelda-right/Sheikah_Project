#pragma once
#include <map>
#include <queue>

// ���� �߿���.
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

		void Initialize(int _id, ePad* _inputArr, int _size);
		void Update();

		void UpdateInputMap(const XINPUT_STATE& _state);

		/// �����е�
		XINPUT_STATE		GetState();

	public:
		friend GamePadManager;
		enum class State
		{
			NONE,
			DOWN,
			PRESSED,
			UP,
		};

		/// Ű �Է�
		bool				IsKeyDown(ePad _input);
		bool				IsKeyPressed(ePad _input);
		bool				IsKeyUp(ePad _input);
		State				IsKeyValue(ePad _input);

		/// Ʈ���� ��
		int					GetTriggerValue(ePadTrigger _index) const;
		int					GetTriggerRawValue(ePadTrigger _index) const;
		float				GetTriggerRatio(ePadTrigger _index) const;

		/// ��ƽ ��
	private:
		void				ApplyDeadZone(int& _value, float _deadZone) const;
		void				StickValueNormalize(int _xValue, int _yValue, float& _outX, float& _outY, float _deadZone);

	public:
		int					GetStickInput(ePadStick _index);
		int					GetStickValue(ePadStick _index, int&, int&) const;
		void				GetStickRawValue(ePadStick _index, int& _outX, int& _outY) const;
		float				GetStickRatio(ePadStick _index, float& _outX, float& _outY) const;
		void				GetStickRawRatio(ePadStick _index, float& _outX, float& _outY) const;

		/// ����
		bool				Vibrate(int _left, int _right) const;
		bool				VibrateRatio(float _left, float _right) const;

		void				Vibrate(int, int, float _time) const;
		void				VibrateRatio(float, float, float _time) const;

		void				VibrateOff() const;

		/// ������ ����
		void				SetDeadZone(unsigned int);
		void				SetDeadZoneRatio(float _ratio);

		/// ���� ����
		bool				IsConnected();

	private:
		bool					enable;
		int						id;
		XINPUT_STATE			state;
		XINPUT_KEYSTROKE		stroke;
		int						deadZone = 1000;
		std::map<ePad, State>	inputMap;
		// std::map<> ���� ���õ� ü���� �����ϴ� ������ ������ ������.
	};
}
