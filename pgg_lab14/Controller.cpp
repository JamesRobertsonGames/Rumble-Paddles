/*!
*  \brief     Controller Class.
*  \details   This class is to Handle XINPUT and the Xbox Controller
*  \author    James Robertson
*  \version   1.0a
*  \date      2015
*  \copyright GNU Public License.
*/
#include "Controller.h"

Controller::Controller()
{
	// Initialise pointers
	controllerState = new XINPUT_STATE();
	vibrationState = new XINPUT_VIBRATION();
}

Controller::~Controller()
{
	delete controllerState;
	delete vibrationState;
}

bool Controller::isControllerConnected()
{
	// Zeros all memory values in the controller
	ZeroMemory(controllerState, sizeof(XINPUT_STATE));

	// Get the current state of controller 1
	dwResult = XInputGetState(0, controllerState);

	// Zeros all the memory values in the vibration modual
	ZeroMemory(vibrationState, sizeof(XINPUT_VIBRATION));

	// Set the state of the vibration moduel
	dvResult = XInputSetState(0, vibrationState);

	// If it worked return true if not then its not connected
	if (dwResult == ERROR_SUCCESS && dvResult == ERROR_SUCCESS)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Controller::vibrate(float leftMotor, float rightMotor)
{
	// Create a new Vibraton 
	XINPUT_VIBRATION Vibration;

	// Set first num bytes to block of memory (I Have no clue)
	memset(&Vibration, 0, sizeof(XINPUT_VIBRATION));

	// Vibrate amount
	WORD leftVib = (WORD)(leftMotor*65535.0f);
	WORD rightVib = (WORD)(rightMotor*65535.0f);

	// Set the Vibration Values
	Vibration.wLeftMotorSpeed = leftVib;
	Vibration.wRightMotorSpeed = rightVib;

	// Vibrate the controller
	XInputSetState((int)0, &Vibration);
}

float Controller::movement(float delta)
{
	// Movement for the character
	float movement = 0.0f;

	// Get all the current states of controller 1
	XInputGetState(0, controllerState);

	// LB is Pressed...
	if (controllerState->Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
	{
		// Go Left and Vibrate
		movement = -0.3f;
	}

	// RB is Pressed...
	if (controllerState->Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
	{
		// Go Right and Vibrate
		movement = 0.3f;
	}

	// LB is Pressed...
	if (controllerState->Gamepad.sThumbLX < -5000)
	{
		// Go Left and Vibrate
		movement += controllerState->Gamepad.sThumbLX * delta / 2000.0f;
	}

	// RB is Pressed...
	if (controllerState->Gamepad.sThumbLX > 5000)
	{
		// Go Left and Vibrate
		movement += controllerState->Gamepad.sThumbLX * delta / 2000.0f;
	}

	// Retrun speed of the ship
	return movement;
}

bool Controller::isUpPressed()
{
	// Get all the current states of controller 1
	XInputGetState(0, controllerState);

	if (controllerState->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
	{
		return true;
	}

	return false;
}

bool Controller::isDownPressed()
{
	// Get all the current states of controller 1
	XInputGetState(0, controllerState);
	if (controllerState->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
	{
		return true;
	}

	return false;
}

bool Controller::isRightPressed()
{
	// Get all the current states of controller 1
	XInputGetState(0, controllerState);
	if (controllerState->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		return true;
	}

	return false;
}

bool Controller::isLeftPressed()
{
	// Get all the current states of controller 1
	XInputGetState(0, controllerState);
	if (controllerState->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
	{
		return true;
	}

	return false;
}


bool Controller::isAPressed()
{
	// Get all the current states of controller 1
	XInputGetState(0, controllerState);
	if (controllerState->Gamepad.wButtons & XINPUT_GAMEPAD_A)
	{
		return true;
	}

	return false;
}

bool Controller::isStartPressed()
{
	// Get all the current states of controller 1
	XInputGetState(0, controllerState);
	if (controllerState->Gamepad.wButtons & XINPUT_GAMEPAD_START)
	{
		return true;
	}

	return false;
}