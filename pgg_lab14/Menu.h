/*!
*  \brief     Menu Class.
*  \details   This class is to create and handle the Menu
*  \author    James Robertson
*  \version   1.0a
*  \date      2015
*  \copyright GNU Public License.
*/
#pragma once

#include "GameWorld.h"
#include "FeedbackForm.h"

class Menu
{
public:
	/// Constructors and Destructors
	Menu();
	~Menu();

	/// Setup the images before using
	void setupImages();

	/// Limit how far the menu can go
	void limitMenuSelection(int8_t min, int8_t max);

	/// Handle the input for the menu
	void inputHandler();

	/// Change to the image that is selected
	void changetoSelectedImage();

	/// Go to the selected state
	void stateSelect();

private:
	// Game World Pointer
	GameWorld* world;

	// Incoming input events
	SDL_Event incomingEvent;

	// Background Image
	SDL_Texture * Background;
	SDL_Rect BackgroundPosition;

	uint16_t timer = 0;

	// Title Image
	SDL_Texture * Title;
	SDL_Rect TitlePosition;

	// Button Image (Both selected and unselected)
	SDL_Texture* ButtonUnselected;
	SDL_Texture* ButtonSelected;
	SDL_Rect ButtonPosition;

	// Button Image (Both selected and unselected)
	SDL_Texture* Button2Unselected;
	SDL_Texture* Button2Selected;
	SDL_Rect Button2Position;

	// Button Image (Both selected and unselected)
	SDL_Texture* Button3Unselected;
	SDL_Texture* Button3Selected;
	SDL_Rect Button3Position;

	// Index of selection
	int8_t selectedButtonIndex = 1;
	int8_t stateSelector = 0;
	uint8_t SetScore = 0;

	int16_t TimeLimit;

	Controller *XboxOneController;

	bool NotFullQuitYet = false;

	FeedbackForm *FeedbackInterface;
	bool CheckConnection;
};