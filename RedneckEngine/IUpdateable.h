#pragma once

class IUpdateable
{
public:
	virtual void Update(float dt) = 0;
	virtual ~IUpdateable() = default;
};