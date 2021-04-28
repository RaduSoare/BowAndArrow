#pragma once
class Balloon
{
public:
	float coordX;
	float coordY;
	float elevation;
	float gravity = 0;
	char color;
	bool isHit = false;
	
	
	Balloon(float x, float y, float elv, char color);
	Balloon();

private:
	
};