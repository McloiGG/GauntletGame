#pragma once
#ifndef FRAMECLOCK_HPP
#define FRAMECLOCK_HPP

namespace engine
{
	class FrameClock
	{
	public:
		void	update();
		float	deltaSeconds() const;

	private:
		float	m_deltaSeconds{0.0f};
	};
}

#endif
