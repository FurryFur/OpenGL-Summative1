//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// Description  : A component for storing per entity
//                mesh information used by the GPU.
// Author       : Lance Chaney
// Mail         : lance.cha7337@mediadesign.school.nz
//

#pragma once

#include "VertexFormat.h"

#include <glad\glad.h>

#include <vector>

struct MeshComponent {
	GLuint VAO;
	GLsizei numIndices;
	const std::vector<VertexFormat>* vertices;
	const std::vector<GLuint>* indices;
};