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
	InputSystem inputSystem(window, renderSystem, scene);
	GameplayLogicSystem gameplayLogicSystem(scene, inputSystem);

	// Order matters, buttons are assigned to the first four entities created
	SceneUtils::createSphere(scene, glm::translate({}, glm::vec3{ -1.5f, 1.5f, 0 }));

	size_t cubeID = SceneUtils::createCube(scene, 
		  glm::translate({}, glm::vec3{ 1.5f, 1.5f, 0})
		* glm::rotate(glm::mat4{}, static_cast<float>(-M_PI / 16), glm::vec3{ 1, 0, 0 }));
	scene.materialComponents[cubeID].hasOutline = true;
	scene.materialComponents[cubeID].texture = GLUtils::loadTexture("Assets/Textures/transparent.png");
	scene.materialComponents[cubeID].isTransparent = true;

	SceneUtils::createCylinder(scene, 1.5, 1.5,
		  glm::translate(glm::mat4{}, glm::vec3{ -1.5f, -1.5f, 0 })
		* glm::rotate(glm::mat4{}, static_cast<float>(M_PI / 4), glm::vec3{ 0, 0, 1 }));

	size_t pyramidID = SceneUtils::createPyramid(scene, glm::translate({}, glm::vec3{ 1.5f, -1.5f, 0 }));
	scene.materialComponents[pyramidID].texture = GLUtils::loadTexture("Assets/Textures/transparent.png");
	scene.materialComponents[pyramidID].isTransparent = true;
	
	size_t waterID = SceneUtils::createQuad(scene, 
		  glm::translate({}, glm::vec3{ 0, -4.0f, 0 })
		* glm::rotate({}, static_cast<float>(-M_PI / 2), glm::vec3{ 1, 0, 0 })
		* glm::scale({}, glm::vec3{ 100, 100, 100 }));
	scene.materialComponents[waterID].shader = GLUtils::getWaterShader();
	scene.materialComponents[waterID].isTransparent = true;
	scene.materialComponents[waterID].shaderParams.metallicness = 0.5;
	scene.materialComponents[waterID].texture = GLUtils::loadTexture("Assets/Textures/water.png");
	scene.componentMasks[waterID] &= ~COMPONENT_LOGIC;

	size_t waterFloorID = SceneUtils::createQuad(scene,
		glm::translate({}, glm::vec3{ 0, -6.0f, 0 })
		* glm::rotate({}, static_cast<float>(-M_PI / 2), glm::vec3{ 1, 0, 0 })
		* glm::scale({}, glm::vec3{ 100, 100, 100 }));
	scene.materialComponents[waterFloorID].shaderParams.metallicness = 0;
	scene.materialComponents[waterFloorID].texture = GLUtils::loadTexture("Assets/Textures/dessert-floor.png");
	scene.componentMasks[waterFloorID] &= ~COMPONENT_LOGIC;
	
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