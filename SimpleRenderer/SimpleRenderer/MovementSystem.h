//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// Description  : A system which handles movement based on an
//                entities input state.
// Author       : Lance Chaney
// Mail         : lance.cha7337@mediadesign.school.nz
//

#pragma once

struct Scene;

class MovementSystem {
public:
	MovementSystem(Scene& scene);

	// Updates the entities position from input
	void update(size_t entityID);

private:
	Scene& m_scene;
};