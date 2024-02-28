#pragma once
#include <algorithm>
#include "eEasing.h"
#include "EasingFunction.h"

class Tween
{
private:
    Easing easing;
    float duration;     // 총 시간
    float elapsedTime;  // 경과 시간

    float startValue;
    float endValue;

public:
    Tween(Easing easingFunc, float durationSec, float start, float end) :
	easing(easingFunc), duration(durationSec),
	elapsedTime(0.0f),
	startValue(start), endValue(end)
    {}

    // Tween을 갱신하는 함수
    void Update(float deltaTime)
    {
        elapsedTime += deltaTime;
    }

    // 현재 Tween의 보간된 값을 가져오는 함수
    float GetValue() const
    {
        // 경과 시간을 보간 함수에 전달하여 현재 값 계산
        const float t = std::min(1.0f, elapsedTime / duration);   // 경과 시간을 0부터 1까지의 값으로 정규화
        const float interpolatedT = EasingFunc[easing](t);                    // 보간 함수를 통해 t값을 변환하여 반환
        return startValue + (endValue - startValue) * interpolatedT;    // 시작 값과 끝 값 사이를 보간
    }
};

