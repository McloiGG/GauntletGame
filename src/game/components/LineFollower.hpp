#pragma once
#ifndef LINEFOLLOWER_HPP
#define LINEFOLLOWER_HPP

#include "game/field/FieldLayout.hpp"

namespace game
{
	struct LineFollower
	{
		field::FieldLineId	lineId;
		float				distanceAlongLine{};
		float				targetDistanceAlongLine{};
		float				walkSpeed{};
		float				sprintSpeed{};
		float				exhaustedSpeed{};
		float				sprintPlayerProximity{};
	};
}

#endif
