#pragma once

#include <string>
#include <SDL.h>

class Button
{
public:
	Button(uint8_t index, std::string imagePath, std::string Location);
	~Button();
	std::string gotoLink();
	void switchtoHoverImage();
private:


};