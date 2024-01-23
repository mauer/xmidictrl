//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2023 Marco Auer
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

#include "map_in_label.h"

// XMidiCtrl
#include "toml_utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
map_in_label::map_in_label(environment& in_env)
    : map_in(in_env)
{
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Read the config
 */
void map_in_label::read_config(text_logger& in_log, toml::value& in_data, toml::value& in_config)
{
    map_in::read_config(in_log, in_data, in_config);

    // additional config
    read_label(in_log, in_data, in_config);
}


/*
 * Return the label name(s)
 */
std::string map_in_label::map_text_label()
{
    if (m_label == nullptr)
        return {};

    return m_label->id;
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Toggle dataref between values
 */
std::string map_in_label::toggle_dataref(text_logger& in_log, std::string_view in_dataref, std::vector<std::string>& in_values)
{
    auto new_value = map_in::toggle_dataref(in_log, in_dataref, in_values);
    display_label(in_log, new_value);

    return new_value;
}


/**
 * Display the label on the screen
 */
void map_in_label::display_label(text_logger& in_log, float in_value)
{
    std::stringstream ss;
    ss << in_value;

    display_label(in_log, ss.str());
}


/**
 * Display the label on the screen
 */
void map_in_label::display_label(text_logger& in_log, std::string_view in_value)
{
    if (m_label == nullptr || m_label->id.empty()) {
        in_log.debug(" --> No label defined");
        return;
    }

    try {
        std::string value_text = m_label->values.at(in_value.data());
        in_log.debug(" --> Found text '" + value_text + "' for value '" + std::string(in_value) + "'");
        env().show_info_message(m_label->id, m_label->text + value_text);
    } catch (std::out_of_range&) {
        env().show_info_message(m_label->id, m_label->text + in_value.data());
    }
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Read label definition
 */
void map_in_label::read_label(text_logger& in_log, toml::value& in_data, toml::value& in_config)
{
    // is a label defined?
    if (!in_data.contains(c_cfg_label.data()))
        return;

    try {
        std::string label_id = in_data[c_cfg_label.data()].as_string();

        if (label_id.empty()) {
            in_log.error_line(in_data.location().line(), "Error reading mapping");
            in_log.error(" --> Parameter '" + std::string(c_cfg_label.data()) + "' is empty");
            return;
        }

        if (!in_config.contains(label_id)) {
            in_log.error_line(in_data.location().line(), "Error reading mapping");
            in_log.error(" --> Definition for label '" + label_id + "' not found");
            return;
        }

        toml::value label_section = toml::find(in_config, label_id);

        // create label
        m_label = std::make_unique<label>();

        // set the label id
        m_label->id = label_id;

        // read the label text
        m_label->text = toml_utils::read_string(in_log, label_section, CFG_KEY_TEXT);

        // add a space at the end, if required
        if (m_label->text.back() != ' ')
            m_label->text.append(" ");

        // read all label values
        if (toml_utils::contains(in_log, label_section, CFG_KEY_VALUES)) {
            auto values = label_section[CFG_KEY_VALUES].as_array();

            for (auto value: values) {
                std::string value_id = toml_utils::read_string(in_log, value, CFG_KEY_VALUE);
                std::string value_text = toml_utils::read_string(in_log, value, CFG_KEY_TEXT);
                m_label->values.try_emplace(value_id, value_text);
            }
        } else {
            in_log.error_line(in_data.location().line(), "Error reading mapping");
            in_log.error(" --> Parameter '" + std::string(CFG_KEY_VALUES) + "' not found");

            m_label.reset();
        }
    } catch (toml::type_error& error) {
        in_log.error_line(in_data.location().line(), "Error reading mapping");
        in_log.error(error.what());

        m_label.reset();
    }
}

} // Namespace xmidictrl