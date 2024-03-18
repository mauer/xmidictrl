//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2024 Marco Auer
//
//   XMidiCtrl is free software: you can redistribute it and/or modify it under the terms of the
//   GNU Affero General Public License as published by the Free Software Foundation, either version 3
//   of the License, or (at your option) any later version.
//
//   XMidiCtrl is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
//   the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU Affero General Public License for more details.
//
//   You should have received a copy of the GNU Affero General Public License along with XMidiCtrl.
//   If not, see <https://www.gnu.org/licenses/>.
//---------------------------------------------------------------------------------------------------------------------

#ifndef XMC_TEXT_LOGGER_H
#define XMC_TEXT_LOGGER_H

// Standard
#include <cstdarg>
#include <deque>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <string_view>

// XMidiCtrl
#include "text_log_msg.h"
#include "types.h"

namespace xmidictrl {

class text_logger {
public:
	explicit text_logger(text_logger* in_parent = nullptr);
	~text_logger() = default;

	void enable_file_logging(const std::filesystem::path& in_path);

	void set_debug_mode(bool in_mode);
	[[nodiscard]] bool debug_mode() const;

	void set_log_info(bool in_mode);

	void clear();
	size_t count();
	text_log_msg* message(size_t in_index);

	std::string messages_as_text();

	[[nodiscard]] bool has_errors() const;
	[[nodiscard]] bool has_warnings() const;

	void debug(std::string_view in_text);
	void debug_line(std::uint_least32_t in_line, std::string_view in_text);
	void debug_param(std::uint_least32_t in_line, std::string_view in_param, std::string_view in_value);

	void info(std::string_view in_text);

	void warn(std::string_view in_text);
	void warn_line(std::uint_least32_t in_line, std::string_view in_text);

	void error(std::string_view in_text);
	void error_line(std::uint_least32_t in_line, std::string_view in_text);

private:
	[[nodiscard]] bool check_log_level(log_level in_level) const;

	void create_message(log_level in_level, std::string_view in_text);
	void add_message(log_level in_level, std::string_view in_text);

	text_logger* m_parent;

	int m_error_count {0};
	int m_warn_count {0};

	bool m_debug_mode {false};
	bool m_log_info {true};

	std::ofstream m_file_stream;

	std::deque<std::shared_ptr<text_log_msg>> m_messages;
};

} // Namespace xmidictrl

#endif // XMC_TEXT_LOGGER_H
