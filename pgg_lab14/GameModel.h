/*!
*  \brief     GameModel Class.
*  \details   This class is to create and update the GameModel
*  \author    James Robertson
*  \version   1.0a
*  \date      2015
*  \copyright GNU Public License.
*/

#pragma once
#include <glm.hpp>
#include <SDL.h>
#include <string>
#include "glew.h"
#include "ObjLoader.h"

///////__declspec(align(16))

/// Class to store and display a model
class GameModel
{
public:

	/// Constructor calls InitialiseVAO and InitialiseShaders
	GameModel(std::string objFileName);
	~GameModel();

	/// Loads object model into OpenGL
	void InitialiseVAO();

	/// Loads shaders for the object into OpenGL
	void InitialiseShaders();

	/// Currently just updates rotation to make the model rotate
	void Update( float deltaTs );

	/// Draws object using the given camera view and projection matrices
	void Draw(glm::mat4& viewMatrix, glm::mat4& projMatrix);

	/// For setting the position of the model
	void SetPosition( float posX, float posY, float posZ ) {_position.x = posX; _position.y = posY; _position.z = posZ;}

	void SetRotation(float posX, float posY, float posZ);
	
	void SetForwardVelocity(float velocity, float deltaTime);

	void SetSidewaysVelocity(float velocity, float deltaTime);

	void CollisionCheck(float RowXPosition, float Width);

	void SetRoll(float angle, float deltaTime);

	glm::vec3 GetModelPosition() {	return _position; }

	void MovePaddle(float Speed);

	void updateBall(float ballDirectionX, float ballDirectionY, glm::vec3 racketPosEnemy, glm::vec3 racketPosPlayer,uint8_t scoreRight, uint8_t scoreLeft);

	void enemyVsBall(glm::vec3 ballPosition);

	bool resetBall();

	float getDistanceBalltoPlayer(glm::vec3 racketPosPlayer);

	char isLeft(glm::vec3 racketPosPlayer);
protected:

	ObjLoader objLoader;

	/// Object position vector
	glm::vec3 _position;

	/// Euler angles for rotation
	glm::vec3 _rotation;

	/// Vertex Array Object for model in OpenGL
	GLuint _VAO;

	/// Shader program
	GLuint _program;

	/// Uniform locations
	GLint _shaderModelMatLocation, _shaderViewMatLocation, _shaderProjMatLocation;

	/// Object's model matrix
	/// This is rebuilt in the update function
	glm::mat4 _modelMatrix;

	/// Number of vertices in the model
	uint64_t _numVertices;
	uint16_t numberOfTries = 0;

	GLuint diffuseTexID;
	GLuint normalTexID;

private:
	void TextureInit();

	float ballSpeed = 1.0f;

	GLuint positionBuffer;
	GLuint normalBuffer;
	GLuint texCoordBuffer;
	GLuint tangentBuffer;
	GLuint biTangentBuffer;

	bool moveLeft = true;
	bool moveRight = false;
	bool moveUp = true;
	bool moveDown = false;

	int8_t bounceVariety = 5;
	//SDL_Surface* surface;

	std::string ReadFile(std::string fileName);
};

