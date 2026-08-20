#pragma once
#ifndef HIGHSCORESTORE_HPP
#define HIGHSCORESTORE_HPP

#include <cstdint>
#include <filesystem>

namespace game::persistence
{
	class HighScoreStore
	{
	public:
		explicit HighScoreStore(std::filesystem::path path);

		std::uint32_t	load() const;
		bool			save(std::uint32_t score) const;

	private:
		std::filesystem::path	m_path;
	};
}

#endif
