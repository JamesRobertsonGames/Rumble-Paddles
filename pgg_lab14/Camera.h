/*!
*  \brief     Camera Class.
*  \details   This class is to Move and Project the Camera
*  \author    James Robertson
*  \version   1.0a
*  \date      2015
*  \copyright GNU Public License.
*/

#pragma once 
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>


class Camera
{
public:
	/// Constructor and Destructor
	Camera();
	~Camera();

	/// Position Setters
	void setAbsolutePosition(glm::vec3 Position);
	void followRocket(glm::vec3 RocketPosition);
	void moveCamera(glm::vec3 Velocity);

	/// Update Camera
	void update();

	/// Get Projection and View
	glm::mat4 getProjection() { return projection; }
	glm::mat4 getView() { return view; }

private:
	glm::vec3 cameraPosition;
	glm::mat4 projection;
	glm::mat4 view;
};