#pragma once
#include "Entity.h"
class Movement : public Component
{
public:
	Movement();
	~Movement();


	void Update(float deltaTime) override;
};

