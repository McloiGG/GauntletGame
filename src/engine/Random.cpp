#include "Random.hpp"
#include <cassert>

namespace engine
{
	std::uint32_t	deriveSeed(std::uint32_t baseSeed, std::uint32_t stream)
	{
		std::uint32_t	value = baseSeed ^ stream;

		value ^= value >> 16;
		value *= 0x7FEB352Du;
		value ^= value >> 15;
		value *= 0x846CA68Bu;
		value ^= value >> 16;
		return value;
	}

	Random::Random(std::uint32_t seed) : m_engine(seed) {}

	int	Random::integer(int minimum, int maximum)
	{
		assert(minimum <= maximum);
		std::uniform_int_distribution<int>	distribution(minimum, maximum);

		return distribution(m_engine);
	}

	float	Random::real(float minimum, float maximum)
	{
		assert(minimum <= maximum);
		std::uniform_real_distribution<float>	distribution(minimum, maximum);

		return distribution(m_engine);
	}

	bool	Random::chance(float probability)
	{
		assert(probability >= 0.0f && probability <= 1.0f);
		std::bernoulli_distribution	distribution(probability);

		return distribution(m_engine);
	}
}
