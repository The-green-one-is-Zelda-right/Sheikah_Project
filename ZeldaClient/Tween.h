#pragma once
#include <algorithm>
#include "eEasing.h"
#include "EasingFunction.h"

class Tween
{
private:
    Easing easing;
    float duration;     // �� �ð�
    float elapsedTime;  // ��� �ð�

    float startValue;
    float endValue;

public:
    Tween(Easing easingFunc, float durationSec, float start, float end) :
	easing(easingFunc), duration(durationSec),
	elapsedTime(0.0f),
	startValue(start), endValue(end)
    {}

    // Tween�� �����ϴ� �Լ�
    void Update(float deltaTime)
    {
        elapsedTime += deltaTime;
    }

    // ���� Tween�� ������ ���� �������� �Լ�
    float GetValue() const
    {
        // ��� �ð��� ���� �Լ��� �����Ͽ� ���� �� ���
        const float t = std::min(1.0f, elapsedTime / duration);   // ��� �ð��� 0���� 1������ ������ ����ȭ
        const float interpolatedT = EasingFunc[easing](t);                    // ���� �Լ��� ���� t���� ��ȯ�Ͽ� ��ȯ
        return startValue + (endValue - startValue) * interpolatedT;          // ���� ���� �� �� ���̸� ����
    }
};

