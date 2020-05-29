#pragma once

#include "SteamID.h"

#include <nlohmann/json_fwd.hpp>

#include <filesystem>
#include <optional>
#include <vector>

namespace tf2_bot_detector
{
	enum class PlayerAttributes;
	class IPlayer;

	enum class TriggerMatchMode
	{
		MatchAll,
		MatchAny,
	};

	void to_json(nlohmann::json& j, const TriggerMatchMode& d);
	void from_json(const nlohmann::json& j, TriggerMatchMode& d);

	enum class TextMatchMode
	{
		Equal,
		Contains,
		StartsWith,
		EndsWith,
		Regex,
		Word,
	};

	void to_json(nlohmann::json& j, const TextMatchMode& d);
	void from_json(const nlohmann::json& j, TextMatchMode& d);

	struct TextMatch
	{
		TextMatchMode m_Mode;
		std::vector<std::string> m_Patterns;
		bool m_CaseSensitive = false;

		bool Match(const std::string_view& text) const;
	};

	struct ModerationRule
	{
		std::string m_Description;

		bool Match(const IPlayer& player) const;
		bool Match(const IPlayer& player, const std::string_view& chatMsg) const;

		struct Triggers
		{
			TriggerMatchMode m_Mode = TriggerMatchMode::MatchAll;

			std::optional<TextMatch> m_UsernameTextMatch;
			std::optional<TextMatch> m_ChatMsgTextMatch;
		} m_Triggers;

		struct Actions
		{
			std::vector<PlayerAttributes> m_Mark;
			std::vector<PlayerAttributes> m_Unmark;
		} m_Actions;
	};

	class Settings final
	{
	public:
		Settings();

		void LoadFile();
		void SaveFile() const;

		SteamID m_LocalSteamID;
		bool m_SleepWhenUnfocused = true;
		std::filesystem::path m_TFDir;

		struct Theme
		{
			struct Colors
			{
				float m_ScoreboardCheater[4] = { 1, 0, 1, 1 };
				float m_ScoreboardSuspicious[4] = { 1, 1, 0, 1 };
				float m_ScoreboardExploiter[4] = { 0, 1, 1, 1 };
				float m_ScoreboardRacist[4] = { 1, 1, 1, 1 };
				float m_ScoreboardYou[4] = { 0.5, 1, 0.5, 1 };
			} m_Colors;

		} m_Theme;

		std::vector<ModerationRule> m_Rules;
	};
}