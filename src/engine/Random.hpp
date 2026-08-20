#pragma once
#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <random>

namespace engine
{
	std::uint32_t	deriveSeed(std::uint32_t baseSeed, std::uint32_t stream);

	class Random
	{
	public:
		explicit Random(std::uint32_t seed);

		int			integer(int minimum, int maximum);
		float		real(float minimum, float maximum);
		bool		chance(float probability);

		template<std::size_t Count>
		std::size_t	weightedIndex(const std::array<float, Count>& weights)
		{
			static_assert(Count > 0, "Weighted selection requires at least one weight");

			float			totalWeight{};
			std::size_t	lastPositiveIndex{};

			for (std::size_t index = 0; index < Count; ++index)
			{
				assert(weights[index] >= 0.0f);
				totalWeight += weights[index];
				if (weights[index] > 0.0f)
					lastPositiveIndex = index;
			}
			assert(totalWeight > 0.0f);

			const float	selection = real(0.0f, totalWeight);
			float		cumulativeWeight{};

			for (std::size_t index = 0; index < Count; ++index)
			{
				cumulativeWeight += weights[index];
				if (selection < cumulativeWeight)
					return index;
			}
			return lastPositiveIndex;
		}

	private:
		std::mt19937	m_engine;
	};
}

#endif
