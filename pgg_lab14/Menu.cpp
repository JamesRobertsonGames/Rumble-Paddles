/*!
*  \brief     Menu Class.
*  \details   This class is to create and handle the Menu
*  \author    James Robertson
*  \version   1.0a
*  \date      2015
*  \copyright GNU Public License.
*/

#include "Menu.h"

Menu::Menu()
{
	// Make New World and Setup Images
	world = new GameWorld();
	FeedbackInterface = new FeedbackForm(world);

	setupImages();
	XboxOneController = new Controller();
	CheckConnection = true;
	TimeLimit = 100;
}

void Menu::setupImages()
{
	// Load images in
	Background = world->createImage("MenuBackground.bmp");
	Title = world->createImage("DestinationOrigin.bmp");
	ButtonSelected = world->createImage("NewGameSelected.bmp");
	ButtonUnselected = world->createImage("NewGameUnselected.bmp");
	Button2Selected = world->createImage("OptionsSelected.bmp");
	Button2Unselected = world->createImage("OptionsUnselected.bmp");
	Button3Selected = world->createImage("ExitSelected.bmp");
	Button3Unselected = world->createImage("ExitUnselected.bmp");

	// Initialise the positions of the images
	// Name 			   |X||Y||W||Z|
	BackgroundPosition	= { 0, 0, 0, 0 };
	TitlePosition		= { 300, 0, 0, 0 };	
	ButtonPosition		= { 350, 170, 0, 0 };
	Button2Position		= { 413, 270, 0, 0 };
	Button3Position		= { 413, 390, 0, 0 };
}

Menu::~Menu()
{
	delete world;

	delete XboxOneController;

	SDL_DestroyTexture(Title);
	SDL_DestroyTexture(ButtonUnselected);
	SDL_DestroyTexture(ButtonSelected);
	SDL_DestroyTexture(Button2Unselected);
	SDL_DestroyTexture(Button2Selected);
	SDL_DestroyTexture(Button3Unselected);
	SDL_DestroyTexture(Button3Selected);
}

void Menu::limitMenuSelection(int8_t min, int8_t max)
{
	// Limit the menu selection in between a certain range.
	if (selectedButtonIndex < min)
		selectedButtonIndex = min;
	if (selectedButtonIndex > max)
		selectedButtonIndex = max;
}

void Menu::inputHandler()
{
	while (stateSelector == 0)
	{
		// Ensures that the player will not go out of range
		limitMenuSelection(1, 3);
		
		// Render the Menu
		//					 |Image Name|      |Position of Image| |Clear Screen|
		world->render2DImages(Background,		BackgroundPosition, false);
		world->render2DImages(Title,			TitlePosition,		false);
		changetoSelectedImage();
		
		if (CheckConnection)
		{
			timer++;
			if (XboxOneController->isUpPressed() && timer > TimeLimit)
			{
				selectedButtonIndex--;
				timer = 0;
			}

			if (XboxOneController->isDownPressed() && timer > TimeLimit)
			{
				selectedButtonIndex++;
				timer = 0;
			}

			if (XboxOneController->isAPressed() && timer > TimeLimit)
			{
				stateSelector = selectedButtonIndex;
				timer = 0;
			}

			while (SDL_PollEvent(&incomingEvent))
			{
				switch (incomingEvent.type)
				{
				case SDL_QUIT:
					break;
					// When Key is pressed
				}
			}
		}
		else
		{
	
		// Poll the incoming event
		while (SDL_PollEvent(&incomingEvent))
		{
			switch (incomingEvent.type)
			{
			// Quit the game safely
			case SDL_QUIT:
				stateSelector = 3;
				break;

			// When the Key is down
			case SDL_KEYDOWN:
				switch (incomingEvent.key.keysym.sym)
				{

				// Exit game
				case SDLK_ESCAPE:
					stateSelector = 3;
					break;

				// Go down the Menu
				case SDLK_DOWN:
					selectedButtonIndex++;
					break;

				// Go up the Menu
				case SDLK_UP:
					selectedButtonIndex--;
					break;

				// Select the item that is selected
				case SDLK_SPACE:
					stateSelector = selectedButtonIndex;
				}
				break;
			}
		}
		}
	}
}

void Menu::changetoSelectedImage()
{
	// If one is Highlighted.
	if (selectedButtonIndex == 1)
	{
		world->render2DImages(ButtonSelected, ButtonPosition, false);
	}
	// If one is NOT Highlighted.
	else
	{
		world->render2DImages(ButtonUnselected, ButtonPosition, false);
	}
	// If two is Highlighted.
	if (selectedButtonIndex == 2)
	{
		world->render2DImages(Button2Selected, Button2Position, false);
	}
	// If two is NOT Highlighted.
	else
	{
		world->render2DImages(Button2Unselected, Button2Position, false);
	}
	// If three is Highlighted.
	if (selectedButtonIndex == 3)
	{
		world->render2DImages(Button3Selected, Button3Position, true);
	}
	// If three is NOT Highlighted.
	else
	{
		world->render2DImages(Button3Unselected, Button3Position, true);
	}
}
void Menu::stateSelect()
{
	while (!NotFullQuitYet)
	{
		// Go to Game world if Quit is not called
		if (stateSelector != 3)
		{
			// Update The Game World
			world->updateGame(SetScore);
		}
		FeedbackInterface->Update();
	}
}
