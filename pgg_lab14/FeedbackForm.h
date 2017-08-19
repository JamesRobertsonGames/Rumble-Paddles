#pragma once
#include "GameWorld.h"
// basic file operations
#include <iostream>

#include <fstream>
#include <vector>
#include <sstream>

class FeedbackForm
{
public:
	FeedbackForm(GameWorld *currentworld);
	~FeedbackForm();

	void GetControllerUpdate();
	void Update();
	void LogUserInput();

private:
	// Temparary Storage Strings
	std::string dataLogFirst;
	std::string dataLogSecond;
	std::string dataLogThird;
	std::string dataLogFourth;

	// Final Storage String
	std::string FinalQuestionaireLog;

	// Controller and Gameworld
	Controller *GameController;
	GameWorld *tempWorld;

	// Textures
	SDL_Texture *First;
	SDL_Texture *Second;
	SDL_Texture *Third;
	SDL_Texture *Fourth;

	// Positions
	SDL_Rect FirstPosition;
	SDL_Rect SecondPosition;
	SDL_Rect ThirdPosition;
	SDL_Rect FourthPosition;

	// Indexs
	uint8_t IndexOfQuestions = 1;
	uint8_t IndexOfAnswers = 0;

	int CountofQuestionaire = 1;

	uint16_t TimerofDPAD = 0;

	int16_t TimeLimit;

	std::ofstream QuestionDataSet;
	bool CompletedQuestion[4];
};