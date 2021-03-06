// GLFW Application.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include <glm/gtx/string_cast.hpp>

#include "Shader.h"
#include "Texture2D.h"
#include "Material.h"
#include "ResourceManager.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

glm::vec3 cameraPos(0.0f, 0.0f, 3.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
float cameraYaw(-90.0f);
float cameraPitch(0.0f);
glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);
float cameraZoom = 45.0f;

glm::vec3 cameraTarget(0.0, 0.0, -7.0f);
glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

float deltaTime;
float currentFrame;
float lastFrame;

glm::mat4 projection;
glm::mat4 view;


namespace GLFW
{
	class Camera
	{
	protected:
	public:
		glm::vec3 position;
		glm::vec3 front;
		glm::vec3 up;
		glm::mat4 view;
		glm::mat4 projection;

		static Camera main;

		Camera();
		~Camera();
		
		void translate(glm::vec3 vector)
		{
			this->position += vector;
		}
	};

	GLFW::Camera::Camera()
	{
	}

	GLFW::Camera::~Camera()
	{
	}
}

int main(int argc, char ** argv)
{
	
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	std::cout << "Refresh rate: " << mode->refreshRate << std::endl;
	std::cout << "Vsync: (ON Line:35)" << std::endl;
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwHideWindow(window);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
														 // glad: load all OpenGL function pointers
														 // ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	glEnable(GL_DEPTH_TEST);

	GLFW::Shader::Init();


	float vertices[] = {
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0, 0.0,
		0.5f, -0.5f, -0.5f,		0.0f,  0.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0, 0.0,
		0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0, 1.0,

		0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0, 1.0,
		-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0, 1.0,
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0, 0.0,

		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0, 0.0,
		0.5f, -0.5f,  0.5f,		0.0f,  0.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0, 0.0,
		0.5f,  0.5f,  0.5f,		0.0f,  0.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0, 1.0,

		0.5f,  0.5f,  0.5f,		0.0f,  0.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0, 1.0,
		-0.5f,  0.5f,  0.5f,	0.0f,  0.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0, 1.0,
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0, 0.0,

		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0, 0.0,
		-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0, 0.0,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0, 1.0,

		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0, 1.0,
		-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0, 1.0,
		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0, 0.0,

		0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0, 1.0,
		0.5f,  0.5f, -0.5f,		1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0, 1.0,
		0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0, 0.0,

		0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0, 0.0,
		0.5f, -0.5f,  0.5f,		1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0, 0.0,
		0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0, 1.0,

		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0, 1.0,
		0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0, 1.0,
		0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0, 0.0,

		0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0, 0.0,
		-0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0, 0.0,
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0, 1.0,

		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0, 1.0,
		0.5f,  0.5f, -0.5f,		0.0f,  1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0, 1.0,
		0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0, 0.0,

		0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0, 0.0,
		-0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0, 0.0,
		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 1.0f, 1.0, 1.0
	};

	// first, configure the cube's VAO (and VBO)
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// color attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// tex coordinates
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(0);
	
	// timing
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	// lighting
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	glm::vec3 lambient(1.0f);
	glm::vec3 ldiffuse(0.0f);
	glm::vec3 lspecular(1.0f);
	
	lambient = glm::vec3(0.25f, 0.20725f, 0.20725f);
	ldiffuse = glm::vec3(1.0f, 0.829f, 0.829f);
	lspecular = glm::vec3(0.296648f);

	

	GLFW::ResourceManager manager;
	manager.load<GLFW::Material>("xml/material_gold.xml", "material/gold");
	manager.load<GLFW::Shader>("xml/shader_distortion.xml", "shader");

	manager.load<GLFW::Material>("xml/material_pearl.xml", "material/lamp");
	manager.load<GLFW::Shader>("xml/shader_lamp.xml", "lampshader");

	manager.load<GLFW::Texture2D>("gfx/test.png", "clouds");

	GLFW::Material* mat = manager.get<GLFW::Material>("material/gold");
	mat->shader = manager.get<GLFW::Shader>("shader");
	GLFW::Texture2D* clouds = manager.get<GLFW::Texture2D>("clouds");
	if(clouds != NULL)
		mat->texture.push_back(clouds);

	GLFW::Material* matLamp = manager.get<GLFW::Material>("material/lamp");
	matLamp->shader = manager.get<GLFW::Shader>("lampshader");

	if (mat->shader == NULL)
	{
		std::cerr << "Shader has not been loaded!" << std::endl;
		return -1;
	}

	mat->use();
	//std::cout << "ambient: " << glm::to_string(mat->ambient) << std::endl;

	mat->setUniform("light.position", lightPos);
	mat->setUniform("viewPos", cameraPos);
	mat->setUniform("light.ambient", lambient);
	mat->setUniform("light.diffuse", ldiffuse);
	mat->setUniform("light.specular", lspecular);
	
	mat->setUniform("material.ambient", mat->ambient);
	mat->setUniform("material.diffuse", mat->diffuse);
	mat->setUniform("material.specular", mat->specular);
	mat->setUniform("material.shininess", mat->shininess);
	//std::cout << "ambient: " << glm::to_string(mat->ambient) << std::endl;

	projection = glm::perspective(glm::radians(cameraZoom), 800.0f / 600.0f, 0.1f, 100.0f);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	mat->setUniform("projection", projection);
	mat->setUniform("view", view);

	glm::mat4 model = glm::mat4(1.0f);
	mat->setUniform("model", model);

	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	glfwShowWindow(window);
	while (!glfwWindowShouldClose(window))
	{
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		
		
		
		/*glm::vec3 lightColor;
		lightColor.x = sin(glfwGetTime() * 2.0f);
		lightColor.y = sin(glfwGetTime() * 0.7f);
		lightColor.z = sin(glfwGetTime() * 1.3f);
		ldiffuse = lightColor * glm::vec3(0.5f);
		lambient = ldiffuse * glm::vec3(0.2f);

		mat->setUniform("light.ambient", lambient);
		mat->setUniform("light.diffuse", ldiffuse);
		mat->setUniform("light.specular", lspecular);*/
		

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		projection = glm::perspective(glm::radians(cameraZoom), 800.0f / 600.0f, 0.1f, 100.0f);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		processInput(window);

		mat->use();
		mat->setUniform("light.position", lightPos);
		mat->setUniform("projection", projection);
		mat->setUniform("view", view);

		model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		mat->setUniform("model", model);

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//mat->shader->use(false);
		glBindVertexArray(0);

		matLamp->use();
		matLamp->setUniform("projection", projection);
		matLamp->setUniform("view", view);
		glm::mat4 mdl = glm::mat4(1.0);
		mdl = glm::translate(mdl, lightPos);
		mdl = glm::scale(mdl, glm::vec3(0.2f)); // a smaller cube
		matLamp->setUniform("model", mdl);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//mat->shader->use(false);

		glfwSwapBuffers(window);
		glfwPollEvents();	
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();

	return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		::cameraZoom -= 0.1f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		::cameraZoom += 0.1f;

	if (::cameraZoom <= 1.0f)
		::cameraZoom = 1.0f;

	if (::cameraZoom >= 120.0f)
		::cameraZoom = 120.0f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		std::cout << "Camera: ";
		std::cout << "\tZoom=\t" << ::cameraZoom << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		view = glm::rotate(view, glm::radians(0.1f), glm::vec3(0.0, 1.0, 0.0));
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		view = glm::rotate(view, glm::radians(0.1f), glm::vec3(0.0, -1.0, 0.0));
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		//view = glm::translate(view, glm::vec3(0.001, 0.0, 0.0));
		cameraPos.x += 0.001f;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		//view = glm::translate(view, glm::vec3(-0.001, 0.0, 0.0));
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}