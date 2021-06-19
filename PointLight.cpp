
#include "PointLight.h"

PointLight::PointLight() : Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f;
	linear = 0.0f;
	exponent = 0.0f;
}

PointLight::PointLight(
	GLfloat red,
	GLfloat green,
	GLfloat blue,
	GLfloat ambientIntensity,
	GLfloat diffuseIntensity,
	glm::vec3 position,
	GLfloat constant,
	GLfloat linear,
	GLfloat exponent
) : Light(red, green, blue, ambientIntensity, diffuseIntensity)
{
	this->position = position;
	this->constant = constant;
	this->linear = linear;
	this->exponent = exponent;
}

void PointLight::useLight(
	GLuint ambientIntensityLocation,
	GLuint colorLocation,
	GLuint diffuseIntensityLocation,
	GLuint positionLocation,
	GLuint constantLocation,
	GLuint linearLocation,
	GLuint exponentLocation
)
{
	glUniform3f(colorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
}

PointLight::~PointLight()
{
}
