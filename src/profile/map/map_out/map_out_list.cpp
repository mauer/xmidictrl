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

#include "map_out_list.h"

// XMidiCtrl
#include "map_out_con.h"
#include "map_out_drf.h"
#include "map_out_sld.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Destructor
 */
map_out_list::~map_out_list()
{
    m_list.clear();
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create the outbound mappings for a device and store it
 */
void map_out_list::create_mappings(text_logger& in_log,
                                   toml::array in_profile,
                                   environment& in_env,
                                   const device_settings& in_dev_settings,
                                   std::string_view in_inc_name)
{
    auto log_prefix = "Device " + std::to_string(in_dev_settings.device_no) + " :: ";

    in_log.info(log_prefix + std::to_string(in_profile.size()) + " outbound mapping(s) found");

    // parse each mapping entry
    for (int map_no = 0; map_no < static_cast<int>(in_profile.size()); map_no++) {
        std::shared_ptr<map_out> mapping;

        in_log.debug(log_prefix + "Mapping " + std::to_string(map_no) + " :: Reading config");

        try {
            map_out_type type = read_map_type(in_log, in_profile[map_no]);

            // depending on the mapping type, we have to read some additional settings
            switch (type) {
                case map_out_type::constant:
                    mapping = std::make_shared<map_out_con>(in_env);
                    break;

                case map_out_type::dataref:
                    mapping = std::make_shared<map_out_drf>(in_env);
                    break;

                case map_out_type::slider:
                    mapping = std::make_shared<map_out_sld>(in_env);
                    break;

                default:
                    in_log.error(log_prefix + "Mapping " + std::to_string(map_no)
                                 + " :: Invalid mapping type");
                    break;
            }

            if (mapping == nullptr) {
                in_log.error(log_prefix + "Mapping " + std::to_string(map_no) + " :: Error reading config");
                continue;
            }

            // read the settings and check if everything we need was defined
            mapping->read_config(in_log, in_profile[map_no]);
            mapping->set_include_name(in_inc_name);

            if (mapping->check(in_log)) {
                add(mapping);
                in_log.debug(" --> Mapping added");
            } else {
                in_log.error(log_prefix + "Mapping " + std::to_string(map_no)
                             + " :: Parameters incomplete or incorrect");
            }
        } catch (toml::type_error& error) {
            in_log.error(log_prefix + "Mapping " + std::to_string(map_no)
                         + " :: Error reading mapping");
            in_log.error(error.what());
        }
    }
}


/**
 * Return the begin iterator
 */
map_out_itr map_out_list::begin()
{
    return m_list.begin();
}


/**
 * Return the end iterator
 */
map_out_itr map_out_list::end()
{
    return m_list.end();
}


/**
 * Return the number of mapping
 */
size_t map_out_list::size()
{
    return m_list.size();
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Read the outbound mapping type
 */
map_out_type map_out_list::read_map_type(text_logger& in_log, toml::value& in_profile)
{
    map_out_type type = map_out_type::none;

    try {
        // read type
        if (in_profile.contains(c_cfg_type.data())) {
            std::string type_str {in_profile[c_cfg_type.data()].as_string()};

            in_log.debug_line(in_profile.location().line(), "Parameter type = '" + type_str + "'");

            // get the mapping type
            type = translate_map_type(type_str);
        } else {
            in_log.error(
                "Line " + std::to_string(in_profile.location().line()) + " :: " + in_profile.location().line_str());
            in_log.error(" --> Parameter '" + std::string(c_cfg_type.data()) + "' is missing");
        }
    } catch (toml::type_error& error) {
        in_log.error(
            "Line " + std::to_string(in_profile.location().line()) + " :: " + in_profile.location().line_str());
        in_log.error("Line " + std::to_string(in_profile.location().line()) + " :: Error reading mapping");
        in_log.error(error.what());
    }

    return type;
}


/**
 * Translate the type string for inbound mappings to an enum value
 */
map_out_type map_out_list::translate_map_type(std::string_view in_type_str)
{
    map_out_type type = map_out_type::none;

    if (in_type_str == CFG_MAPTYPE_CONSTANT)
        type = map_out_type::constant;
    else if (in_type_str == CFG_MAPTYPE_SLIDER)
        type = map_out_type::slider;
    else if (in_type_str == CFG_MAPTYPE_DATAREF)
        type = map_out_type::dataref;

    return type;
}


/**
 * Add a new mapping
 */
void map_out_list::add(const std::shared_ptr<map_out>& in_map)
{
    m_last_map_no++;
    in_map->set_no(m_last_map_no);

    m_list.push_back(in_map);
}

} // Namespace xmidictrl