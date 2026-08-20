#pragma once
#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <cstddef>
#include <cstdint>
#include <random>
#include <vector>

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
		std::size_t	weightedIndex(const std::vector<float>& weights);

	private:
		std::mt19937	m_engine;
	};
}

#endif
