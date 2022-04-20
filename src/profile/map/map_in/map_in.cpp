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

#include "map_in.h"

// XMidiCtrl
#include "plugin.h"
#include "toml_utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
map_in::map_in(xplane &in_xp)
    : map(in_xp)
{
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the sublayer name
 */
std::string_view map_in::sl() const
{
    return m_sl;
}


/**
 * Read the config
 */
void map_in::read_config(text_logger &in_log, toml::value &in_data, toml::value &in_config)
{
    // read the common data
    read_common_config(in_log, in_data);

    // additional config
    read_sublayer(in_log, in_data);
    read_label(in_log, in_data, in_config);
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Check if the command is defined for the current sublayer
 */
bool map_in::check_sublayer(std::string_view in_sl_value)
{
    if (in_sl_value != m_sl && !m_sl.empty())
        return false;

    return true;
}


/**
 * Toggle dataref between values
 */
void map_in::toggle_dataref(text_logger &in_log, std::string_view in_dataref, std::vector<std::string> &in_values)
{
    if (in_values.size() == 2) {
        auto value = xp().datarefs().toggle(in_log, in_dataref, in_values[0], in_values[1]);
        display_label(in_log, value);
    } else {
        // get current value
        std::string value;
        xp().datarefs().read(in_log, in_dataref, value);

        // search for the value in the values list
        auto it = std::find(in_values.begin(), in_values.end(), value);

        if (it != in_values.end()) {
            auto idx = std::distance(in_values.begin(), it);

            if (idx < in_values.size() - 1)
                idx++;
            else
                idx = 0;

            value = in_values[idx];
        } else {
            // value not found, let's take the first one of the list
            value = in_values[0];
        }

        in_log.debug(" --> Change dataref '%s' to value '%s'", in_dataref.data(), value.c_str());

        xp().datarefs().write(in_log, in_dataref, value);
        display_label(in_log, value);
    }
}


/**
 * Display the label on the screen
 */
void map_in::display_label(text_logger &in_log, float in_value)
{
    std::stringstream ss;
    ss << in_value;

    display_label(in_log, ss.str());
}


/**
 * Display the label on the screen
 */
void map_in::display_label(text_logger &in_log, std::string_view in_value)
{
    if (m_label == nullptr) {
        in_log.debug(" --> No label defined");
        return;
    }

    try {
        std::string value_text = m_label->values.at(in_value.data());
        in_log.debug(" --> Found text '%s' for value '%s'", value_text.c_str(), in_value.data());
        plugin::instance().show_info_message(m_label->id, m_label->text + value_text);
    } catch (std::out_of_range &ex) {
        plugin::instance().show_info_message(m_label->id, m_label->text + in_value.data());
    }
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Read parameter sl
 */
void map_in::read_sublayer(text_logger &in_log, toml::value &in_data)
{
    m_sl.clear();

    try {
        // read sublayer
        if (in_data.contains(CFG_KEY_SL)) {
            m_sl = in_data[CFG_KEY_SL].as_string();

            in_log.debug(" --> Line %i :: Parameter '%s' = '%s'", in_data.location().line(), CFG_KEY_SL, m_sl.c_str());
        }
    } catch (toml::type_error &error) {
        in_log.error(" --> Line %i :: Error reading mapping", in_data.location().line());
        in_log.error(error.what());
    }
}


/**
 * Read label definition
 */
void map_in::read_label(text_logger &in_log, toml::value &in_data, toml::value &in_config)
{
    // is a label defined?
    if (!in_data.contains(CFG_KEY_LABEL))
        return;

    try {
        std::string label_id = in_data[CFG_KEY_LABEL].as_string();

        if (label_id.empty()) {
            in_log.error(" --> Line %i :: Error reading mapping", in_data.location().line());
            in_log.error(" --> Parameter '%s' is empty", CFG_KEY_LABEL, label_id.c_str());
            return;
        }

        if (!in_config.contains(label_id)) {
            in_log.error(" --> Line %i :: Error reading mapping", in_data.location().line());
            in_log.error(" --> Definition for label '%s' not found", label_id.c_str());
            return;
        }

        toml::value label_section = toml::find(in_config, label_id);

        // start created the label
        m_label = std::make_unique<label>();

        // read the label text
        m_label->text = toml_utils::read_string(in_log, label_section, CFG_KEY_TEXT);

        // add a space at the end, if required
        if (m_label->text.back() != ' ')
            m_label->text.append(" ");

        // read all label values
        if (toml_utils::contains(in_log, label_section, CFG_KEY_VALUES)) {
            auto values = label_section[CFG_KEY_VALUES].as_array();

            for (auto value : values) {
                std::string value_id = toml_utils::read_string(in_log, value, CFG_KEY_VALUE);
                std::string value_text = toml_utils::read_string(in_log, value, CFG_KEY_TEXT);
                m_label->values.emplace(value_id, value_text);
            }
        } else {
            in_log.error(" --> Line %i :: Error reading mapping", in_data.location().line());
            in_log.error(" --> Parameter '%s' not found", CFG_KEY_VALUES);

            m_label.reset();
        }
    } catch (toml::type_error &error) {
        in_log.error(" --> Line %i :: Error reading mapping", in_data.location().line());
        in_log.error(error.what());

        m_label.reset();
    }
}

} // Namespace xmidictrl