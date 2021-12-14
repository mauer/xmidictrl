//---------------------------------------------------------------------------------------------------------------------
//   MIT License
//
//   Copyright (c) 2021 Marco Auer
//
//   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
//   documentation files (the "Software"), to deal in the Software without restriction, including without limitation
//   the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
//   to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
//   The above copyright notice and this permission notice shall be included in all copies or substantial portions of
//   the Software.
//
//   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//   THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
//   CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//   IN THE SOFTWARE.
//---------------------------------------------------------------------------------------------------------------------

#ifndef LOGGER_H
#define LOGGER_H

// Standard
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

    log_level get_log_Level() const;
    bool get_log_midi() const;

    void clear_text_messages();
    void clear_midi_messages();

    unsigned int count_text_messages();
    unsigned int count_midi_messages();

    std::shared_ptr<text_log_msg> get_text_message(int idx);
    std::shared_ptr<midi_log_msg> get_midi_message(int idx);

private:
    bool check_log_level(text_msg_type type);

    std::shared_ptr<settings> m_settings;

    std::ofstream m_stream;

	std::vector<std::shared_ptr<text_log_msg>> m_text_messages;
    std::vector<std::shared_ptr<midi_log_msg>> m_midi_messages;
};

} // Namespace xmidictrl

#endif // LOGGER_H