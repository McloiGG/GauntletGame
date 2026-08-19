#pragma once
#ifndef INTERSECTIONS_HPP
#define INTERSECTIONS_HPP

#include "Circle.hpp"

namespace engine::collision
{
	bool	intersects(const Circle& first, const Circle& second);
}

#endif
