#include"Game.h"
#include"Balloon.h"
#include"Arrow.h"
#include"Constants.h"
#include "Transform2D.h"

#include<iostream>
#include <time.h>
#include <string.h>
#include <Core/Engine.h>

using namespace std;

Game::Game()
{
}

Game::~Game()
{
}


void Game::Init()
{
	/*
		Set game's window
	*/
	
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	/*
		Draw game's components
	*/
	drawer.InitBow(&meshes, bowRadius, arrowLength, arrowHeadHeight);
	drawer.InitShuriken(&meshes, shurikenSide);
	drawer.InitBalloon(&meshes, "balloonRed", balloonRadius);
	drawer.InitBalloon(&meshes, "balloonYellow", balloonRadius);
	drawer.InitBalloon(&meshes, "balloonWhite", balloonRadius);
	drawer.InitPowerBar(&meshes);

	// Allocate arrays of balloons and shurikens
	balloons = allocateBalloons();
	shurikens = allocateShurikens();
	
}

void Game::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Game::Update(float deltaTimeSeconds)
{	
	// Render player
	RenderBow(deltaTimeSeconds);
	
	// Render arrow
	RenderArrows(&arrow, deltaTimeSeconds, 0);
	
	// Render balloons
	for (int i = 0; i < numberOfBalloons; i++) {
		// check for collisions between arrow and balloons and update score
		UpdateScore(&balloons[i]);
		RenderBalloon(&balloons[i], deltaTimeSeconds);
	} 
	
	// Render shurikens
	for (int i = 0; i < numberOfShurikens; i++) {
		// check for collisions between player and shurikens and update player's lives
		checkColissionShurikenBow(&shurikens[i]);
		RenderShuriken(&shurikens[i], deltaTimeSeconds);
	} 

	// Render powerbar
	RenderPowerbar(&arrow, deltaTimeSeconds);
	
}


void Game::RenderPowerbar(Arrow* arrow, float deltaTimeSeconds) {
	
	modelMatrix = Transform2D::Translate(bowX - bowRadius, bowY + translateBowY - bowRadius);

	// Show powerbar if the arrow was not released
	if (arrowReleased == false) {
		modelMatrix *= Transform2D::Scale(1, arrowAcceleration / 10);
	}
	else {
		// Hide the powerbar
		modelMatrix *= Transform2D::Scale(0, 0);
	}
	
	RenderMesh2D(meshes["powerbar"], shaders["VertexColor"], modelMatrix);
} 

void Game::RenderShuriken(Shuriken *shuriken, float deltaTimeSeconds) {
	
	shurikenRotationSpeed += deltaTimeSeconds * 2;

	// Compute distance from the initial position
	shuriken->distance += deltaTimeSeconds * shurikenSpeed;

	// Respawn if the shuriken came off the screen 
	if (shuriken->distance >= 1280 || shuriken->hit == true) {
		shuriken->distance = 0;
		shuriken->coordX = getRandom(1280, 1300);
		shuriken->coordY = getRandom(0, 720);
		shuriken->hit = false;
	}

	modelMatrix = Transform2D::Translate(shuriken->coordX, shuriken->coordY);
	// Translation on OX
	modelMatrix *= Transform2D::Translate(-shuriken->distance, 0);
	// Rotation around the center
	modelMatrix *= Transform2D::Rotate(shurikenRotationSpeed);
	RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrix);
}

void Game::RenderBalloon(Balloon *balloon, float deltaTimeSeconds) {

	// Check for the type of balloon we want to render 
	const char* balloonType = new const char;
	switch (balloon->color) {
	case 'r':
		balloonType = "balloonRed";
		break;
	case 'y':
		balloonType = "balloonYellow";
		break;
	case 'w':
		balloonType = "balloonWhite";
		break;
	}

	modelMatrix = Transform2D::Translate(balloon->coordX, balloon->coordY);
	/*
		If the balloon is white, it uses animation which simulates gravity
	*/
	if (balloon->color == 'w') {
		balloon->gravity += deltaTimeSeconds * gravitationalAcceleration;
		modelMatrix *= Transform2D::Translate(0, - balloon->gravity);
		// Check if the baloon has reached the ground
		if (balloon->coordY + balloon->elevation - balloon->gravity < 0 ) {
			balloon->gravity = -1;
		}
	}

	balloon->elevation += deltaTimeSeconds * baloonSpeed;
	/*
		Respawn if the balloon came off the screen
	*/
	if (balloon->elevation >= screenHeight + -(balloon->coordY) || balloon->gravity == -1) {
		balloon->elevation = 0;
		balloon->color = (getRandom(0, 1) == 0) ? 'r' : 'y';
		balloon->gravity = 0;
		// Get random coordinates every time it appears on the screen
		balloon->coordX = getRandom(400, 1100);
		balloon->coordY = getRandom(-50, 0);
	}
	// Translation on OY
	modelMatrix *= Transform2D::Translate(0, balloon->elevation);
	modelMatrix *= Transform2D::Scale(balloonScaleX, balloonScaleY);
	RenderMesh2D(meshes[balloonType], shaders["VertexColor"], modelMatrix);

	// Render balloon string
	modelMatrix = Transform2D::Translate(balloon->coordX, balloon->coordY - balloonRadius * balloonScaleY);
	// Gravity animation
	if (balloon->color == 'w') {
		modelMatrix *= Transform2D::Translate(0, -balloon->gravity);
	}
	modelMatrix *= Transform2D::Translate(0, balloon->elevation);
	RenderMesh2D(meshes["balloonBottom"], shaders["VertexColor"], modelMatrix);
}



void Game::RenderBow(float deltaTimeSeconds) {
	// Render straight part of the arrow
	modelMatrix = Transform2D::Translate(bowX, bowY);
	modelMatrix *= Transform2D::Translate(0, translateBowY);
	modelMatrix *= Transform2D::Rotate(bowRotate);
	RenderMesh2D(meshes["bowStraightLine"], shaders["VertexColor"], modelMatrix);

	// Render curved part of the arrow
	modelMatrix = Transform2D::Translate(bowX, bowY);
	modelMatrix *= Transform2D::Translate(0, translateBowY);
	modelMatrix *= Transform2D::Rotate(bowRotate);
	
	RenderMesh2D(meshes["bowCurve"], shaders["VertexColor"], modelMatrix);
	
}

/*
 Check if the the arrowhead touches the balloon
*/
bool Game :: checkColissionArrowBalloon(Balloon balloon) {
	if (sqrt(pow(abs(arrowXpos - balloon.coordX), 2) + pow(abs(arrowYpos - (balloon.coordY + balloon.elevation)), 2)) <= balloonRadius * 2) {
		return true;
	} 
	return false;
}

/*
 Check if the the shuriken touches the player
*/
void Game::checkColissionShurikenBow(Shuriken* shuriken) {
	float dx = bowX - (shuriken->coordX - shuriken->distance);
	float dy = (bowY + translateBowY) - shuriken->coordY;
	float distance = sqrt(dx * dx + dy * dy);

	float radiusSum = bowRadius + shurikenRadius;

	if (distance < radiusSum && shuriken->hit == false) {
		shuriken->hit = true;
		playerLives--;
		if (playerLives == 0) {
			cout << "You lost the game!" << endl;
			exit(1);
		}
		cout << "You lost a life. " << playerLives << " more left" << endl;
		return;
	}
	
}



void Game::RenderArrows(Arrow* arrow, float deltaTimeSeconds, int currentArrow) 
{
	/*
		Render arrow's stick
	*/
	modelMatrix = Transform2D::Translate(bowX + bowRadius + bowGap, bowY);

	// Retains position and rotation of the arrow before the release
	if (arrowReleased == false) {
		arrow->flyingAngle = bowRotate;
		arrow->translateY = translateBowY;
	} 
	// Compute arrow's current coordinates
	arrowXpos = bowX + (bowRadius + bowGap + arrowLength) * glm::cos(arrow->flyingAngle) + arrow->xSpeed;
	arrowYpos = bowY + arrow->ySpeed + arrow->translateY + (bowRadius + bowGap + arrowLength) * glm::sin(arrow->flyingAngle);

	// Compute arrow's movement after the release
	if (arrowReleased == true) {
		arrow->arrowAcceleration = arrowAcceleration;

		// Compute the speed on both axis
		arrow->xSpeed += deltaTimeSeconds * arrow->arrowAcceleration;
		arrow->ySpeed += glm::sin(arrow->flyingAngle) * deltaTimeSeconds * arrow->arrowAcceleration;
		modelMatrix *= Transform2D::Translate(arrow->xSpeed, arrow->ySpeed);

		// Keep the trajectory given by the bow
		modelMatrix *= Transform2D::Translate(-(bowRadius + bowGap), 0);
		modelMatrix *= Transform2D::Translate(0, arrow->translateY);
		modelMatrix *= Transform2D::Rotate(arrow->flyingAngle);
		modelMatrix *= Transform2D::Translate((bowRadius + bowGap), 0);
		
		// Check if the arrow is still on screen
		if (arrowXpos >= screenWidth || 
			arrowYpos >= screenHeight ||
			arrowYpos <= 0) {
			arrow->xSpeed = 0;
			arrow->ySpeed = 0;
			arrow->translateY = 0;
			arrow->flyingAngle = 0;
			arrow->isFlying = false; 
			arrowReleased = false;
			arrowAcceleration = arrowMinAcceleration;
			
		} 
	}
	else {
		// Follow bow movement
		modelMatrix *= Transform2D::Translate(-(bowRadius + bowGap), 0);
		modelMatrix *= Transform2D::Translate(0, translateBowY);
		modelMatrix *= Transform2D::Rotate(bowRotate);
		modelMatrix *= Transform2D::Translate((bowRadius + bowGap), 0);
		// Charging the bow animation
		modelMatrix *= Transform2D::Translate(-arrowAcceleration / 10, 0);
	}
	
	RenderMesh2D(meshes["arrowStraightLine"], shaders["VertexColor"], modelMatrix);

	// Follow the rest of the arrow's movement
	modelMatrix = Transform2D::Translate(bowX + bowRadius + bowGap + arrowLength, bowY);
	if (arrowReleased == true) {
		modelMatrix *= Transform2D::Translate(arrow->xSpeed, arrow->ySpeed);
		modelMatrix *= Transform2D::Translate(-(bowRadius + bowGap + arrowLength), 0);
		modelMatrix *= Transform2D::Translate(0, arrow->translateY);
		modelMatrix *= Transform2D::Rotate(arrow->flyingAngle);
		modelMatrix *= Transform2D::Translate((bowRadius + bowGap + arrowLength), 0);
	} 
	else {
		
		modelMatrix *= Transform2D::Translate(-(bowRadius + bowGap + arrowLength), 0);
		modelMatrix *= Transform2D::Translate(0, translateBowY);
		modelMatrix *= Transform2D::Rotate(bowRotate);
		modelMatrix *= Transform2D::Translate((bowRadius + bowGap + arrowLength), 0);
		// Charging the bow animation
		modelMatrix *= Transform2D::Translate(-arrowAcceleration / 10, 0);
	}

	RenderMesh2D(meshes["arrowHead"], shaders["VertexColor"], modelMatrix);
}

void Game::OnInputUpdate(float deltaTime, int mods)
{
	// Player's movement on OY
	if (window->KeyHold(GLFW_KEY_W))
	{
		
		if (bowY + translateBowY <= screenHeight - bowRadius) {
			translateBowY += deltaTime * playerSpeed;
		}
	}
	if (window->KeyHold(GLFW_KEY_S))
	{
		
		if (bowY + translateBowY >= bowRadius) {
			translateBowY -= deltaTime * playerSpeed;
		}
	}
	// Get the acceleration given to the arrow
	if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
		
		if (arrowAcceleration < arrowMaxAcceleration) {
			arrowAcceleration = arrowAcceleration + 10;
		}

	}
	
}

void Game::FrameEnd() {}

void Game::OnKeyPress(int key, int mods){}

void Game::OnKeyRelease(int key, int mods){}

void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// Using arctg(Y/X) to compute the angle at which the bow must rotate
	float temp = (float)(-(mouseY - bowY + translateBowY)) / (float)(mouseX - bowX);
	bowRotate = glm::atan(temp) * 1.5;

	// Rotation limits at -45 and 45 degree
	if (bowRotate > glm::pi<float>() / 4) {
		bowRotate = glm::pi<float>() / 4;

	} else if (bowRotate < -glm::pi<float>() / 4) {
		bowRotate = -glm::pi<float>() / 4;

	}

}

void Game::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods){}

void Game::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// Check if the arrow was released
	arrowReleased = true;
}

void Game::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY){}

void Game::OnWindowResize(int width, int height){}

Balloon* Game::allocateBalloons() {
	int rangeMin = 400, rangeMax = 1100;
	Balloon *balloons = new Balloon[numberOfBalloons];
	for (int i = 0; i < numberOfBalloons; i++) {
		int min = rangeMin + (100 * i);
		int max = rangeMin + (200 * i);
		float randX = getRandom(min, max);
		float randY = getRandom(-500, 0);
		char color = getRandom(0, 1) == 0 ? 'r' : 'y';
		balloons[i] = Balloon(randX, randY, 0, color);
	}
	return balloons;

}

/*
	Allocate the array of shurikens
	Assign random starting positions
*/
Shuriken* Game::allocateShurikens() {
	Shuriken* shurikens = new Shuriken[numberOfBalloons];
	int rangeMin = 0, rangeMax = 720;
	for (int i = 0; i < numberOfShurikens; i++) {
		float randX = getRandom(1000, 1200);
		float randY = getRandom(rangeMin, rangeMax);
		shurikens[i] = Shuriken(randX, randY);
	}
	return shurikens;
}

// Random generator
int Game::getRandom(int min, int max) {
	return rand() % (max - min + 1) + min;
}

/*
 Check for balloon colission and update the score depending of the color
*/
void Game::UpdateScore(Balloon* balloon) {
	if (checkColissionArrowBalloon(*balloon) && balloon->color != 'w') {
		if (balloon->color == 'r') {
			score += redBalloonPoints;
		}
		else {
			score += yellowBalloonPoints;
		}
		cout << "NEW SCORE IS : " << score << endl;
		/*
			LEVEL UP System
			La fiecare prag de 50 de puncte acumulate se avanseaza in nivel,
		deci creste si dificultatea jocului:
			- numarul de baloane creste
			- viteza baloanelor creste
			- numarul de shurikens creste
		*/
		if (score >= level * 50) {
			level++;
			cout << "You have passed level " << level << endl;
			numberOfBalloons += 2;
			numberOfShurikens++;
			baloonSpeed += 50;
		}
		balloon->color = 'w';
	}
}
