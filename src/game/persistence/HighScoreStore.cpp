#include "HighScoreStore.hpp"
#include <charconv>
#include <fstream>
#include <string>
#include <system_error>
#include <utility>

namespace game::persistence
{
	HighScoreStore::HighScoreStore(std::filesystem::path path) : m_path(std::move(path)) {}

	std::uint32_t	HighScoreStore::load() const
	{
		std::ifstream	input{m_path};
		std::string		serializedScore;

		if (!(input >> serializedScore))
			return 0;

		input >> std::ws;
		if (!input.eof())
			return 0;

		std::uint32_t	score{};
		const char*		begin = serializedScore.data();
		const char*		end = begin + serializedScore.size();
		const auto		result = std::from_chars(begin, end, score);

		if (result.ec != std::errc{} || result.ptr != end)
			return 0;

		return score;
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
