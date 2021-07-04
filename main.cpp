#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Constants.h"

#include "Mesh.h"
#include "Shader.h"
#include "GLWindow.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "PointLight.h"
#include "SpotLight.h"

const float toRadians = 3.14159265f / 180.0f;

GLWindow mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;
Camera camera;

Material shinyMaterial;
Material dullMaterial;

DirectionalLight directionalLight;
PointLight pointLights[MAX_POINT_LIGHT_COUNT];
SpotLight spotLights[MAX_SPOT_LIGHT_COUNT];

Texture greenMetalTexture;
Texture rustyMetalTexture;
Texture plainTexture;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;


// Shaders
static const char* vShader = "shaders/shader.vertex";
static const char* fShader = "shaders/shader.fragment";

void printVertices(GLfloat* vertices)
{
	printf("****************************************************************************************************************\n");
	for (size_t i = 0; i < 32; i += 8)
	{
		printf("VERTEX: %f, %f, %f   UV: %f, %f   N: %f, %f, %f\n", vertices[i], vertices[i + 1], vertices[i + 2],
																	vertices[i + 3], vertices[i + 4],
																	vertices[i + 5], vertices[i + 6], vertices[i + 7]);
	}
	printf("****************************************************************************************************************\n");
}

void calculateAverageNormals(unsigned int* indices, unsigned int indexCount,
							 GLfloat* vertices, unsigned int vertexCount,
							 unsigned int vertexLength, unsigned int normalOffset)
{
	printf("ORIGINAL VERTICES\n");
	printVertices(vertices);

	for (size_t i = 0; i < indexCount; i += 3)
	{
		unsigned int index0 = indices[i] * vertexLength;
		unsigned int index1 = indices[i + 1] * vertexLength;
		unsigned int index2 = indices[i + 2] * vertexLength;

		glm::vec3 v1(vertices[index1] - vertices[index0],
					 vertices[index1 + 1] - vertices[index0 + 1],
					 vertices[index1 + 2] - vertices[index0 + 2]);

		glm::vec3 v2(vertices[index2] - vertices[index0],
				     vertices[index2 + 1] - vertices[index0 + 1],
				     vertices[index2 + 2] - vertices[index0 + 2]);

		glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

		index0 += normalOffset;
		index1 += normalOffset;
		index2 += normalOffset;

		vertices[index0] += normal.x;
		vertices[index0 + 1] += normal.y;
		vertices[index0 + 2] += normal.z;

		vertices[index1] += normal.x;
		vertices[index1 + 1] += normal.y;
		vertices[index1 + 2] += normal.z;

		vertices[index2] += normal.x;
		vertices[index2 + 1] += normal.y;
		vertices[index2 + 2] += normal.z;
	}
	printf("VERTICES AFTER LOOP 1\n");
	printVertices(vertices);

	for (size_t i = 0; i < vertexCount / vertexLength; i++)
	{
		unsigned int offset = i * vertexLength + normalOffset;
		glm::vec3 vec(vertices[offset], vertices[offset + 1], vertices[offset + 2]);
		vec = glm::normalize(vec);
		vertices[offset] = vec.x;
		vertices[offset + 1] = vec.y;
		vertices[offset + 2] = vec.z;
	}
	printf("VERTICES AFTER LOOP 2\n");
	printVertices(vertices);
}

void createMeshes()
{

	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//X       Y       Z        U     V            Normals
		-1.0f,  -1.0f,   -0.6f,    0.0f, 0.0f,    0.0f,   0.0f,   0.0f,
		 0.0f,  -1.0f,   1.0f,    0.5f, 0.0f,    0.0f,   0.0f,   0.0f,
		 1.0f,  -1.0f,   -0.6f,    1.0f, 0.0f,    0.0f,   0.0f,   0.0f,
		 0.0f,   1.0f,   0.0f,    0.5f, 1.0f,    0.0f,   0.0f,   0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,     0.0f, 0.0f,     0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,      10.0f, 0.0f,    0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,      0.0f, 10.0f,    0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,       10.0f, 10.0f,   0.0f, -1.0f, 0.0f
	};

	calculateAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh* mesh1 = new Mesh();
	mesh1->createMesh(vertices, indices, 32, 12);
	meshList.push_back(mesh1);

	Mesh* mesh2 = new Mesh();
	mesh2->createMesh(vertices, indices, 32, 12);
	meshList.push_back(mesh2);

	Mesh* mesh3 = new Mesh();
	mesh3->createMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(mesh3);
}

void createShaders()
{
	Shader *shader1 = new Shader();
	shader1->createFromFiles(vShader, fShader);
	shaderList.push_back(shader1);
}

int main()
{
	mainWindow = GLWindow(1366, 768);
	mainWindow.initialize();

	createMeshes();
	createShaders();

	camera = Camera(
		glm::vec3(
			0.0f,
			0.0f,
			0.0f
		),
		glm::vec3(
			0.0f,
			1.0f,
			0.0f
		),
		-90.0f,
		0.0f,
		5.0f,
		0.25f
	);
	directionalLight = DirectionalLight(
		1.0f, 1.0f, 1.0f,
		0.0f,
		0.0f,
		glm::vec3(
			0.0f,
			0.0f,
			-1.0f
		)
	);

	unsigned int pointLightCount = 0;

	pointLights[0] = PointLight(
		0.0f, 0.0f, 1.0f,
		0.0f,
		0.0f,
		glm::vec3(
			4.0f,
			0.0f,
			0.0f
		),
		0.3f,
		0.2f,
		0.1f
	);
	//pointLightCount++;
	pointLights[1] = PointLight(
		0.0f, 1.0f, 0.0f,
		0.0f,
		0.0f,
		glm::vec3(
			-4.0f,
			2.0f,
			0.0f
		),
		0.3f,
		0.1f,
		0.1f
	);
	//pointLightCount++;

	unsigned int spotLightCount = 0;

	spotLights[0] = SpotLight(
		1.0f, 1.0f, 1.0f,
		0.0f,
		2.0f,
		glm::vec3(
			0.0f,
			0.0f,
			0.0f
		),
		glm::vec3(
			0.0,
			-1.0f,
			0.0f
		),
		1.0f,
		0.0f,
		0.0f,
		20.0f
	);
	spotLightCount++;

	spotLights[1] = SpotLight(
		1.0f, 0.0f, 0.0f,
		0.0f,
		1.0f,
		glm::vec3(
			0.0f,
			-1.5f,
			0.0f
		),
		glm::vec3(
			-100.0f,
			-1.0f,
			0.0f
		),
		1.0f,
		0.0f,
		0.0f,
		20.0f
	);
	//spotLightCount++;

	greenMetalTexture = Texture("textures/brick.png");
	greenMetalTexture.loadTexture();
	rustyMetalTexture = Texture("textures/dirt.png");
	rustyMetalTexture.loadTexture();
	plainTexture = Texture("textures/white.png");
	plainTexture.loadTexture();

	shinyMaterial = Material(4.0f, 256.0f);
	dullMaterial = Material(0.3f, 4.0f);

	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformSpecularIntensity = 0;
	GLuint uniformEyePosition = 0;
	GLuint uniformShininess = 0;

	glm::mat4 projection = glm::perspective(
		glm::radians(45.0f),
		mainWindow.getBufferWidth() / mainWindow.getBufferHeight(),
		0.1f,
		100.0f
	);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		// Get + handle user input events
		glfwPollEvents();

		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0]->useShader();
		uniformModel = shaderList[0]->getModelLocation();
		uniformProjection = shaderList[0]->getProjectionLocation();
		uniformView = shaderList[0]->getViewLocation();
		uniformSpecularIntensity = shaderList[0]->getSpecularIntensityLocation();
		uniformShininess = shaderList[0]->getShininessLocation();
		uniformEyePosition = shaderList[0]->getEyePositionLocation();

		glm::vec3 flashLightPosition = camera.getCameraPosition();
		flashLightPosition.y -= 0.3f;
		spotLights[0].setFlash(flashLightPosition, camera.getCameraDirection());

		shaderList[0]->setDirectionalLight(&directionalLight);
		shaderList[0]->setPointLights(pointLights, pointLightCount);
		shaderList[0]->setSpotLights(spotLights, spotLightCount);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(
			uniformEyePosition,
			camera.getCameraPosition().x,
			camera.getCameraPosition().y,
			camera.getCameraPosition().z
		);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		greenMetalTexture.useTexture();
		shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->renderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rustyMetalTexture.useTexture();
		dullMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->renderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rustyMetalTexture.useTexture();
		//shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->renderMesh();

		// Unload program
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}