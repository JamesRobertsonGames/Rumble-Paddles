#include "FeedbackForm.h"

FeedbackForm::FeedbackForm(GameWorld *currentworld)
{
	tempWorld = currentworld;

	First = tempWorld->createImage("First.bmp");
	Second = tempWorld->createImage("Second.bmp");
	Third = tempWorld->createImage("Third.bmp");
	Fourth = tempWorld->createImage("Fourth.bmp");

	FirstPosition = { 0,0,0,0 };
	SecondPosition = { 0,0,0,0 };
	ThirdPosition = { 0,0,0,0 };
	FourthPosition = { 0,0,0,0 };

	GameController = new Controller();

	CompletedQuestion[0] = false;
	CompletedQuestion[1] = false;
	CompletedQuestion[2] = false;
	CompletedQuestion[3] = false;

	TimeLimit = 2000;
}

FeedbackForm::~FeedbackForm()
{
	SDL_DestroyTexture(First);
	SDL_DestroyTexture(Second);
	SDL_DestroyTexture(Third);
	SDL_DestroyTexture(Fourth);

	delete GameController;
	delete tempWorld;
}

void FeedbackForm::GetControllerUpdate()
{
	if (GameController->isUpPressed() && TimerofDPAD > TimeLimit)
	{
		TimerofDPAD = 0;
		switch (IndexOfQuestions)
		{
		case 1:
			IndexOfQuestions = 2;
			dataLogFirst = "Was the Direction of Vibration Clear - Excellent";
			break;
		case 2:
			IndexOfQuestions = 3;
			dataLogSecond = "Did Vibration Enhance Gameplay- Excellent";
			break;
		case 3:
			IndexOfQuestions = 4;
			dataLogThird = "Question 3 - Excellent";
			break;
		case 4:
			IndexOfQuestions = 5;
			dataLogFourth = "Question 4";
			break;
		}

	}
	if (GameController->isDownPressed() && TimerofDPAD > TimeLimit)
	{
		TimerofDPAD = 0;
		switch (IndexOfQuestions)
		{
		case 1:
			dataLogFirst = "Was the Direction of Vibration Clear - Playable";
			IndexOfQuestions = 2;
			break;
		case 2:
			dataLogSecond = "Did Vibration Enhance Gameplay - Great Enhancement";
			IndexOfQuestions = 3;
			break;
		case 3:
			dataLogThird = "Question 3 - Excellent";
			IndexOfQuestions = 4;
			break;
		case 4:
			dataLogFourth = "Question 4";
			IndexOfQuestions = 5;
			break;
		}
	}
	if (GameController->isRightPressed() && TimerofDPAD > TimeLimit)
	{
		TimerofDPAD = 0;

		switch (IndexOfQuestions)
		{
		case 1:
			IndexOfQuestions = 2;
			dataLogFirst = "Was the Direction of Vibration Clear - Vibration Direction not Clear";
			break;
		case 2:
			IndexOfQuestions = 3;
			dataLogSecond = "Did Vibration Enhance Gameplay - Slight Enhancement";
			break;
		case 3:
			IndexOfQuestions = 4;
			dataLogThird = "Question 3 - Excellent";
			break;
		case 4:
			IndexOfQuestions = 5;
			dataLogFourth = "Question 4";
			break;
		}
	}
	if (GameController->isLeftPressed() && TimerofDPAD > TimeLimit)
	{
		// Question Answer Written to a File
		// * Case - which question we on
		// * Write to string ready for export
		TimerofDPAD = 0;

		switch (IndexOfQuestions)
		{
		case 1:
			IndexOfQuestions = 2;
			dataLogFirst = "Was the Direction of Vibration Clear- Could not Determine Vibration Detection";
			break;
		case 2:
			IndexOfQuestions = 3;
			dataLogSecond = "Did Vibration Enhance Gameplay - No Enhancement";
			break;
		case 3:
			IndexOfQuestions = 4;
			dataLogThird = "Question 3 - Excellent";
			break;
		case 4:
			IndexOfQuestions = 5;
			dataLogFourth = "Question 4";
			break;
		}
	}
}

void FeedbackForm::Update()
{
	SDL_Event incomingEvent;

	bool stop = false;

	while (stop == false)
	{
		while (SDL_PollEvent(&incomingEvent))
		{
			switch (incomingEvent.type)
			{
			case SDL_QUIT:
				break;
				// When Key is pressed
			}
		}
		TimerofDPAD++;
		switch (IndexOfQuestions)
		{
		case 1:
			tempWorld->render2DImages(First, FirstPosition, true);
			break;
		case 2:
			tempWorld->render2DImages(Second, SecondPosition, true);
			break;
		case 3:
			tempWorld->render2DImages(Third, ThirdPosition, true);
			break;
		case 4:
			tempWorld->render2DImages(Fourth, FourthPosition, true);
			break;
		case 5:
			stop = true;
		}
		GetControllerUpdate();
	}
	LogUserInput();
	stop = false;
	IndexOfQuestions = 1;
}

void FeedbackForm::LogUserInput()
{
	FinalQuestionaireLog = dataLogFirst + dataLogSecond + dataLogThird + dataLogFourth;
	// Print out the data to a TXT file and offer storage for later analysis

	// File Handling
	std::ostringstream Filename;
	Filename << "ExportData/" << "Questionaire" << CountofQuestionaire << ".txt";
	CountofQuestionaire++;
	std::cout << "Enter The Filename for this Questionaire: ";
	QuestionDataSet.open(Filename.str());

	QuestionDataSet << dataLogFirst << std::endl << dataLogSecond << std::endl << dataLogThird << std::endl << dataLogFourth << std::endl;
	QuestionDataSet.close();
}