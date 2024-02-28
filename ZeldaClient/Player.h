#pragma once
#include "Component.h"

class Player : public PurahEngine::Component
{
public:
				~Player()		override = default;
	void		Awake()			override;
	void		Start()			override;
	void		FixedUpdate()	override;
	void		Update()		override;
	void		Initialize()	override;

public:
	float speed;
	float linearDrag;
	float angularDrag;
};

