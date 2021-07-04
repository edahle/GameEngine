#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <glew.h>

#include "Constants.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class Shader
{
public:
	Shader();

	void createFromString(const char* vertex, const char* fragment);
	void createFromFiles(const char* vertex, const char* fragment);

	std::string readFile(const char* fileLocation);

	GLuint getProjectionLocation();
	GLuint getModelLocation();
	GLuint getViewLocation();
	GLuint getAmbientIntensityLocation();
	GLuint getAmbientIntensityColorLocation();
	GLuint getdiffuseIntensityLocation();
	GLuint getDirectionLocation();
	GLuint getSpecularIntensityLocation();
	GLuint getShininessLocation();
	GLuint getEyePositionLocation();

	void setDirectionalLight(DirectionalLight* directionalLight);
	void setPointLights(PointLight *pointLights, unsigned int count);
	void setSpotLights(SpotLight* spotLights, unsigned int count);

	void useShader();
	void clearShader();

	~Shader();

private:
	int pointLightCount;
	int spotLightCount;

	GLuint shaderID,
		uniformProjection,
		uniformModel,
		uniformView,
		uniformEyePosition,
		uniformSpecularIntensity,
		uniformShininess;

	struct UniformDirectionalLight
	{
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	} uniformDirectionalLight;

	GLuint uniformPointLightCount;

	struct UniformPointLight
	{
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;

		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHT_COUNT];

	GLuint uniformSpotLightCount;

	struct UniformSpotLight
	{
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;

		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;
	} uniformSpotLight[MAX_SPOT_LIGHT_COUNT];

	void compileShader(const char* vertex, const char* fragment);
	void addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};
