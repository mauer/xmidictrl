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

#include "map_in_sld.h"

// fmt
#include "fmt/format.h"

// XMidiCtrl
#include "toml_utils.h"
#include "utils.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
map_in_sld::map_in_sld(environment& in_env) : map_in_label(in_env)
{}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping type
 */
map_in_type map_in_sld::type()
{
    return map_in_type::slider;
}


/**
 * Read settings from config
 */
void map_in_sld::read_config(text_logger& in_log, toml::value& in_data, toml::value& in_config)
{
    in_log.debug_line(in_data.location().line(), "Read settings for type 'sld'");
    map_in_label::read_config(in_log, in_data, in_config);

    // check if dataref was defined
    if (toml_utils::contains(in_log, in_data, c_cfg_dataref)) {
        in_log.debug_line(in_data.location().line(), "Use 'dataref' mode for slider mapping");

        m_dataref = toml_utils::read_string(in_log, in_data, c_cfg_dataref);

        m_value_min = toml_utils::read_float(in_log, in_data, c_cfg_value_min, 0.0f);
        m_value_max = toml_utils::read_float(in_log, in_data, c_cfg_value_max, 1.0f);
    } else {
        in_log.debug_line(in_data.location().line(), "Use 'command' mode for slider mapping");

        m_command_up = toml_utils::read_string(in_log, in_data, c_cfg_command_up);
        m_command_middle = toml_utils::read_string(in_log, in_data, c_cfg_command_middle);
        m_command_down = toml_utils::read_string(in_log, in_data, c_cfg_command_down);
    }

    // read data 2 configuration
    m_data_2_min = toml_utils::read_midi_value(in_log, in_data, c_cfg_data_2_min, MIDI_DATA_2_MIN);
    m_data_2_max = toml_utils::read_midi_value(in_log, in_data, c_cfg_data_2_max, MIDI_DATA_2_MAX);

    m_data_2_margin = toml_utils::read_midi_value(in_log, in_data, c_cfg_data_2_margin, 10);
}


/**
 * Check the mapping
 */
bool map_in_sld::check(text_logger& in_log, const device_settings& in_dev_settings)
{
    bool result = true;

    if (!map::check(in_log, in_dev_settings))
        result = false;

    if (!m_dataref.empty()) {
        // dataref mode
        if (!env().drf().check(m_dataref)) {
            in_log.error(source_line());
            in_log.error(fmt::format(" --> Dataref '{}' not found", m_dataref));
            result = false;
        }

        // value min/max
        if (m_value_min == m_value_max) {
            in_log.error(source_line());
            in_log.error(fmt::format(" --> Parameter '{}' is equal to parameter '{}", c_cfg_value_min, c_cfg_value_max));
            result = false;
        }
    } else {
        // command mode
        if (m_command_up.empty() && m_command_down.empty()) {
            in_log.error(source_line());
            in_log.error(fmt::format(" --> Parameters '{}' and '{}' are not defined", c_cfg_command_up, c_cfg_command_down));
            result = false;
        }
    }

    if (m_data_2_min >= m_data_2_max) {
        in_log.error(source_line());
        in_log.error(fmt::format(" --> Parameter '{}' ({:d}) has to be smaller than parameter '{}' ({:d})", c_cfg_data_2_min, m_data_2_min, c_cfg_data_2_max, m_data_2_max));
        result = false;
    }

    if (m_data_2_margin > 25) {
        in_log.error(source_line());
        in_log.error(fmt::format(" --> Parameter '{}' has to be between 0 and 25", c_cfg_data_2_margin));
        result = false;
    }

    return result;
}


/**
 * Execute the action in X-Plane
 */
std::unique_ptr<map_result> map_in_sld::execute(map_param* in_param)
{
	auto result = std::make_unique<map_result>();
	result->completed = true;

	auto param_in = get_param_in(in_param);
	if (param_in == nullptr)
		return result;

    if (!check_sublayer(param_in->sl_value()))
        return result;

    if (!m_dataref.empty()) {
        // dataref mode
        float value;

        if (param_in->msg().data_2() == m_data_2_min)
            value = m_value_min;
        else if (param_in->msg().data_2() == m_data_2_max)
            value = m_value_max;
        else
            value = ((m_value_max - m_value_min) * (static_cast<float>(param_in->msg().data_2()) / 127.0f)) + m_value_min;

		param_in->msg().log().debug(fmt::format(" --> Set dataref '{}' to value '{}'", m_dataref, std::to_string(value)));

        if (env().drf().write(param_in->msg().log(), m_dataref, value)) {
            try {
                display_label(param_in->msg().log(), std::to_string(value));
            } catch (std::bad_alloc& ex) {
				param_in->msg().log().error(fmt::format("Error converting float '{}' value to string", std::to_string(value)));
				param_in->msg().log().error(ex.what());
            }
        }
    } else {
        // command mode
        if (param_in->msg().data_2() <= m_data_2_min + m_data_2_margin) {
			if (m_command_down != m_command_down_prev) {
				param_in->msg().log().debug(fmt::format(" --> Execute command '{}'", m_command_down));

				env().cmd().execute(param_in->msg().log(), m_command_down);
				m_command_down_prev = m_command_down;

				m_command_middle_prev.clear();
				m_command_up_prev.clear();
			}
        } else if (param_in->msg().data_2() >= m_data_2_max - m_data_2_margin) {
            if (m_command_up != m_command_up_prev) {
				param_in->msg().log().debug(fmt::format(" --> Execute command '{}'", m_command_up));

				env().cmd().execute(param_in->msg().log(), m_command_up);
				m_command_up_prev = m_command_up;

				m_command_middle_prev.clear();
				m_command_down_prev.clear();
			}
        } else if (param_in->msg().data_2() >= (m_data_2_max - m_data_2_min) / 2 - m_data_2_margin &&
				   param_in->msg().data_2() <= (m_data_2_max - m_data_2_min) / 2 + m_data_2_margin) {
            if (!m_command_middle.empty()) {
                if (m_command_middle != m_command_middle_prev) {
					param_in->msg().log().debug(fmt::format(" --> Execute command '{}'", m_command_middle));

					env().cmd().execute(param_in->msg().log(), m_command_middle);
					m_command_middle_prev = m_command_middle;

					m_command_down_prev.clear();
					m_command_up_prev.clear();
				}
            }
        } else {
            // clear previous commands
            m_command_up_prev.clear();
			m_command_middle_prev.clear();
			m_command_down_prev.clear();
        }
    }

    return result;
}


/**
 * Return mapped command
 */
std::string map_in_sld::map_text_cmd_drf()
{
    std::string map_str {};

    if (!m_dataref.empty()) {
        map_str.append(m_dataref);
    } else {
        map_str.append(m_command_up + " (up)");

        if (!m_command_middle.empty())
            map_str.append("\n" + m_command_middle + " (middle)");

        map_str.append("\n" + m_command_down + " (down)");
    }

    return map_str;
}


/**
 * Return mapped parameter
 */
std::string map_in_sld::map_text_parameter()
{
    std::string map_str {};

    if (!m_dataref.empty()) {
        map_str.append(fmt::format("Value min = {}", m_value_min));
        map_str.append("   |   ");
        map_str.append(fmt::format("Value max = {}", m_value_max));
    }

    return map_str;
}




//---------------------------------------------------------------------------------------------------------------------
//   PROTECTED
//---------------------------------------------------------------------------------------------------------------------

/**
 * Return the mapping as string
 */
std::string map_in_sld::build_mapping_text(bool in_short)
{
    std::string map_str {};
    std::string sep_str {", "};

    if (!in_short) {
        sep_str = "\n";
        map_str = " ====== Slider ======" + sep_str;
    }

    if (!m_dataref.empty()) {
        map_str.append("Dataref = '" + m_dataref + "'" + sep_str);
        map_str.append(fmt::format("Value min = {}", + m_value_min) + sep_str);
        map_str.append(fmt::format("Value max = {}", m_value_max));
    } else {
        if (!m_command_middle.empty())
            map_str.append("Command down = '" + m_command_down + "'" + sep_str + "Command middle = '" + m_command_middle
                           + "'" + sep_str + "Command up = '" + m_command_up + "'");
        else
            map_str.append("Command down = '" + m_command_down + "'" + sep_str + "Command up = '" + m_command_up + "'");
    }

    return map_str;
}

} // Namespace xmidictrl