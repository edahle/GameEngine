#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
	pointLightCount = 0;
	spotLightCount = 0;
}

void Shader::createFromString(const char* vertex, const char* fragment)
{
	compileShader(vertex, fragment);
}

std::string Shader::readFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open())
	{
		printf("failed to read %s!", fileLocation);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();

	return content;
}

void Shader::createFromFiles(const char* vertex, const char* fragment)
{
	std::string vertexString = readFile(vertex);
	std::string fragmentString = readFile(fragment);

	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	compileShader(vertexCode, fragmentCode);
}

GLuint Shader::getProjectionLocation()
{
	return uniformProjection;
}

GLuint Shader::getModelLocation()
{
	return uniformModel;
}

GLuint Shader::getViewLocation()
{
	return uniformView;
}

GLuint Shader::getAmbientIntensityLocation()
{
	return uniformDirectionalLight.uniformAmbientIntensity;
}

GLuint Shader::getAmbientIntensityColorLocation()
{
	return uniformDirectionalLight.uniformColor;
}

GLuint Shader::getdiffuseIntensityLocation()
{
	return uniformDirectionalLight.uniformDiffuseIntensity;
}

GLuint Shader::getDirectionLocation()
{
	return uniformDirectionalLight.uniformDirection;
}

GLuint Shader::getSpecularIntensityLocation()
{
	return uniformSpecularIntensity;
}

GLuint Shader::getShininessLocation()
{
	return uniformShininess;
}

GLuint Shader::getEyePositionLocation()
{
	return uniformEyePosition;
}

void Shader::setDirectionalLight(DirectionalLight* directionalLight)
{
	directionalLight->useLight(
		uniformDirectionalLight.uniformAmbientIntensity,
		uniformDirectionalLight.uniformColor,
		uniformDirectionalLight.uniformDiffuseIntensity,
		uniformDirectionalLight.uniformDirection
	);
}

void Shader::setPointLights(PointLight* pointLights, unsigned int count)
{
	if (count > MAX_POINT_LIGHT_COUNT) count = MAX_POINT_LIGHT_COUNT;

	glUniform1i(uniformPointLightCount, count);

	for (rsize_t i = 0; i < count; i++)
	{
		pointLights[i].useLight(
			uniformPointLight[i].uniformAmbientIntensity,
			uniformPointLight[i].uniformColor,
			uniformPointLight[i].uniformDiffuseIntensity,
			uniformPointLight[i].uniformPosition,
			uniformPointLight[i].uniformConstant,
			uniformPointLight[i].uniformLinear,
			uniformPointLight[i].uniformExponent
		);
	}
}

void Shader::setSpotLights(SpotLight* spotLights, unsigned int count)
{
	if (count > MAX_SPOT_LIGHT_COUNT) count = MAX_SPOT_LIGHT_COUNT;

	glUniform1i(uniformSpotLightCount, count);

	for (rsize_t i = 0; i < count; i++)
	{
		spotLights[i].useLight(
			uniformSpotLight[i].uniformAmbientIntensity,
			uniformSpotLight[i].uniformColor,
			uniformSpotLight[i].uniformDiffuseIntensity,
			uniformSpotLight[i].uniformPosition,
			uniformSpotLight[i].uniformDirection,
			uniformSpotLight[i].uniformConstant,
			uniformSpotLight[i].uniformLinear,
			uniformSpotLight[i].uniformExponent,
			uniformSpotLight[i].uniformEdge
		);
	}
}

void Shader::useShader()
{
	glUseProgram(shaderID);
}

void Shader::clearShader()
{
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}
	
	uniformModel = 0;
	uniformProjection = 0;
}

void Shader::addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d program '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

void Shader::compileShader(const char* vertex, const char* fragment)
{
	//1. Create Program
	//2. Read Shader Source String
	//3. Compile Shader
	//4. Attach Shader to Program
	//5. Link Program
	//6. Validate Program
	//7. Other stuff (uniform vars)

	shaderID = glCreateProgram();

	if (!shaderID)
	{
		printf("Error creating shader program!");
		return;
	}

	addShader(shaderID, vertex, GL_VERTEX_SHADER);
	addShader(shaderID, fragment, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program '%s'\n", eLog);
		return;
	}

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program '%s'\n", eLog);
		return;
	}

	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformView = glGetUniformLocation(shaderID, "view");

	uniformDirectionalLight.uniformColor = glGetUniformLocation(shaderID, "directionalLight.base.color");
	uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
	uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
	uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");

	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
	uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");

	uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

	for (rsize_t i = 0; i < MAX_POINT_LIGHT_COUNT; i++)
	{
		char localBuff[100] = { '\0' };

		snprintf(localBuff, sizeof(localBuff), "pointLights[%d].base.color", i);
		uniformPointLight[i].uniformColor = glGetUniformLocation(shaderID, localBuff);

		snprintf(localBuff, sizeof(localBuff), "pointLights[%d].base.ambientIntensity", i);
		uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, localBuff);

		snprintf(localBuff, sizeof(localBuff), "pointLights[%d].base.diffuseIntensity", i);
		uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, localBuff);

		snprintf(localBuff, sizeof(localBuff), "pointLights[%d].position", i);
		uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, localBuff);

		snprintf(localBuff, sizeof(localBuff), "pointLights[%d].constant", i);
		uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, localBuff);

		snprintf(localBuff, sizeof(localBuff), "pointLights[%d].linear", i);
		uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, localBuff);

		snprintf(localBuff, sizeof(localBuff), "pointLights[%d].exponent", i);
		uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, localBuff);
	}

	uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

	for (rsize_t i = 0; i < MAX_SPOT_LIGHT_COUNT; i++)
	{
		char localBuff[100] = { '\0' };

		snprintf(localBuff, sizeof(localBuff), "spotLights[%d].base.base.color", i);
		uniformSpotLight[i].uniformColor = glGetUniformLocation(shaderID, localBuff);

		snprintf(localBuff, sizeof(localBuff), "spotLights[%d].base.base.ambientIntensity", i);
		uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, localBuff);

		snprintf(localBuff, sizeof(localBuff), "spotLights[%d].base.base.diffuseIntensity", i);
		uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, localBuff);

		snprintf(localBuff, sizeof(localBuff), "spotLights[%d].base.position", i);
		uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, localBuff);

		snprintf(localBuff, sizeof(localBuff), "spotLights[%d].base.constant", i);
		uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, localBuff);

		snprintf(localBuff, sizeof(localBuff), "spotLights[%d].base.linear", i);
		uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID, localBuff);

		snprintf(localBuff, sizeof(localBuff), "spotLights[%d].base.exponent", i);
		uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID, localBuff);

		snprintf(localBuff, sizeof(localBuff), "spotLights[%d].direction", i);
		uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID, localBuff);

		snprintf(localBuff, sizeof(localBuff), "spotLights[%d].edge", i);
		uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID, localBuff);
	}
}


Shader::~Shader()
{

}
