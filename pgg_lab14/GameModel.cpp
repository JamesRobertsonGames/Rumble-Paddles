/*!
*  \brief     GameModel Class.
*  \details   This class is to create and update the GameModel
*  \author    James Robertson
*  \version   1.0a
*  \date      2015
*  \copyright GNU Public License.
*/

#include "GameModel.h"

#include <iostream>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

void vec2_norm(float& x, float &y) {
	// sets a vectors length to 1 (which means that x + y == 1)
	float length = sqrt((x * x) + (y * y));
	if (length != 0.0f) {
		length = 1.0f / length;
		x *= length;
		y *= length;
	}
}

bool CheckShaderCompiled( GLint shader )
{
	// Check if it is compiled
	GLint compiled;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
	if ( !compiled )
	{
		// Store Error in Log
		GLsizei len;
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &len );

		GLchar* log = new GLchar[len+1];
		glGetShaderInfoLog( shader, len, &len, log );

		// Tell user of error
		std::cerr << "ERROR: Shader compilation failed: " << log << std::endl;

		// Delete Memory
		delete [] log;

		return false;
	}
	return true;
}

GameModel::GameModel(std::string objFileName)
{
	// Initialise variables
	_VAO = 0;
	_program = 0;
	_shaderModelMatLocation = _shaderViewMatLocation = _shaderProjMatLocation = 0;
	_numVertices = 0;

	// Load Object
	objLoader.Load(objFileName);

	// Create the model
	InitialiseVAO();

	// Create the shaders
	InitialiseShaders();
}

GameModel::~GameModel()
{
	// TODO: destroy VAO, shaders etc
}

void GameModel::InitialiseVAO()
{
	// Creates one VAO and binds it
	glGenVertexArrays( 1, &_VAO );
	glBindVertexArray( _VAO );

	// Finds vertex amount
	_numVertices = objLoader.GetMeshVertices().size() / 3;

	// Variable for storing a VBO
	GLuint positionBuffer = 0;

	// Create a generic 'buffer'
	glGenBuffers(1, &positionBuffer);

	// Tell OpenGL that we want to activate the buffer and that it's a VBO
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);

	// Draw Verts
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _numVertices * 3, &objLoader.GetMeshVertices()[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray(0);

	// Variable for storing a VBO
	GLuint normalBuffer = 0;

	// Create a generic 'buffer'
	glGenBuffers(1, &normalBuffer);

	// Tell OpenGL that we want to activate the buffer and that it's a VBO
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);

	// With this buffer active, we can now send our data to OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _numVertices * 3, &objLoader.GetMeshNormals()[0], GL_STATIC_DRAW);

	// This tells OpenGL how we link the vertex data to the shader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray(1);
	
	// Bind the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray( 0 );

	// Technically we can do this, because the enabled / disabled state is stored in the VAO
	glDisableVertexAttribArray(0);
}

void GameModel::InitialiseShaders()
{
	// This is the vertex shader
	const GLchar *vShaderText = "#version 430 core\n\
						 layout(location = 0) in vec4 vPosition;\n\
						 layout(location = 1) in vec3 vNormalIn;\n\
						 \n\
						 uniform mat4 modelMat;\n\
						 uniform mat4 invModelMat;\n\
						 uniform mat4 viewMat;\n\
						 uniform mat4 projMat;\n\
						 \n\
						 uniform vec4 worldSpaceLightPos = {1000,400,-3000,0};\n\
						 \n\
						 out vec3 vNormalV;\n\
						 out vec3 lightDirV;\n\
						 \n\
						 void main()\n\
						 {\n\
								gl_Position = projMat * viewMat * modelMat * vPosition;\n\
								\n\
								vec4 eyeSpaceVertPos = viewMat * modelMat * vPosition;\n\
								vec4 eyeSpaceLightPos = viewMat * worldSpaceLightPos;\n\
								\n\
								lightDirV =  normalize( vec3(eyeSpaceLightPos) - vec3(eyeSpaceVertPos) );\n\
								\n\
								vNormalV = mat3(viewMat * modelMat) * vNormalIn;\n\
						 }";

	// This is the fragment shader
	const GLchar *fShaderText = "#version 430 core\n\
								in vec3 lightDirV;\n\
								in vec3 vNormalV;\n\
								\n\
								uniform vec3 lightColour = {1,1,1};\n\
								uniform vec3 emissiveColour = {0.0,0,0.1};\n\
								uniform vec3 ambientColour  = {1.0f,1.0f,1.0f};\n\
								uniform vec3 diffuseColour  = {0.2f,0.6f,0.5f};\n\
								uniform vec3 specularColour = {0.3f,0.3f,0.3f};\n\
								uniform float shininess     = 10.0f;\n\
								uniform float alpha         = 2.0f;\n\
								\n\
								out vec4 fragColour;\n\
								\n\
								void main()\n\
								{\n\
									vec3 lightDir = normalize( lightDirV );\n\
									vec3 vNormal = normalize( vNormalV );\n\
									\n\
										vec3 diffuse = diffuseColour * lightColour * max( dot( vNormal, lightDir ), 0);\n\
										\n\
										fragColour = vec4( emissiveColour + ambientColour + diffuse, alpha);\n\
								}";

	// The 'program' stores the shaders
	_program = glCreateProgram();

	// Create the vertex shader
	GLuint vShader = glCreateShader( GL_VERTEX_SHADER );
	// Give GL the source for it
	glShaderSource( vShader, 1, &vShaderText, NULL );
	// Compile the shader
	glCompileShader( vShader );
	// Check it compiled and give useful output if it didn't work!
	if( !CheckShaderCompiled( vShader ) )
	{
		return;
	}
	// This links the shader to the program
	glAttachShader( _program, vShader );

	// Same for the fragment shader
	GLuint fShader = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fShader, 1, &fShaderText, NULL );
	glCompileShader( fShader );
	if( !CheckShaderCompiled( fShader ) )
	{
		return ;
	}
	glAttachShader( _program, fShader );

	// This makes sure the vertex and fragment shaders connect together
	glLinkProgram( _program );
	// Check this worked
	GLint linked;
	glGetProgramiv( _program, GL_LINK_STATUS, &linked );
	if ( !linked )
	{
		GLsizei len;
		glGetProgramiv( _program, GL_INFO_LOG_LENGTH, &len );

		GLchar* log = new GLchar[len+1];
		glGetProgramInfoLog( _program, len, &len, log );
		std::cerr << "ERROR: Shader linking failed: " << log << std::endl;
		delete [] log;

		return;
	}

	// We need to get the location of the uniforms in the shaders
	// This is so that we can send the values to them from the application
	// We do this in the following way: 
	_shaderModelMatLocation = glGetUniformLocation( _program, "modelMat" );
	_shaderViewMatLocation = glGetUniformLocation( _program, "viewMat" );
	_shaderProjMatLocation = glGetUniformLocation( _program, "projMat" );

}

void GameModel::Update( float deltaTs )
{

}

void GameModel::Draw(glm::mat4& viewMatrix, glm::mat4& projMatrix)
{
	// Next, we translate this matrix according to the object's _position vector:
	_modelMatrix = glm::translate(glm::mat4(1.0f), _position);

	// Next, we rotate this matrix in the x-axis by the object's x-rotation:
	_modelMatrix = glm::rotate(_modelMatrix, _rotation.x, glm::vec3(1, 0, 0));
	// Next, we rotate this matrix in the y-axis by the object's y-rotation:
	_modelMatrix = glm::rotate(_modelMatrix, _rotation.y, glm::vec3(0, 1, 0));
	// Next, we rotate this matrix in the z-axis by the object's z-rotation:
	_modelMatrix = glm::rotate(_modelMatrix, _rotation.z, glm::vec3(0, 0, 1));
	// Activate the shader program
	glUseProgram( _program );

		// Activate the VAO
		glBindVertexArray( _VAO );

			// Send matrices to the shader as uniforms like this:
			glUniformMatrix4fv(_shaderModelMatLocation, 1, GL_FALSE, glm::value_ptr(_modelMatrix) );
			glUniformMatrix4fv(_shaderViewMatLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix) );
			glUniformMatrix4fv(_shaderProjMatLocation, 1, GL_FALSE, glm::value_ptr(projMatrix) );


			// Tell OpenGL to draw it
			// Must specify the type of geometry to draw and the number of vertices
			glDrawArrays(GL_TRIANGLES, 0, _numVertices);
			
		// Unbind VAO
		glBindVertexArray( 0 );
	
	// Technically we can do this, but it makes no real sense because we must always have a valid shader program to draw geometry
	glUseProgram( 0 );
}

void GameModel::SetForwardVelocity(float velocity, float deltaTime)
{
	_position.z -= velocity * deltaTime;
}

void GameModel::SetSidewaysVelocity(float velocity, float deltaTime)
{
	// Times the velocity by delta to get sideways speed up
	_position.x -= velocity * deltaTime;


}

void GameModel::CollisionCheck(float RowXPosition, float Width)
{

}

void GameModel::SetRoll(float angle, float deltaTime)
{
	// update the rotation angle of our cube
	_rotation.y += deltaTime * angle;
}

void GameModel::SetRotation(float posX, float posY, float posZ)
{
	// Update all of the Coordinates
	_rotation.x = posX; 
	_rotation.y = posY; 
	_rotation.z = posZ;
}

void GameModel::MovePaddle(float Speed)
{
	_position.x += Speed;
	// Set Boundries of the Level

	if (_position.x > 8.f)
		_position.x = 7.9f;
	if (_position.x < -8.f)
		_position.x = -7.9f;
}

void GameModel::updateBall(float ballDirectionX, float ballDirectionY, 
						   glm::vec3 racketPosEnemy, glm::vec3 racketPosPlayer,
						   uint8_t scoreRight, uint8_t scoreLeft)
{
	if (_position.x > 10)
	{
		moveLeft = true;
		moveRight = false;
	}

	if (_position.x < -10)
	{
		moveRight = true;
		moveLeft = false;
	}

	if ((_position.z > 0) && (_position.z < 1) && (_position.x + 5 > racketPosPlayer.x) && (_position.x - 5 < racketPosPlayer.x))
	{
		moveDown = true;
		moveUp = false;
		bounceVariety = ((int)rand() % 3) + 1;
	}

	if ((_position.z < -40) && (_position.z > -41) && (_position.x + 5 > racketPosEnemy.x) && (_position.x - 5 < racketPosEnemy.x))
	{
		moveUp = true;
		moveDown = false;
		bounceVariety = ((int)rand() % 3) + 1;
	}

	if ((moveLeft == true) && (bounceVariety == 1))
	{
		_position.x -= 0.1f;
	}

	if ((moveLeft == true) && (bounceVariety == 2))
	{
		_position.x -= 0.15f;
	}

	if ((moveLeft == true) && (bounceVariety == 3))
	{
		_position.x -= 0.02f;
	}

	if ((moveRight == true) && (bounceVariety == 1))
	{
		_position.x += 0.1f;
	}

	if ((moveRight == true) && (bounceVariety == 2))
	{
		_position.x += 0.15f;
	}

	if ((moveRight == true) && (bounceVariety == 3))
	{
		_position.x += 0.02f;
	}

	if (moveUp)
	{
		_position.z += 0.2f;
	}

	if (moveDown)
	{
		_position.z -= 0.2f;
	}

	//std::cout << "POS - Ball" << std::abs(_position.x - racketPosPlayer.x) << std::endl;
	//std::cout << "POS - Racket" << (float)racketPosPlayer.x << std::endl;
}

void GameModel::enemyVsBall(glm::vec3 ballPosition)
{
	_position.x = ballPosition.x;
}

bool GameModel::resetBall()
{
	if ((_position.z > 10) || (_position.z < -50))
	{
		_position.z = -40;
		std::cout << "Lose";
		return true;
	}
	return false;
}

float GameModel::getDistanceBalltoPlayer(glm::vec3 racketPosPlayer)
{
	float temp = 0.0f;
	temp = std::abs(_position.x - racketPosPlayer.x);
	return temp;
}

char GameModel::isLeft(glm::vec3 racketPosPlayer)
{
	if ((_position.x + 2 > racketPosPlayer.x) && (_position.x - 2 < racketPosPlayer.x))
	{
		return 'M';
	}
	if (_position.x > racketPosPlayer.x)
	{
		return 'R';
	}
	if (_position.x < racketPosPlayer.x)
	{
		return 'L';
	}
	return 'A';
}

