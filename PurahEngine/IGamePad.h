#pragma once

#include "ePad.h"
#include "PurahEngineAPI.h"

namespace PurahEngine
{
	class PURAHENGINE_API IGamePad
	{
	public:
		/// 키 입력
		virtual bool		GetKey(ePad _input) = 0;
		virtual bool		IsKeyDown(ePad _input) = 0;
		virtual bool		IsKeyPressed(ePad _input) = 0;
		virtual bool		IsKeyUp(ePad _input) = 0;
		virtual bool		IsKeyReleased(ePad _input) = 0;
		virtual ePadState	IsKeyValue(ePad _input) = 0;

		/// 트리거 값
		virtual int			GetTriggerRawValue(ePadTrigger _index) const = 0;
		virtual float		GetTriggerRatio(ePadTrigger _index) const = 0;

		virtual int			GetStickValue(ePadStick _index, int&, int&) const = 0;
		virtual float		GetStickRatio(ePadStick _index, float& _outX, float& _outY) const = 0;

		/// 진동
		virtual bool		VibrateRatio(float _left, float _right) const = 0;
		virtual void		VibrateRatio(float _left, float _right, float _time) = 0;

		virtual void		VibrateStop() = 0;
		virtual void		VibrateResume() = 0;
		virtual void		VibrateOff() const = 0;

		/// 데드존 설정
		virtual void		SetDeadZone(unsigned int) = 0;
		virtual void		SetDeadZoneRatio(float _ratio) = 0;

		/// 연결 여부
		virtual bool		IsConnected() = 0;
	};
}