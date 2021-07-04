
#include "SpotLight.h"

SpotLight::SpotLight() : PointLight()
{
	direction = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));
	edge = 0.0f;
	processedEdge = cosf(glm::radians(edge));
}

SpotLight::SpotLight(
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
) : PointLight(red, green, blue, ambientIntensity, diffuseIntensity, position, constant, linear, exponent)
{
	this->direction = glm::normalize(direction);
	this->edge = edge;
	this->processedEdge = cosf(glm::radians(edge));
}

void SpotLight::useLight(
	GLuint ambientIntensityLocation,
	GLuint colorLocation,
	GLuint diffuseIntensityLocation,
	GLuint positionLocation,
	GLuint directionLocation,
	GLuint constantLocation,
	GLuint linearLocation,
	GLuint exponentLocation,
	GLuint edgeLocation
)
{
	glUniform3f(colorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
	glUniform1f(edgeLocation, processedEdge);
}

void SpotLight::setFlash(glm::vec3 position, glm::vec3 direction)
{
	this->position = position;
	this->direction = direction;
}

SpotLight::~SpotLight()
{
}