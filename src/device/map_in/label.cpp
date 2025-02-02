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

#include "label.h"

// fmt
#include "fmt/format.h"

// XMidiCtrl
#include "toml_utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
label::label(environment& in_env)
	: m_env(in_env)
{}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Read the config
 */
void label::read_config(text_logger& in_log,
						toml::value& in_data,
						toml::value& in_config,
						std::string_view in_dataref,
						std::string_view in_cfg_label)
{
	// is a label defined?
	if (!in_data.contains(in_cfg_label.data()))
		return;

	try {
		std::string label_id = in_data[in_cfg_label.data()].as_string();

		if (label_id.empty()) {
			in_log.error_line(in_data.location().line(), "Error reading mapping");
			in_log.error(fmt::format(" --> Parameter '{}' is empty", in_cfg_label));
			return;
		}

		if (!in_config.contains(label_id)) {
			in_log.error_line(in_data.location().line(), "Error reading mapping");
			in_log.error(fmt::format(" --> Definition for label '{}' not found", label_id));
			return;
		}

		toml::value label_section = toml::find(in_config, label_id);

		// set the label id
		m_id = label_id;

		// read the label dataref (if set)
		if (toml_utils::contains(in_log, label_section, c_cfg_dataref))
			m_dataref = toml_utils::read_string(in_log, label_section, c_cfg_dataref);
		else
			m_dataref = in_dataref;

		// read the label text
		m_text = toml_utils::read_string(in_log, label_section, c_cfg_text);

		// add a space at the end, if required
		if (m_text.back() != ' ')
			m_text.append(" ");

		// read all label values
		if (toml_utils::contains(in_log, label_section, c_cfg_values)) {
			auto values = label_section[c_cfg_values.data()].as_array();

			for (auto value: values) {
				std::string value_id = toml_utils::read_string(in_log, value, c_cfg_value);
				std::string value_text = toml_utils::read_string(in_log, value, c_cfg_text);
				m_values.try_emplace(value_id, value_text);
			}
		}
	} catch (toml::type_error& error) {
		in_log.error_line(in_data.location().line(), "Error reading mapping");
		in_log.error(error.what());
	}
}


/**
 * Check the config
 */
bool label::check(text_logger& in_log, std::string_view in_source_line)
{
	if (m_id.empty())
		return true;

	bool result = true;

	if (m_dataref.empty()) {
		in_log.error(in_source_line);
		in_log.error(fmt::format(" --> Parameter '{}' is empty", c_cfg_dataref));
		result = false;
	} else if (!m_env.drf().check(m_dataref)) {
		in_log.error(in_source_line);
		in_log.error(fmt::format(" --> Dataref '{}' not found", m_dataref));
		result = false;
	}

	return result;
}


/**
 * Display the label on the screen
 */
void label::display_label(xmidictrl::text_logger& in_log)
{
	// is a label defined?
	if (m_id.empty()) {
		in_log.debug(" --> No label defined");
		return;
	}

	// read current dataref value
	std::string value;

	if (!m_env.drf().read(in_log, m_dataref, value))
		return;

	try {
		std::string value_text = m_values.at(value);
		in_log.debug(fmt::format(" --> Found text '{}' for value '{}'", value_text, value));
		m_env.show_info_message(m_id, m_text + value_text);
	} catch (std::out_of_range&) {
		m_env.show_info_message(m_id, m_text + value);
	}
}


/**
 * Return the label id
 */
std::string label::id() const
{
	return m_id;
}

} // Namespace xmidictrl