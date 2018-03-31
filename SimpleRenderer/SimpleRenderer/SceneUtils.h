//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// Description  : A collection of functions used to create new entities.
// Author       : Lance Chaney
// Mail         : lance.cha7337@mediadesign.school.nz
//

#pragma once

#include <glad\glad.h>
#include <glm\glm.hpp>

#include <vector>

struct Scene;
struct VertexFormat;
struct MeshComponent;
struct InputComponent;

namespace SceneUtils {
	// Creates a new entity in the scene and returns its ID
	size_t createEntity(Scene& scene);

	// Destroys an entity in the scene
	void destroyEntity(Scene& scene, size_t entityID);

	// Returns the number of entities in the scene
	size_t getEntityCount(const Scene& scene);

	// Creates a unit square facing down the positive z axis with the 
	// specified transform
	size_t createQuad(Scene&, const glm::mat4& transform = glm::mat4{ 1 });

	// Creates a unit sphere, centered at the origin, with the specified 
	// transform;
	size_t createSphere(Scene&, const glm::mat4& transform = glm::mat4{ 1 });

	// Creates a cylinder with the specified radius and height.
	size_t createCylinder(Scene&, float radius = 1, float height = 1, const glm::mat4& transform = glm::mat4{ 1 });

	// Creates a pyramid
	size_t createPyramid(Scene&, const glm::mat4& transform = glm::mat4{ 1 });

	// Creates a cube
	size_t createCube(Scene&, const glm::mat4& transform = glm::mat4{ 1 });

	// Creates a camera.
	// This camera needs to be set as active on the render in order to be rendered from.
	size_t createCamera(Scene&, const glm::vec3& pos, const glm::vec3& center, const glm::vec3& up = glm::vec3{ 0, 1, 0 });

	// Creates a skybox.
	// Can be used to set the environment map for the renderer.
	size_t createSkybox(Scene&, const std::vector<std::string>& faceFilenames);
	
	// Handles boilerplate input binding
	void setDefaultInputBindings(InputComponent& input);

	// Returns the vertices to construct a quad.
	// This function is cached for efficiency 
	// (only 1 set of vertices will be constructed).
	const std::vector<VertexFormat>& getQuadVertices();

	// Returns the indices to construct a quad.
	// This function is cached for efficiency 
	// (only 1 set of indices will be constructed).
	const std::vector<GLuint>& getQuadIndices();

	// Returns the vertices to construct a sphere.
	// This function is cached for efficiency 
	// (only 1 set of vertices will be constructed).
	const std::vector<VertexFormat>& getSphereVertices();

	// Returns the indices to construct a sphere.
	// This function is cached for efficiency 
	// (only 1 set of vertices will be constructed).
	const std::vector<GLuint>& getSphereIndices();

	// Returns the vertices to construct a cylinder.
	// This function is cached for efficiency 
	// (only 1 set of vertices will be constructed).
	const std::vector<VertexFormat>& getCylinderVertices();

	// Returns the indices to construct a cylinder.
	// This function is cached for efficiency 
	// (only 1 set of indices will be constructed).
	const std::vector<GLuint>& getCylinderIndices();

	// Returns the vertices to construct a pyramid.
	// This function is cached for efficiency 
	// (only 1 set of vertices will be constructed).
	const std::vector<VertexFormat>& getPyramidVertices();

	// Returns the indices to construct a pyramid.
	// This function is cached for efficiency 
	// (only 1 set of indices will be constructed).
	const std::vector<GLuint>& getPyramidIndices();

	// Returns the vertices to construct a cube.
	// This function is cached for efficiency 
	// (only 1 set of vertices will be constructed).
	const std::vector<VertexFormat>& getCubeVertices();

	// Returns the indices to construct a cube.
	// This function is cached for efficiency 
	// (only 1 set of indices will be constructed).
	const std::vector<GLuint>& getCubeIndices();

	// Returns a Mesh Component containing the VAO for a quad.
	// This function is cached for efficiency 
	// (Only 1 mesh will be buffered).
	MeshComponent getQuadMesh();

	// Returns a Mesh Component containing the VAO for a sphere.
	// This function is cached for efficiency 
	// (Only 1 mesh will be buffered).
	MeshComponent getSphereMesh();

	// Returns a Mesh Component containing the VAO for a cylinder.
	// This function is cached for efficiency 
	// (Only 1 mesh will be buffered).
	MeshComponent getCylinderMesh();

	// Returns a Mesh Component containing the VAO for a pyramid.
	// This function is cached for efficiency
	// (Only 1 mesh will be buffered).
	MeshComponent getPyramidMesh();

	// Returns a Mesh Component containing the VAO for a cube.
	// This function is cached for efficiency
	// (Only 1 mesh will be buffered).
	MeshComponent getCubeMesh();
}