#pragma once

// Gameplay Constants
int score = 0;
int playerLives = 3;
int redBalloonPoints = 5;
int yellowBalloonPoints = -2;
int gravitationalAcceleration = 300;
int screenWidth = 1280;
int screenHeight = 720;

// Player Constants
float bowX = 100, bowY = 400;
float bowRadius = 50;
float arrowLength = 80;
float bowGap = 20;
float arrowHeadHeight = 5;
int playerSpeed = 200;

// Balloon Constants
float balloonRadius = 15;
float balloonScaleX = 1, balloonScaleY = 2;
float balloonX = 600, balloonY = 400;
int numberOfBalloons = 10;
int baloonSpeed = 100;

// Shuriken Constants
float shurikenSide = 20;
float shurikenSize = 2 * shurikenSide;
float shurikenRadius = shurikenSize * sqrt(2) / 2;
int numberOfShurikens = 3;
float shurikenSpeed = 400;

//  Constants
float arrowMaxAcceleration = 1000;
float arrowMinAcceleration = 500;
float arrowAcceleration = arrowMinAcceleration;