//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// Description  : A struct containing model, view and projection matrices
//                as well as camera position.
//                Corresponds to an equivalent uniform buffer object on the
//                GPU with same alignment.
// Author       : Lance Chaney
// Mail         : lance.cha7337@mediadesign.school.nz
//

#pragma once

#include <glm\glm.hpp>

struct UniformFormat {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec4 cameraPos;
};