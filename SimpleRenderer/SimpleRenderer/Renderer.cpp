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

#include "RenderSystem.h"

#include "GLUtils.h"
#include "MaterialComponent.h"
#include "MeshComponent.h"
#include "Scene.h"
#include "UniformFormat.h"
#include "SceneUtils.h"

#include <GLFW\glfw3.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

using glm::mat4;
using glm::vec3;
using glm::vec4;

RenderSystem::RenderSystem(GLFWwindow* glContext, Scene& scene)
	: m_glContext{ glContext }
	, m_scene{ scene }
	, m_uniformBindingPoint{ 0 }
	, m_shaderParamsBindingPoint{ 1 }
{
	// Create buffer for camera parameters
	glGenBuffers(1, &m_uboUniforms);
	glBindBufferBase(GL_UNIFORM_BUFFER, m_uniformBindingPoint, m_uboUniforms);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(UniformFormat), nullptr, GL_DYNAMIC_DRAW);

	// Create buffer for shader parameters
	glGenBuffers(1, &m_uboShaderParams);
	glBindBufferBase(GL_UNIFORM_BUFFER, m_shaderParamsBindingPoint, m_uboShaderParams);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(ShaderParams), nullptr, GL_DYNAMIC_DRAW);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void RenderSystem::beginRender()
{
	glDepthMask(GL_TRUE);

	glStencilMask(0xFF);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glm::vec3 cameraPos = m_scene.transformComponents.at(m_cameraEntity)[3];
	m_transparentObjects = std::priority_queue<size_t, std::vector<size_t>, CompareDepth>(
		CompareDepth(&m_scene, cameraPos)
	);
}

void RenderSystem::endRender()
{
	while (m_transparentObjects.size() > 0) {
		update(m_transparentObjects.top());
		m_transparentObjects.pop();
	}

	glfwSwapBuffers(m_glContext);
}

void RenderSystem::update(size_t entityID)
{
	// Filter renderable entities
	const size_t kRenderableMask = COMPONENT_MESH | COMPONENT_MATERIAL;
	size_t components = m_scene.componentMasks.at(entityID);
	if ((m_scene.componentMasks.at(entityID) & kRenderableMask) != kRenderableMask)
		return;

	// Get rendering components of entity
	MaterialComponent& material = m_scene.materialComponents[entityID];
	const MeshComponent& mesh = m_scene.meshComponents.at(entityID);
	mat4& transform = m_scene.transformComponents.at(entityID);

	// TODO: Add check that camera is a valid camera entity, throw error otherwise
	const mat4& cameraTransform = m_scene.transformComponents.at(m_cameraEntity);

	if (material.enableDepth) {
		glCullFace(GL_BACK);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
	}
	else {
		glCullFace(GL_FRONT);
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_LEQUAL);
	}

	// Do Stencil setup for outlined objects
	if (material.hasOutline) {
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilFunc(GL_ALWAYS, 0xFF, 0xFF);
	}
	else if (material.isOutline) {
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilFunc(GL_NOTEQUAL, 0xFF, 0xFF);
	}
	else {
		glDisable(GL_STENCIL_TEST);
	}

	// Handle transparent objects
	if (material.isTransparent && m_transparentObjects.size() > 0 && m_transparentObjects.top() == entityID) {
		// If we are rendering the transparent object queue then enable blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else if (material.isTransparent) {
		// If we encounter a transparent object while rendering then defer the render till later
		m_transparentObjects.push(entityID);
		return;
	}
	else {
		// Disable blending for opaque objects
		glDisable(GL_BLEND);
	}

	// Tell the gpu what material to use
	glUseProgram(material.shader);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(material.shader, "sampler"), 0);
	glBindTexture(material.textureType, material.texture);

	// Set environment map to use on GPU
	if (m_isEnvironmentMap) {
		glActiveTexture(GL_TEXTURE1);
		glUniform1i(glGetUniformLocation(material.shader, "environmentSampler"), 1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_environmentMap);
	}

	// Send shader parameters to gpu
	GLuint blockIndex;
	blockIndex = glGetUniformBlockIndex(material.shader, "ShaderParams");
	glUniformBlockBinding(material.shader, blockIndex, m_shaderParamsBindingPoint);
	glBindBufferBase(GL_UNIFORM_BUFFER, m_shaderParamsBindingPoint, m_uboShaderParams);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ShaderParams), &material.shaderParams);
		
	// Get Aspect ratio
	int width, height;
	glfwGetFramebufferSize(m_glContext, &width, &height);
	float aspectRatio = static_cast<float>(width) / height;

	// Get model, view and projection matrices
	UniformFormat uniforms;
	bool hasTransform = (components & COMPONENT_TRANSFORM) ==  COMPONENT_TRANSFORM;
	uniforms.model = hasTransform ? transform : glm::mat4{ 1 };
	uniforms.view = glm::inverse(cameraTransform);
	uniforms.projection = glm::perspective(glm::radians(60.0f), aspectRatio, 0.5f, 100.0f);
	uniforms.cameraPos = cameraTransform[3];

	// Send the model view and projection matrices to the gpu
	blockIndex = glGetUniformBlockIndex(material.shader, "Uniforms");
	glUniformBlockBinding(material.shader, blockIndex, m_uniformBindingPoint);
	glBindBufferBase(GL_UNIFORM_BUFFER, m_uniformBindingPoint, m_uboUniforms);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(UniformFormat), &uniforms);

	// Draw object
	glBindVertexArray(mesh.VAO);
	glDrawElements(GL_TRIANGLES, mesh.numIndices, GL_UNSIGNED_INT, 0);

	// Handle rendering outline for outlined objects
	if (material.hasOutline) {
		// Save original render state variables
		GLuint origShader = material.shader;
		mat4 origTransform = transform;

		// Apply outline render state
		material.shader = GLUtils::getOutlineShader();
		material.hasOutline = false;
		material.isOutline = true;
		transform = transform * glm::scale(mat4{}, vec3{ 1.1f, 1.1f, 1.1f });

		// Render scaled up object with outline shader
		update(entityID);
		
		// Restore render state variables
		material.shader = origShader;
		material.hasOutline = true;
		material.isOutline = false;
		transform = origTransform;

		glClear(GL_STENCIL_BUFFER_BIT);
	}
}

void RenderSystem::setCamera(size_t entityID)
{
	// TODO: Throw error if entity does not have a camera component
	m_cameraEntity = entityID;
}

void RenderSystem::setEnvironmentMap(size_t entityID)
{
	// TODO: Error if not a cube map
	m_environmentMap = m_scene.materialComponents.at(entityID).texture;
	m_isEnvironmentMap = true;
}

bool RenderSystem::mousePick(const glm::dvec2& mousePos, size_t& outEntityID) const
{
	/**********************************/
	/** Construct ray in world space **/
	/**********************************/

	int width;
	int height;
	glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
	float aspectRatio = static_cast<float>(width) / height;

	// Get mouse position in NDC space
	glm::vec2 mousePosNDC;
	mousePosNDC.x = 2 * mousePos.x / width - 1;
	mousePosNDC.y = 1 - 2 * mousePos.y / height;

	// Clip space
	glm::vec4 clipCoords = glm::vec4(mousePosNDC.x, mousePosNDC.y, 1, 1);

	// View space
	mat4 inverseProj = glm::inverse(glm::perspective(glm::radians(60.0f), aspectRatio, 0.5f, 100.0f));
	glm::vec4 eyeCoords = inverseProj * clipCoords;
	//eyeCoords /= eyeCoords.w;
	//eyeCoords.z = -1;
	eyeCoords.w = 0;

	// World space
	const mat4& inverseView = m_scene.transformComponents.at(m_cameraEntity);
	glm::vec4 rayDir4 = inverseView * eyeCoords;
	glm::vec3 rayDir = rayDir4;
	rayDir = glm::normalize(rayDir);
	glm::vec3 rayOrigin = inverseView[3];

	/***************************************/
	/** Perform raytrace for scene entity **/
	/***************************************/

	// Perform ray trace for entities
	for (size_t entityID = 0; entityID < SceneUtils::getEntityCount(m_scene); ++entityID) {
		// Filter for renderable components
		const size_t kRenderableMask = COMPONENT_MESH | COMPONENT_MATERIAL;
		if ((m_scene.componentMasks.at(entityID) & kRenderableMask) != kRenderableMask)
			continue;

		auto& indices = *m_scene.meshComponents[entityID].indices;
		auto& vertices = *m_scene.meshComponents[entityID].vertices;
		mat4 model = m_scene.transformComponents[entityID];

		for (size_t i = 0; i < indices.size(); i += 3) {

			const float EPSILON = 0.0000001;
			vec3 vertex0 = model * vec4{ vertices[indices[i]].position, 1.0f };
			vec3 vertex1 = model * vec4{ vertices[indices[i + 1]].position, 1.0f };
			vec3 vertex2 = model * vec4{ vertices[indices[i + 2]].position, 1.0f };
			vec3 edge1, edge2, h, s, q;
			float a, f, u, v;
			edge1 = vertex1 - vertex0;
			edge2 = vertex2 - vertex0;
			h = glm::cross(rayDir, edge2);
			a = glm::dot(edge1, h);
			if (a > -EPSILON && a < EPSILON)
				continue;
			f = 1 / a;
			s = rayOrigin - vertex0;
			u = f * glm::dot(s, h);
			if (u < 0.0 || u > 1.0)
				continue;
			q = glm::cross(s, edge1);
			v = f * glm::dot(rayDir, q);
			if (v < 0.0 || u + v > 1.0)
				continue;
			// At this stage we can compute t to find out where the intersection point is on the line.
			float t = f * glm::dot(edge2, q);
			if (t > EPSILON) // ray intersection
			{
				// outIntersectionPoint = rayOrigin + rayDir * t;
				outEntityID = entityID;
				return true;
			}
		}
	}

	return false;
}
