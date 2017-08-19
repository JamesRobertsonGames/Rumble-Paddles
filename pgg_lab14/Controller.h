/*!
*  \brief     Camera Class.
*  \details   This class is to Handle XINPUT and the Xbox Controller
*  \author    James Robertson
*  \version   1.0a
*  \date      2015
*  \copyright GNU Public License.
*/

#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <stdint.h>
#include <SDL.h>
#include <iostream>

#define XINPUT_GAMEPAD_LEFT_SHOULDER    0x0100
#define XINPUT_GAMEPAD_RIGHT_SHOULDER   0x0200
#define XINPUT_GAMEPAD_DPAD_UP			0x0001
#define XINPUT_GAMEPAD_DPAD_DOWN		0x0002
#define XINPUT_GAMEPAD_DPAD_LEFT		0x0004
#define XINPUT_GAMEPAD_DPAD_RIGHT		0x0008

class Controller
{
public:
	/// Constructor and Destructor
	Controller();
	~Controller();

	/// Check if the controller is connected
	bool isControllerConnected();

	/// Vibrate the controller
	void vibrate(float leftMotor, float rightMotor);

	/// Move the player
	float movement(float delta);

	/// Is the Up Button 
	bool isUpPressed();

	/// Is the Up Button 
	bool isDownPressed();

	/// Is the Up Button 
	bool isLeftPressed();

	/// Is the Up Button 
	bool isRightPressed();

	/// Is the Up Button 
	bool isAPressed();

	bool isStartPressed();

private:
	// States for the controller
	XINPUT_STATE *controllerState;
	XINPUT_VIBRATION *vibrationState;

	// Store if the controller is connected
	DWORD dwResult;
	DWORD dvResult;

};
