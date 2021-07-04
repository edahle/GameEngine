#pragma once
#include "PointLight.h"
class SpotLight :
    public PointLight
{
public:
	SpotLight();
	SpotLight(
		GLfloat red,
		GLfloat green,
		GLfloat blue,
		GLfloat ambientIntensity,
		GLfloat diffuseIntensity,
		glm::vec3 position,
		glm::vec3 direction,
		GLfloat constant,
		GLfloat linear,
		GLfloat exponent,
		GLfloat edge
	);
	void useLight(
		GLuint ambientIntensityLocation,
		GLuint colorLocation,
		GLuint diffuseIntensityLocation,
		GLuint positionLocation,
		GLuint directionLocation,
		GLuint constantLocation,
		GLuint linearLocation,
		GLuint exponentLocation,
		GLuint edgeLocation
	);

	void setFlash(glm::vec3 position, glm::vec3 direction);

	~SpotLight();

private:
	glm::vec3 direction;

	GLfloat edge, processedEdge;
};
