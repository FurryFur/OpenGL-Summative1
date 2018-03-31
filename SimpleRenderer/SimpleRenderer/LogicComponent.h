//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// Description  : A component for storing per entity
//                game logic state.
// Author       : Lance Chaney
// Mail         : lance.cha7337@mediadesign.school.nz
//

#pragma once

#include <glm\glm.hpp>

struct LogicComponent {
	// The axis about which the object should rotate
	glm::vec3 rotationAxis;
};