#include "GameWorld.h"
#include "Menu.h"
int main(int, char**)
{
	// Initialise Menu
	Menu* gameMenu = new Menu();

	// Go to the Menu Input Handler
	gameMenu->inputHandler();

	// Go to Selected State
	gameMenu->stateSelect();

	// Delete Menu
	delete gameMenu;

	// Exit Game
	return 0;
}