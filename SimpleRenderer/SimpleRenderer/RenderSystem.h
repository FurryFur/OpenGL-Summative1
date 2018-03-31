//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// Description  : A system which handles rendering each entity.
// Author       : Lance Chaney
// Mail         : lance.cha7337@mediadesign.school.nz
//

#pragma once

#include <glad\glad.h>

struct Scene;
struct GLFWwindow;

class RenderSystem {
public:
	RenderSystem(GLFWwindow* glContext, Scene&);
	RenderSystem(const RenderSystem&) = delete;
	RenderSystem& operator=(const RenderSystem&) = delete;

	// Starts rendering the frame.
	// Should be called before update.
	void beginRender();

	// Renders an entity.
	void update(size_t entityID);

	// Ends the frame.
	void endRender();

	// Sets the current camera.
	void setCamera(size_t entityID);

	// Sets the environment map for reflections
	void setEnvironmentMap(size_t entityID);

private:
	GLFWwindow* m_glContext;
	Scene& m_scene;
	GLuint m_uboUniforms;
	GLuint m_uboShaderParams;
	GLuint m_uniformBindingPoint;
	GLuint m_shaderParamsBindingPoint;
	size_t m_cameraEntity;

	// Handler to a cube map on the GPU, used for reflections and environmental lighting
	GLuint m_environmentMap;
	bool m_isEnvironmentMap;
};