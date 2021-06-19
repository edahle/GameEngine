#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();
	DirectionalLight(
		GLfloat red,
		GLfloat green,
		GLfloat blue,
		GLfloat ambientIntensity,
		GLfloat diffuseIntensity,
		glm::vec3 direction
		
	);
	void useLight(
		GLuint ambientIntensityLocation,
		GLuint colorLocation,
		GLuint diffuseIntensityLocation,
		GLuint directionLocation
	);
	~DirectionalLight();

private:
	glm::vec3 direction;
};

