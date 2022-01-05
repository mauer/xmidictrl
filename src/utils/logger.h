//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2022 Marco Auer
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

#ifndef LOGGER_H
#define LOGGER_H

// Standard
#include <deque>
#include <fstream>
#include <memory>
#include <string_view>

// XMidiCtrl
#include "log_entry.h"
#include "midi_log_msg.h"
#include "midi_message.h"
#include "settings.h"
#include "text_log_msg.h"
#include "text_message.h"

// Macros for logging
#define LOG_ALL   (xmidictrl::log_entry() << xmidictrl::log_entry::all)
#define LOG_ERROR (xmidictrl::log_entry() << xmidictrl::log_entry::error)
#define LOG_WARN  (xmidictrl::log_entry() << xmidictrl::log_entry::warn)
#define LOG_INFO  (xmidictrl::log_entry() << xmidictrl::log_entry::info)
#define LOG_DEBUG (xmidictrl::log_entry() << xmidictrl::log_entry::debug)

#define LOG_END xmidictrl::log_entry::endLine;

namespace xmidictrl {

class logger {
public:
	logger() = default;
	~logger();

	// Object cannot be copied, because of the stream
    logger(const logger&) = delete;
    logger& operator=(const logger&) = delete;

	static logger& instance();

    void init(std::string_view a_path, std::shared_ptr<settings> a_settings);

	void post_text_message(const std::shared_ptr<text_message> &msg);
    void post_midi_message(const std::shared_ptr<midi_message> &msg);

    log_level logging_Level() const;
    bool log_midi() const;

    void clear_text_messages();
    void clear_midi_messages();

    unsigned int count_text_messages();
    unsigned int count_midi_messages();

    std::shared_ptr<text_log_msg> get_text_message(unsigned int idx);
    std::shared_ptr<midi_log_msg> get_midi_message(unsigned int idx);

private:
    bool check_log_level(text_msg_type type);

    std::shared_ptr<settings> m_settings;

    std::ofstream m_stream;

	std::deque<std::shared_ptr<text_log_msg>> m_text_messages;
    std::deque<std::shared_ptr<midi_log_msg>> m_midi_messages;
};

} // Namespace xmidictrl

#endif // LOGGER_H