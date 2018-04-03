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

#include "Scene.h"

#include <glad\glad.h>
#include <glm\glm.hpp>

#include <queue>

struct Scene;
struct GLFWwindow;

class CompareDepth {
public:
	CompareDepth() {}
	CompareDepth(const Scene* scene, const glm::vec3& cameraPos)
		: m_scene{ scene }
		, m_cameraPos{ cameraPos }
	{ }

	bool operator()(size_t entityID1, size_t entityID2) {
		// return "true" if "entity1" is ordered before "entity2", for example:
		glm::vec3 entity1Pos = m_scene->transformComponents[entityID1][3];
		glm::vec3 entity2Pos = m_scene->transformComponents[entityID2][3];
		float entity1Depth = glm::length(entity1Pos - m_cameraPos);
		float entity2Depth = glm::length(entity2Pos - m_cameraPos);

		return entity1Depth < entity2Depth;
	}

private:
	const Scene* m_scene;
	glm::vec3 m_cameraPos;
};

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

	bool mousePick(const glm::dvec2& mousePos, size_t& outEntityID) const;
private:
	GLFWwindow* m_glContext;
	Scene& m_scene;
	GLuint m_uboUniforms;
	GLuint m_uboShaderParams;
	GLuint m_uniformBindingPoint;
	GLuint m_shaderParamsBindingPoint;
	size_t m_cameraEntity;
	std::priority_queue<size_t, std::vector<size_t>, CompareDepth> m_transparentObjects;

	// Handler to a cube map on the GPU, used for reflections and environmental lighting
	GLuint m_environmentMap;
	bool m_isEnvironmentMap;
};