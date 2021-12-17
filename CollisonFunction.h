#pragma once

class CollisionFunction
{
public:
	virtual ~CollisionFunction() {}

	virtual void OnCollided() = 0;
};