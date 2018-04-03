//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// Description  : A system which handles input propogation.
// Author       : Lance Chaney
// Mail         : lance.cha7337@mediadesign.school.nz
//

#pragma once

#include <glm\glm.hpp>

#include <functional>
#include <vector>

struct Scene;
struct GLFWwindow;
class IKeyObserver;
class RenderSystem;

class InputSystem {
public:
	InputSystem(GLFWwindow* window, RenderSystem&, Scene&);

	// Updates the entity with input
	void update(size_t entityID);

	// Does per frame input system update
	void beginFrame();

	// Registers observers for keyboard input
	void registerKeyObserver(IKeyObserver* observer);

private:
	// Handles keyboard events
	void keyCallback(int key, int scancode, int action, int mods);

	void mouseBtnCallback(int button, int action, int mods);

	GLFWwindow* m_window;
	Scene& m_scene;
	RenderSystem& m_renderSystem;
	glm::dvec2 m_mouseDelta;
	std::vector<IKeyObserver*> m_keyObservers;
};