#pragma once

#include "PurahEngine.h"

using namespace PurahEngine;

class AnimationTestController : public Component
{
public:
	void Awake() override;
	void Update() override;

public:
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;

private:
	float timer;
	Animator* animator;
};

