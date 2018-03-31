//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// Description  : A container for all the entities / components in 
//                scene.
// Author       : Lance Chaney
// Mail         : lance.cha7337@mediadesign.school.nz
//

#pragma once

#include "InputComponent.h"
#include "MeshComponent.h"
#include "MaterialComponent.h"
#include "MovementComponent.h"
#include "LogicComponent.h"

#include <glm\glm.hpp>

#include <vector>

enum ComponentMask {
	COMPONENT_NONE = 0,
	COMPONENT_TRANSFORM = 1 << 0,
	COMPONENT_VELOCITY = 1 << 1,
	COMPONENT_ANGULAR_VELOCITY = 1 << 2,
	COMPONENT_MESH = 1 << 3,
	COMPONENT_MATERIAL = 1 << 4,
	COMPONENT_CAMERA = 1 << 5,
	COMPONENT_MOVEMENT = 1 << 6,
	COMPONENT_INPUT = 1 << 7,
	COMPONENT_LOGIC = 1 << 8
};

struct Scene {
	std::vector<size_t> componentMasks;
	std::vector<glm::mat4> transformComponents;
	std::vector<glm::vec3> velocityComponents;
	std::vector<glm::vec3> angualarVelocityComponent;
	std::vector<MeshComponent> meshComponents;
	std::vector<MaterialComponent> materialComponents;
	std::vector<MovementComponent> movementComponents;
	std::vector<InputComponent> inputComponents;
	std::vector<LogicComponent> logicComponents;
};