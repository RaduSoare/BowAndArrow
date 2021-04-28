#pragma once
class Arrow
{
public:
	float xSpeed = 0;
	float ySpeed = 0;
	float flyingAngle = 0;
	float translateX = 0;
	float translateY = 0;
	bool isFlying = false;
	float arrowAcceleration = 500;

	Arrow();
};

