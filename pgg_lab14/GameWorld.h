/*!
*  \brief     GameWorld Class.
*  \details   This class is to create and run the Game World
*  \author    James Robertson
*  \version   1.0a
*  \date      2015
*  \copyright GNU Public License.
*/

#pragma once

#include <SDL.h>
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "glew.h"
#include "ObjLoader.h"
#include "GameModel.h"
#include "Controller.h"
#include "Camera.h"

class GameWorld
{
public:
	/// Constructor and Destructor
	GameWorld();
	~GameWorld();

	/// Initialisation
	void initialiseAll();
	void initialiseSDL();
	bool initialiseOpenGL();
	void initialiseScene();

	/// Render 2D images on the screen
	void render2DImages(SDL_Texture* Image, SDL_Rect Location, bool Update);

	/// Image Convertor
	SDL_Texture* createImage(std::string filename);

	/// In Game Loop
	void keyInputHandler();
	bool updateGame(uint8_t Score);
	void updateObjects();
	void drawObjects();

private:
	// SDL Specific Stuffs
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Surface *surface;
	SDL_GLContext glContext;
	SDL_Event incomingEvent;

	// Models
	GameModel* playerRocket;
	GameModel* enemyPaddle;

	GameModel* Ball;
	float ballDirectionX = 0.0f;
	float ballDirectionY = -1.0f;
	uint8_t scoreRight;
	uint8_t scoreLeft;

	GameModel* Terrain;

	// Xbox Controller
	Controller* xboxController;

	// 3D Camera
	Camera* camera;

	// Boolean to keep the loop going
	bool go;

	bool scoreReset = false;

	// Store if the controller is connected
	bool xboxControllerConnected;

	bool controllerisnotset = true;

	bool isDraw = true;

	bool Lose = false;

	uint8_t score = 0;

	char leftSide = 'M';

	float currentVibrationValue = 0.0f;

	// Timings
	float deltaTime = 0.0f;
	float spinAmount = 0.0f;
	uint32_t lastTime = 0;
	uint32_t current = 0;

	// Rocket Speed
	const float MAX_SPINAMOUNT = 27.0f;
	const float SPIN_ACCELERATION = 1.3f;
	const float MAX_SPEED = 38.5f;
	float currentSpeed = 0.0f;

	// Window Specific Attributes
	uint16_t winPosX = 0;
	uint16_t winPosY = 0;
	uint16_t winWidth = 1920;
	uint16_t winHeight = 1080;
};
