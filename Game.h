#pragma once
#include <Component/SimpleScene.h>
#include <Core/Engine.h>
#include"Balloon.h"
#include"Arrow.h"
#include"Shuriken.h"
#include"Drawer.h"
class Game : public SimpleScene
{
public:
	Game();
	~Game();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;


	void RenderBalloon(Balloon *balloon, float deltaTimeSeconds);
	void RenderBow(float deltaTimeSeconds);
	void RenderArrows(Arrow* arrow, float deltaTimeSeconds, int currentArrow);
	void RenderShuriken(Shuriken *shuriken, float deltaTimeSeconds);
	void RenderPowerbar(Arrow* arrow, float deltaTimeSeconds);


	void checkColissionShurikenBow(Shuriken* shuriken);
	bool checkColissionArrowBalloon(Balloon balloon);

	Balloon* allocateBalloons();
	Shuriken* allocateShurikens();
	
	int getRandom(int min, int max);
	void UpdateScore(Balloon* balloon);





protected:
	
	glm::mat3 modelMatrix;
	int score = 0;
	int level = 1;
	
	float bowRotate;
	float translateBowX, translateBowY;
	
	Balloon* balloons;
	
	Shuriken* shurikens;
	float shurikenRotationSpeed;

	Arrow arrow;
	float arrowXpos;
	float arrowYpos;
	bool arrowReleased;

	Drawer drawer;

};