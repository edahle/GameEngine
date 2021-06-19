#pragma once

#include <glew.h>

class Material
{
public:
	Material();
	Material(GLfloat specularIntensity, GLfloat shininess);

	void useMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);

	~Material();

private:
	GLfloat specularIntensity;
	GLfloat shininess;
};

