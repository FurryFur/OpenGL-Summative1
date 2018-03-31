//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// Description  : A collection of utility functions that rely on the GLM libary
// Author       : Lance Chaney
// Mail         : lance.cha7337@mediadesign.school.nz
//

#pragma once

namespace GLMUtils {
	// Limits a vector to the specified magnitude
	template <typename VecT>
	VecT limitVec(const VecT& vec, float maxMagnitude)
	{
		auto magnitude = glm::length(vec);
		if (magnitude > maxMagnitude) {
			return glm::normalize(vec) * maxMagnitude;
		}
		return vec;
	}
}