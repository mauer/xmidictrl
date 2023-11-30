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

#include "map_in_list.h"

// XMidiCtrl
#include "map_in_cmd.h"
#include "map_in_drf.h"
#include "map_in_enc.h"
#include "map_in_pnp.h"
#include "map_in_sld.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Destructor
 */
map_in_list::~map_in_list()
{
    m_list_key.clear();
    m_list_no.clear();
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Create the inbound mapping for a device and store it
 */
void map_in_list::create_mappings(text_logger& in_log,
                                  toml::array in_profile,
                                  environment& in_env,
                                  bool in_is_virtual,
                                  device_settings& in_dev_settings,
                                  std::string_view in_inc_name,
                                  toml::value& in_config)
{
    std::string log_prefix;

    if (in_is_virtual)
        log_prefix = "Virtual Device :: ";
    else
        log_prefix = "Device " + std::to_string(in_dev_settings.device_no) + " :: ";

    in_log.info(log_prefix + std::to_string(in_profile.size()) + " inbound mapping(s) found");

    // parse each mapping entry
    for (int map_no = 0; map_no < static_cast<int>(in_profile.size()); map_no++) {
        std::shared_ptr<map_in> mapping;

        in_log.debug(log_prefix + "Read settings for mapping " + std::to_string(map_no));

        try {
            auto type = read_map_type(in_log, in_profile[map_no]);

            // depending on the mapping type, we have to read some additional settings
            switch (type) {
                case map_in_type::command:
                    mapping = std::make_shared<map_in_cmd>(in_env);
                    break;

                case map_in_type::dataref:
                    mapping = std::make_shared<map_in_drf>(in_env);
                    break;

                case map_in_type::push_pull:
                    mapping = std::make_shared<map_in_pnp>(in_env);
                    break;

                case map_in_type::encoder:
                    if (!in_is_virtual)
                        mapping = std::make_shared<map_in_enc>(in_env, in_dev_settings.default_enc_mode);
                    else
                        in_log.error(log_prefix + "Mapping " + std::to_string(map_no)
                                     + " :: mapping type not supported for virtual devices");
                    break;

                case map_in_type::slider:
                    if (!in_is_virtual)
                        mapping = std::make_shared<map_in_sld>(in_env);
                    else
                        in_log.error(log_prefix + "Mapping " + std::to_string(map_no)
                                     + " :: mapping type not supported for virtual devices");
                    break;

                case map_in_type::none:
                    in_log.error(log_prefix + "Mapping " + std::to_string(map_no)
                                         + " :: Invalid mapping type");
                    break;
            }

            if (mapping == nullptr) {
                in_log.error("Line " + std::to_string(in_profile[map_no].location().line()) + " :: "
                                     + in_profile[map_no].location().line_str());
                in_log.error(" --> Error reading mapping");
                continue;
            }

            // read the settings and check if everything we need was defined
            mapping->read_config(in_log, in_profile[map_no], in_config);
            mapping->set_include_name(in_inc_name);

            if (mapping->check(in_log)) {
                add(mapping);
                in_log.debug(" --> Mapping added");
            } else {
                in_log.error("Line " + std::to_string(in_profile[map_no].location().line()) + " :: "
                                     + in_profile[map_no].location().line_str());
                in_log.error(" --> Parameters incomplete or incorrect");
            }
        } catch (toml::type_error& error) {
            in_log.error("Line " + std::to_string(in_profile[map_no].location().line()) + " :: "
                                 + in_profile[map_no].location().line_str());
            in_log.error(" --> Error reading config");
            in_log.error(error.what());
        }
    }
}


/**
 * Retrieve all mappings for a given key
 */
std::vector<std::shared_ptr<map_in>> map_in_list::find(const std::string& in_key)
{
    std::vector<std::shared_ptr<map_in>> result {};

    auto search_result = m_list_key.equal_range(in_key);

    for (auto it = search_result.first; it != search_result.second; it++)
        result.push_back(it->second);

    return result;
}


/**
 * Return the number of mapping
 */
size_t map_in_list::size()
{
    return m_list_key.size();
}


/**
 * Return the begin iterator
 */
std::multimap<unsigned int, std::shared_ptr<map_in>>::iterator map_in_list::begin()
{
    return m_list_no.begin();
}


/**
 * Return the end iterator
 */
std::multimap<unsigned int, std::shared_ptr<map_in>>::iterator map_in_list::end()
{
    return m_list_no.end();
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Read the inbound mapping type
 */
map_in_type map_in_list::read_map_type(text_logger& in_log, toml::value& in_params)
{
    map_in_type type = map_in_type::none;

    try {
        // read type
        if (in_params.contains(c_cfg_type.data())) {
            std::string type_str {in_params[c_cfg_type.data()].as_string()};

            in_log.debug_line(in_params.location().line(), "Parameter type = '" + type_str + "'");

            // get the mapping type
            type = translate_map_type(type_str);
        } else {
            in_log.error(
                "Line " + std::to_string(in_params.location().line()) + " :: " + in_params.location().line_str());
            in_log.error(" --> Parameter '" + std::string(c_cfg_type.data()) + "' is missing");
        }
    } catch (toml::type_error& error) {
        in_log.error(
            "Line " + std::to_string(in_params.location().line()) + " :: " + in_params.location().line_str());
        in_log.error("Line " + std::to_string(in_params.location().line()) + " :: Error reading mapping");
        in_log.error(error.what());
    }

    return type;
}


/**
 * Translate the type string for inbound mappings to an enum value
 */
map_in_type map_in_list::translate_map_type(std::string_view in_type_str)
{
    map_in_type type = map_in_type::none;

    if (in_type_str == CFG_MAPTYPE_COMMAND)
        type = map_in_type::command;
    else if (in_type_str == CFG_MAPTYPE_SLIDER)
        type = map_in_type::slider;
    else if (in_type_str == CFG_MAPTYPE_DATAREF)
        type = map_in_type::dataref;
    else if (in_type_str == CFG_MAPTYPE_PUSH_PULL)
        type = map_in_type::push_pull;
    else if (in_type_str == CFG_MAPTYPE_ENCODER)
        type = map_in_type::encoder;

    return type;
}


/**
 * Add a new mapping
 */
void map_in_list::add(const std::shared_ptr<map_in>& in_map)
{
    m_last_map_no++;
    in_map->set_no(m_last_map_no);

    m_list_key.insert({in_map->get_key(), in_map});
    m_list_no.insert({in_map->no(), in_map});
}

} // Namespace xmidictrl