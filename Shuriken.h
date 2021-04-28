#pragma once
class Shuriken
{
public:
	Shuriken();
	Shuriken::Shuriken(float x, float y);

	float coordX = 0;
	float coordY = 0;
	float distance = 0;
	bool hit = false;
};