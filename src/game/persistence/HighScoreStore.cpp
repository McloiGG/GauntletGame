#include "HighScoreStore.hpp"
#include <cctype>
#include <cstddef>
#include <fstream>
#include <limits>
#include <system_error>
#include <utility>

namespace game::persistence
{
	HighScoreStore::HighScoreStore(std::filesystem::path path) : m_path(std::move(path)) {}

	std::uint32_t	HighScoreStore::load() const
	{
		std::ifstream			input{m_path};
		std::uint32_t			score{};
		std::size_t				digitCount{};
		bool					hasDigit{false};
		bool					hasTrailingWhitespace{false};
		char					character{};
		constexpr std::size_t	maximumDigitCount = std::numeric_limits<std::uint32_t>::digits10 + 1;
		constexpr std::uint32_t	maximumScore = std::numeric_limits<std::uint32_t>::max();

		while (input.get(character))
		{
			if (std::isspace(static_cast<unsigned char>(character)) != 0)
			{
				if (hasDigit)
					hasTrailingWhitespace = true;
				continue;
			}
			if (hasTrailingWhitespace || character < '0' || character > '9')
				return 0;

			hasDigit = true;
			if (++digitCount > maximumDigitCount)
				return 0;

			const std::uint32_t	digit = static_cast<std::uint32_t>(character - '0');

			if (score > (maximumScore - digit) / 10)
				return 0;
			score = score * 10 + digit;
		}

		if (input.bad() || !input.eof())
			return 0;
		return hasDigit ? score : 0;
	}

	bool	HighScoreStore::save(std::uint32_t score) const
	{
		std::error_code				error;
		const std::filesystem::path	directory = m_path.parent_path();

		if (!directory.empty())
			std::filesystem::create_directories(directory, error);
		if (error)
			return false;

		std::ofstream	output{m_path, std::ios::trunc};

		if (!output)
			return false;

		output << score << '\n';
		return static_cast<bool>(output);
	}
}
