//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// Description  : The main entry point for the program.
//                Handles executing the main loop.
// Author       : Lance Chaney
// Mail         : lance.cha7337@mediadesign.school.nz
//

#define _USE_MATH_DEFINES

#include "GLUtils.h"
#include "SceneUtils.h"
#include "InputSystem.h"
#include "MovementSystem.h"
#include "RenderSystem.h"
#include "Scene.h"
#include "GameplayLogicSystem.h"

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <cmath>

int main()
{
	GLFWwindow* window = GLUtils::initOpenGL();

	Scene scene;
	RenderSystem renderSystem(window, scene);
	MovementSystem movementSystem(scene);
	InputSystem inputSystem(window, scene);
	GameplayLogicSystem gameplayLogicSystem(scene, inputSystem);

	// Order matters, buttons are assigned to the first four entities created
	SceneUtils::createSphere(scene, glm::translate({}, glm::vec3{ -1.5f, 1.5f, 0 }));
	SceneUtils::createQuad(scene, 
		  glm::translate({}, glm::vec3{ 1.5f, 1.5f, 0})
		* glm::rotate(glm::mat4{}, static_cast<float>(-M_PI / 16), glm::vec3{ 1, 0, 0 }));
	SceneUtils::createCylinder(scene, 1.5, 1.5,
		  glm::translate(glm::mat4{}, glm::vec3{ -1.5f, -1.5f, 0 })
		* glm::rotate(glm::mat4{}, static_cast<float>(M_PI / 4), glm::vec3{ 0, 0, 1 }));
	SceneUtils::createPyramid(scene, glm::translate({}, glm::vec3{ 1.5f, -1.5f, 0 }));
	
	//SceneUtils::createCube(scene);
	size_t skybox = SceneUtils::createSkybox(scene, {
		"Assets/Textures/Skybox/right.jpg",
		"Assets/Textures/Skybox/left.jpg",
		"Assets/Textures/Skybox/top.jpg",
		"Assets/Textures/Skybox/bottom.jpg",
		"Assets/Textures/Skybox/back.jpg",
		"Assets/Textures/Skybox/front.jpg",
	});
	renderSystem.setEnvironmentMap(skybox);

	size_t cameraEntity = SceneUtils::createCamera(scene, { 0, 0, 6 }, { 0, 0, 0 }, { 0, 1, 0 });
	renderSystem.setCamera(cameraEntity);

	while (!glfwWindowShouldClose(window)) {
		inputSystem.beginFrame();
		renderSystem.beginRender();

		for (size_t entityID = 0; entityID < SceneUtils::getEntityCount(scene); ++entityID) {
			gameplayLogicSystem.update(entityID);
			inputSystem.update(entityID);
			movementSystem.update(entityID);
			renderSystem.update(entityID);
		}
		
		renderSystem.endRender();
		
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}