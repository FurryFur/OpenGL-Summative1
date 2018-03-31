//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// Description  : A component for storing per entity
//                material information used by the GPU.
// Author       : Lance Chaney
// Mail         : lance.cha7337@mediadesign.school.nz
//

#pragma once

#include "ShaderParams.h"

#include <glad\glad.h>

struct MaterialComponent {
	GLuint shader;
	GLuint texture;
	GLenum textureType;
	bool enableDepth;
	ShaderParams shaderParams;
};