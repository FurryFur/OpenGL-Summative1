//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// Description  : The main game logic goes here.
//                This is class is intended to contain all non-portable code.
// Author       : Lance Chaney
// Mail         : lance.cha7337@mediadesign.school.nz
//

#pragma once

#include "KeyObserver.h"

struct Scene;
class InputSystem;

class GameplayLogicSystem : IKeyObserver {
public:
	GameplayLogicSystem(Scene& scene, InputSystem& inputSystem);

	// Runs the logic system on the specified entity
	void update(size_t entityID);

	// Callback for observing key press events
	virtual void keyCallback(int key, int scancode, int action, int mods) override;

private:
	Scene& m_scene;
	size_t m_oldPossessedEntity;
	size_t m_possessedEntity;
	bool m_oldPossessedEntityUpdated;
	bool m_newPossessedEntityUpdated;

	// Change in object rotation per frame
	float m_dTheta;
};