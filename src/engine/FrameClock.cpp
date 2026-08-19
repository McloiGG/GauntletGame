#include "FrameClock.hpp"
#include <algorithm>
#include "raylib.h"

namespace engine
{
	void	FrameClock::update()
	{
		m_deltaSeconds = std::clamp(GetFrameTime(), 0.0f, 0.05f);
	}

	float	FrameClock::deltaSeconds() const
	{
		return m_deltaSeconds;
	}
}
