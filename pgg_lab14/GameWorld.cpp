/*!
*  \brief     GameWorld Class.
*  \details   This class is to create and run the Game World
*  \author    James Robertson
*  \version   1.0a
*  \date      2015
*  \copyright GNU Public License.
*/

#include "GameWorld.h"

GameWorld::GameWorld()
{
	// TODO: I don't understand why this has been put here. In the initialiser? 
	if (controllerisnotset)
	{
		// Create new instances of these classes
		xboxController = new Controller();
		controllerisnotset = true;

		// Is the controller connected - Boolean
		xboxControllerConnected = xboxController->isControllerConnected();
	}

	// Initialise the Pointers to NULL
	window = nullptr;
	renderer = nullptr;
	glContext = NULL;

	// Create the camera
	camera = new Camera();

	// The game is looping
	go = true;

	// Initialise SDL - OpenGL - Scene
	initialiseAll();
}
//---------------------------------------------------------120 limit right here----------------------------------------

GameWorld::~GameWorld()
{
	// TODO: There is certainly more pointers to delete. Maybe use some smart pointers to remove the need for this though.
	// Delete Pointers!
	// GameObjects
	delete playerRocket;
	delete enemyPaddle;
	delete Terrain;

	// Componants 
	delete xboxController;
	delete camera;
	
	// Destroy SDL Specific Stuff
	SDL_DestroyRenderer(renderer);
	delete surface;
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
}

void GameWorld::initialiseAll()
{
	// Initialise Everything in this order
	initialiseSDL();
	initialiseOpenGL();
	initialiseScene();
}

void GameWorld::initialiseSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		// Something went very wrong in initialisation, all we can do is exit
		std::cout << "Whoops! Something went very wrong, cannot initialise SDL :(" << std::endl;
	}

	// OpenGL Everything Initialised
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Creae a Window with that information
	window = SDL_CreateWindow("Force Pong",
			 winPosX, winPosY, winWidth, winHeight,
			 SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	// Make a Renderer
	renderer = SDL_CreateRenderer(window, -1, 0);
	glContext = SDL_GL_CreateContext(window);

	// Get Ticks
	lastTime = SDL_GetTicks();
}

bool GameWorld::initialiseOpenGL()
{
	// TODO: Investigate why glewExperimental is used.
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cerr << "Error: GLEW failed to initialise with message: " << glewGetErrorString(err) << std::endl;
		return false;
	}

	// Print out GLEW and OpenGL details into the console
	std::cout << "INFO: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
	std::cout << "INFO: OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "INFO: OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "INFO: OpenGL Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	
	// Enable the depth test to make sure triangles in front are always in front no matter the order they are drawn
	glEnable(GL_DEPTH_TEST);
	return true;
}

void GameWorld::initialiseScene()
{
	// Setup Models
	playerRocket = new GameModel("Paddle.obj");
	enemyPaddle = new GameModel("Paddle.obj");
	Ball = new GameModel("ball.obj");
	Terrain = new GameModel("Pong Level.obj");

	// Position Terrain
	Terrain->SetPosition(0, 0 ,-25);

	// Set object's position like this:
	playerRocket->SetPosition(0, 0, 0);
	playerRocket->SetRotation(0, 0, 0);

	// Enemy position is on the other side of the map
	enemyPaddle->SetPosition(0, 0, -40);

	// Ball is in the middle of the map
	Ball->SetPosition(0, 0, -20);
}

void GameWorld::render2DImages(SDL_Texture* Image, SDL_Rect Location, bool Update)
{
	// Tell it where to render
	SDL_QueryTexture(Image, NULL, NULL, &Location.w, &Location.h);
	SDL_RenderCopy(renderer, Image, NULL, &Location);

	// Needs to be updated by the last image in the frame sequence.
	// TODO: Do this automatically. This isn't necessary since I could do this in my own draw call...
	if (Update)
	{
		// Get Renderer
		SDL_RenderPresent(renderer);

		// Clear the entire screen to our selected colour
		SDL_RenderClear(renderer);
	}
}

SDL_Texture* GameWorld::createImage(std::string filename)
{
	// Create a temp texture.
	// TODO: WHY IS THIS NOT BEING DELETED
	SDL_Texture* temp = nullptr;
	
	// Set the file to the surface
	surface = SDL_LoadBMP(filename.c_str());

	// Always do the check for the most likely first to save time
	if (surface)
	{
		// Transparent for all of the cyan images
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0xFF, 0xff));
		
		temp = SDL_CreateTextureFromSurface(renderer, surface);
		
		SDL_FreeSurface(surface);
		return temp;
	}
	// If not - Lets tell the user
	std::cout << "This hasn't worked m'dear" << std::endl;

	// Quit!
	SDL_Quit();
	return temp;
}

void GameWorld::keyInputHandler()
{
	// This is less important since this is a controller based game only.
	// Unless "Rumble Keyboards" become a thing in the future. 
	switch (incomingEvent.type)
	{
	case SDL_QUIT:
		go = false;
		break;
	// When Key is pressed
	case SDL_KEYDOWN:
		switch (incomingEvent.key.keysym.sym)
		{

		// Escape - Exit the game
		case SDLK_ESCAPE:
			go = false;
			break;

		// A - Spin Left
		case SDLK_a:
			spinAmount -= SPIN_ACCELERATION;
			break;

		// D - Spin Right
		case SDLK_d:
			spinAmount += SPIN_ACCELERATION;
			break;
		}
		break;
	}
}

void GameWorld::updateObjects()
{
	// Update our world
	current = SDL_GetTicks();
	deltaTime = (float)(current - lastTime) / 1000.0f;

	// Now that we've done this we can use the current time as the next frame's previous time
	lastTime = current;

	// Send the Velocity to the Rocket!
	Ball->updateBall(ballDirectionX, ballDirectionY, enemyPaddle->GetModelPosition(), 
					 playerRocket->GetModelPosition(), scoreRight, scoreLeft);

	// Set lose every frame to see if the player has lost pong?
	// TODO: This does work and the logic is there however all these checks are a little much.
	Lose = Ball->resetBall();

	// When you have lost then increment the score (One way because the AI is impossible to beat.)
	if (Lose)
	{
		score++;
		Lose = false;
	}

	// When the player has lost 3 goals then stop the game playing (With 'go = false' taking them out the game)
	if (score == 2)
	{
		go = false;
		scoreReset = true;
	}

	// Match the position of the enemy paddle to where the ball is
	enemyPaddle->enemyVsBall(Ball->GetModelPosition());

	// Set the camera to follow the Rocket
	camera->followRocket(playerRocket->GetModelPosition());

	// Specify the colour to clear the framebuffer to
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// This writes the above colour to the colour part of the framebuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GameWorld::drawObjects()
{
	// Update the Camera
	camera->update();

	// Update the Objects
	playerRocket->Draw(camera->getView(), camera->getProjection());
	enemyPaddle->Draw(camera->getView(), camera->getProjection());

	// If up. The ball is drawn
	if (xboxController->isUpPressed())
	{
		isDraw = true;
	}

	// If down. The ball is not drawn
	if (xboxController->isDownPressed())
	{
		isDraw = false;
	}

	// If start. Go to the survey screen
	if (xboxController->isStartPressed())
	{
		go = false;
		scoreReset = true;
	}

	// if we are drawing the ball, draw it! 
	if (isDraw)
	{
		Ball->Draw(camera->getView(), camera->getProjection());
	}
	
	// The map bounds need to be drawn
	Terrain->Draw(camera->getView(), camera->getProjection());

	// Double Buffering Stuff Yes!
	SDL_GL_SwapWindow(window);

	// Limit to 60FPS
	if (deltaTime < (1.0f / 120.0f))
	{
		SDL_Delay((unsigned int)(((1.0f / 120.0f) - deltaTime)*1000.0f));
	}
}

bool GameWorld::updateGame(uint8_t Score)
{
	// Reset the score at the start of the frame if necessary 
	if (scoreReset)
	{
		score = Score;
		go = true;
	}
	// While the game loop is going
	while (go)
	{
		// Use Xbox Input if availible
		if (xboxControllerConnected)
		{
			// Move the paddle based on the ball position
			playerRocket->MovePaddle(xboxController->movement(deltaTime));

			//  Set the vibration based on the postion of the ball 
			currentVibrationValue = Ball->getDistanceBalltoPlayer(playerRocket->GetModelPosition());

			// is the ball.... left? 
			// TODO: What? 
			leftSide = Ball->isLeft(playerRocket->GetModelPosition());

			if (leftSide == 'L')
			{
				xboxController->vibrate(currentVibrationValue * 2, 0);
			}
			if (leftSide == 'R')
			{
				xboxController->vibrate(0, currentVibrationValue * 2);
			}
			if (leftSide == 'M')
			{
				xboxController->vibrate(0.01f, 0.01f);
			}

			while (SDL_PollEvent(&incomingEvent))
			{
				switch (incomingEvent.type)
				{
				case SDL_QUIT:
					go = false;
					break;
				}
			}
		}
		else
		{
			// Keyboard input 
			while (SDL_PollEvent(&incomingEvent))
			{
				keyInputHandler();
			}
		}
		
		// Update and Draw the Objects
		updateObjects();
		drawObjects();
	}
	// Exit out
	return false;
}
