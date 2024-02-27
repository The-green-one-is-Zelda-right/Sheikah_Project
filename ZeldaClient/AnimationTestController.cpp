#include "AnimationTestController.h"

void AnimationTestController::Awake()
{
	timer = 0.0f;

	animator = GetGameObject()->GetComponent<Animator>();
	
	animator->SetLoop(0u, true);
	animator->SetBlendTime(0u, 0u, 2.0f);
	animator->SetBlend(true);
	animator->SetPlaySpeed(0u, 0.05f);

	animator->Play(0u);
}

void AnimationTestController::Update()
{
	assert(animator != nullptr);

	timer += 0.016f;
	
	const static float playTime = 20.0f;

	if (timer > playTime)
	{
		animator->Play(0u);

		timer -= playTime;
	}
}
