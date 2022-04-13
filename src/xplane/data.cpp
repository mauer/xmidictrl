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

#include "data.h"

#include <sstream>
#include <cstring>

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Check if a dataref is valid
 */
bool data::check(std::string_view in_name)
{
    data_item *item = retrieve_data(in_name);

    if (item == nullptr)
        return false;
    else
        return true;
}


/**
 * Read a dataref
 */
bool data::read(text_logger &in_log, std::string_view in_name, std::string &out_value)
{
    out_value.assign(std::string());

    data_item *item = retrieve_data(in_name);

    if (item == nullptr)
        return false;

    // check the dataref type
    if (item->type & xplmType_Int) {
        int i = read_int(item);
        out_value.assign(std::to_string(i));

        return true;

    } else if (item->type & xplmType_Float) {
        std::stringstream ss;

        ss << read_float(item);
        out_value.assign(ss.str());

        return true;

    } else if (item->type & xplmType_Double) {
        std::stringstream ss;

        ss << read_double(item);
        out_value.assign(ss.str());

        return true;

    } else if (item->type & xplmType_Data) {
        out_value.assign(read_byte(item));

        return true;

    } else if (item->type & xplmType_IntArray) {
        int i = read_int_array(in_log, item, get_index(in_log, in_name));
        out_value.assign(std::to_string(i));

        return true;

    } else if (item->type & xplmType_FloatArray) {
        std::stringstream ss;

        ss << read_float_array(in_log, item, get_index(in_log, in_name));
        out_value.assign(ss.str());

        return true;

    } else if (item->type & xplmType_Unknown) {
        in_log.error("Could not determine type for dataref '%s'", in_name.data());
        return false;

    } else {
        in_log.error("Unknown type '%i' for dataref '%s'", item->type, in_name.data());
        return false;
    }
}


/**
 * Read a numeric dataref
 */
bool data::read(text_logger &in_log, std::string_view in_name, float &out_value)
{
    out_value = 0.0f;

    data_item *item = retrieve_data(in_name);

    if (item == nullptr)
        return false;

    // check the dataref type
    if (item->type & xplmType_Int) {
        int i = read_int(item);
        out_value = static_cast<float>(i);

        return true;

    } else if (item->type & xplmType_Float) {
        out_value = read_float(item);

        return true;

    } else if (item->type & xplmType_Double) {
        double d = read_double(item);
        out_value = static_cast<float>(d);

        return true;

    } else if (item->type & xplmType_Data) {
        in_log.error("Dataref '%s' is not numeric", in_name.data());
        return false;

    } else if (item->type & xplmType_IntArray) {
        int i = read_int_array(in_log, item, get_index(in_log, in_name));
        out_value = static_cast<float>(i);

        return true;

    } else if (item->type & xplmType_FloatArray) {
        out_value = read_float_array(in_log, item, get_index(in_log, in_name));

        return true;

    } else if (item->type & xplmType_Unknown) {
        in_log.error("Could not determine type for dataref '%s'", in_name.data());
        return false;

    } else {
        in_log.error("Unknown type '%i' for dataref '%s'", item->type, in_name.data());
        return false;
    }
}


/**
 * Read a dataref as float array
 */
bool data::read(text_logger &in_log, std::string_view in_name, std::vector<float> &out_values)
{
    data_item *item = retrieve_data(in_name);
    out_values = {};

    if (item == nullptr)
        return false;

    if (item->type & xplmType_FloatArray) {
        out_values = read_float_array(item);
        return true;

    } else {
        in_log.error("Could not read dataref '%s' as 'FloatArray'", in_name.data());
        return false;
    }
}


/**
 * Read a dataref as integer array
 */
bool data::read(text_logger &in_log, std::string_view in_name, std::vector<int> &out_values)
{
    data_item *item = retrieve_data(in_name);
    out_values = {};

    if (item == nullptr)
        return false;

    if (item->type & xplmType_IntArray) {
        out_values = read_int_array(item);
        return true;

    } else {
        in_log.error("Could not read dataref '%s' as 'IntArray'", in_name.data());
        return false;
    }
}


/**
 * Write an value to a dataref
 */
bool data::write(text_logger &in_log, std::string_view in_name, std::string_view in_value)
{
    data_item *item = retrieve_data(in_name);

    if (item == nullptr)
        return false;

    if (!item->writeable) {
        in_log.error("Dataref '%s' is not writeable", in_name.data());
        return false;
    }

    in_log.debug(" --> Set dataref '%s' to value '%s'", in_name.data(), in_value.data());

    try {
        if (item->type & xplmType_Int) {
            write_int(item, std::stoi(in_value.data()));
            return true;
        } else if (item->type & xplmType_Float) {
            write_float(item, std::stof(in_value.data()));
            return true;
        } else if (item->type & xplmType_Double) {
            write_double(item, std::stod(in_value.data()));
            return true;
        } else if (item->type & xplmType_Data) {
            write_byte(item, in_value);
            return true;
        } else if (item->type & xplmType_IntArray) {
            write_int_array(item, get_index(in_log, in_name), std::stoi(in_value.data()));
            return true;
        } else if (item->type & xplmType_FloatArray) {
            write_float_array(item, get_index(in_log, in_name), std::stof(in_value.data()));
            return true;
        } else if (item->type & xplmType_Unknown) {
            in_log.error("Could not determine type for dataref '%s'", in_name.data());
            return false;
        } else {
            in_log.error("Unknown type '%i' for dataref '%s'", item->type, in_name.data());
            return false;
        }
    } catch (std::invalid_argument &ex) {
        in_log.error("Error writing dataref '%s'", in_name.data());
        in_log.error(ex.what());
    } catch (std::out_of_range &ex) {
        in_log.error("Error writing dataref '%s'", in_name.data());
        in_log.error(ex.what());
    }

    return false;
}


/**
 * Write an value to a numeric dataref
 */
bool data::write(text_logger &in_log, std::string_view in_name, float in_value)
{
    data_item *item = retrieve_data(in_name);

    if (item == nullptr)
        return false;

    if (!item->writeable) {
        in_log.error("Dataref '%s' is not writeable", in_name.data());
        return false;
    }

    in_log.debug(" --> Set dataref '%s' to value '%f'", in_name.data(), in_value);

    if (item->type & xplmType_Int) {
        write_int(item, static_cast<int>(in_value));
        return true;
    } else if (item->type & xplmType_Float) {
        write_float(item, in_value);
        return true;
    } else if (item->type & xplmType_Double) {
        write_double(item, static_cast<double>(in_value));
        return true;
    } else if (item->type & xplmType_Data) {
        in_log.error("Dataref '%s' is not numeric", in_name.data());
        return false;
    } else if (item->type & xplmType_IntArray) {
        write_int_array(item, get_index(in_log, in_name), static_cast<int>(in_value));
        return true;
    } else if (item->type & xplmType_FloatArray) {
        write_float_array(item, get_index(in_log, in_name), in_value);
        return true;
    } else if (item->type & xplmType_Unknown) {
        in_log.error("Could not determine type for dataref '%s'", in_name.data());
        return false;
    } else {
        in_log.error("Unknown type '%i' for dataref '%s'", item->type, in_name.data());
        return false;
    }
}


/**
 * Toggle a dataref between on and off
 */
std::string data::toggle(text_logger &in_log,
                         std::string_view in_name,
                         std::string_view in_value_on,
                         std::string_view in_value_off)
{
    data_item *item = retrieve_data(in_name);

    if (item == nullptr)
        return {};

    if (!item->writeable) {
        in_log.error("Dataref '%s' is not writeable", in_name.data());
        return {};
    }

    if (item->type & xplmType_Int) {
        return toggle_int(in_log, item, in_value_on, in_value_off);
    } else if (item->type & xplmType_Float) {
        return toggle_float(in_log, item, in_value_on, in_value_off);
    } else if (item->type & xplmType_Double) {
        return toggle_double(in_log, item, in_value_on, in_value_off);
    } else if (item->type & xplmType_Data) {
        return toggle_byte(in_log, item, in_value_on, in_value_off);
    } else if (item->type & xplmType_IntArray) {
        return toggle_int_array(in_log, item, get_index(in_log, in_name), in_value_on, in_value_off);
    } else if (item->type & xplmType_FloatArray) {
        return toggle_float_array(in_log, item, get_index(in_log, in_name), in_value_on, in_value_off);
    } else if (item->type & xplmType_Unknown) {
        in_log.error("Could not determine type for dataref '%s'", in_name.data());
        return {};
    } else {
        in_log.error("Unknown type '%i' for dataref '%s'", item->type, in_name.data());
        return {};
    }
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Get the data ref for a dataref string
 */
data_item *data::retrieve_data(std::string_view in_name)
{
    std::string dataref_name = in_name.data();

    // is the dataref an array?
    if (dataref_name.back() == ']') {
        // remove index from name
        dataref_name = dataref_name.substr(0, dataref_name.find('['));
    }

    if (m_data_cache.count(dataref_name) == 0) {
        std::unique_ptr<data_item> item = std::make_unique<data_item>();

        item->dataref = XPLMFindDataRef(dataref_name.c_str());
        item->name = dataref_name;
        item->type = xplmType_Unknown;

        if (item->dataref == nullptr)
            return {};

        item->type = XPLMGetDataRefTypes(item->dataref);
        item->writeable = XPLMCanWriteDataRef(item->dataref);

        // in case of a byte ref, we have to determine to size
        if (item->type & xplmType_Data)
            item->size = XPLMGetDatab(item->dataref, nullptr, 0, 0);

        // add new dataref to cache
        m_data_cache.emplace(dataref_name, std::move(item));
    }

    return m_data_cache.at(dataref_name).get();
}


/**
 * Extract the index from the name
 */
int data::get_index(text_logger &in_log, std::string_view in_name)
{
    int index = -1;

    // is the dataref an array?
    if (in_name.back() == ']') {
        int pos = static_cast<int>(in_name.find('['));

        // read index
        try {
            index = std::stoi(in_name.substr(pos + 1, in_name.find(']') - pos - 1).data());
        } catch (std::invalid_argument &ex) {
            in_log.error("Error reading index from dataref name '%s'", in_name.data());
            in_log.error(ex.what());
        } catch (std::out_of_range const &ex) {
            in_log.error("Error reading index from dataref name '%s'", in_name.data());
            in_log.error(ex.what());
        }
    }

    return index;
}


/**
 * Read a integer dataref
 */
int data::read_int(const data_item *item)
{
    // read current value
    return XPLMGetDatai(item->dataref);
}


/**
 * Read a float dataref
 */
float data::read_float(const data_item *item)
{
    // read current value
    return XPLMGetDataf(item->dataref);
}


/**
 * Read a double dataref
 */
double data::read_double(const data_item *item)
{
    // read current value
    return XPLMGetDatad(item->dataref);
}


/**
 * Read a byte dataref
 */
std::string data::read_byte(const data_item *item)
{
    std::vector<char> tmpValue(item->size);
    XPLMGetDatab(item->dataref, &tmpValue[0], 0, static_cast<int>(tmpValue.size()));

    return {tmpValue.data(), strnlen(tmpValue.data(), tmpValue.size())};
}


/**
 * Write an integer dataref
 */
void data::write_int(const data_item *item, int value)
{
    XPLMSetDatai(item->dataref, value);
}


/**
 * Write an integer array dataref
 */
void data::write_int_array(const data_item *item, int index, int value)
{
    if (index < 0)
        return;

    XPLMSetDatavi(item->dataref, &value, index, 1);
}


/**
 * Write a float dataref
 */
void data::write_float(const data_item *item, float value)
{
    XPLMSetDataf(item->dataref, value);
}


/**
 * Write a float array dataref
 */
void data::write_float_array(const data_item *item, int index, float value)
{
    if (index < 0)
        return;

    XPLMSetDatavf(item->dataref, &value, index, 1);
}


/**
 * Write a double dataref
 */
void data::write_double(const data_item *item, double value)
{
    XPLMSetDatad(item->dataref, value);
}


/**
 * Write a byte dataref
 */
void data::write_byte(const data_item *item, std::string_view value)
{
    XPLMSetDatab(item->dataref, const_cast<char *>(value.data()), 0, static_cast<int>(value.size()) + 1);
}


/**
 * Read a float array dataref by index
 */
float data::read_float_array(text_logger &in_log, const data_item *in_item, int in_index)
{
    float value = 0.0f;

    if (in_index > -1)
        XPLMGetDatavf(in_item->dataref, &value, in_index, 1);
    else
        in_log.error("Invalid index '%i' for dataref '%s'", in_index, in_item->name.c_str());

    return value;
}


/**
 * Read a float array dataref
 */
std::vector<float> data::read_float_array(const data_item *item)
{
    std::vector<float> values {};

    int size = XPLMGetDatavf(item->dataref, nullptr, 0, 0);

    values.resize(size);
    XPLMGetDatavf(item->dataref, &values[0], 0, size);

    return values;
}


/**
 * Read an integer array dataref by index
 */
int data::read_int_array(text_logger &in_log, const data_item *in_item, int in_index)
{
    int value = 0;

    if (in_index > -1)
        XPLMGetDatavi(in_item->dataref, &value, in_index, 1);
    else
        in_log.error("Invalid index '%i' for dataref '%s'", in_index, in_item->name.c_str());

    return value;
}


/**
 * Read an integer array dataref
 */
std::vector<int> data::read_int_array(const data_item *item)
{
    std::vector<int> values {};

    values.resize(XPLMGetDatavi(item->dataref, nullptr, 0, 0));
    XPLMGetDatavi(item->dataref, &values[0], 0, static_cast<int>(values.size()));

    return values;
}


/**
 * Toggle an integer dataref between on and off
 */
std::string data::toggle_int(text_logger &in_log,
                             const data_item *in_item,
                             std::string_view in_value_on,
                             std::string_view in_value_off)
{
    // read current value
    int value = read_int(in_item);

    try {
        if (value == std::stoi(in_value_on.data())) {
            in_log.debug(" --> Set dataref '%s' to value '%s'", in_item->name.c_str(), in_value_off.data());
            write_int(in_item, std::stoi(in_value_off.data()));

            return in_value_off.data();
        } else {
            in_log.debug(" --> Set dataref '%s' to value '%s'", in_item->name.c_str(), in_value_on.data());
            write_int(in_item, std::stoi(in_value_on.data()));

            return in_value_on.data();
        }
    } catch (std::invalid_argument &ex) {
        in_log.error("Error reading/writing dataref '%s'", in_item->name.c_str());
        in_log.error(ex.what());
    } catch (std::out_of_range &ex) {
        in_log.error("Error reading/writing dataref '%s'", in_item->name.c_str());
        in_log.error(ex.what());
    }

    return {};
}


/**
 * Toggle an integer array dataref between on and off
 */
std::string data::toggle_int_array(text_logger &in_log,
                                   const data_item *in_item,
                                   int in_index,
                                   std::string_view in_value_on,
                                   std::string_view in_value_off)
{
    if (in_index < 0)
        return {};

    // read current value
    int value = read_int_array(in_log, in_item, in_index);

    try {
        if (value == std::stoi(in_value_on.data())) {
            in_log.debug(" --> Set dataref '%s[%i]' to value '%s'", in_item->name.c_str(), in_index, in_value_off.data());
            write_int_array(in_item, in_index, std::stoi(in_value_off.data()));

            return in_value_off.data();
        } else {
            in_log.debug(" --> Set dataref '%s[%i]' to value '%s'", in_item->name.c_str(), in_index, in_value_on.data());
            write_int_array(in_item, in_index, std::stoi(in_value_on.data()));

            return in_value_on.data();
        }
    } catch (std::invalid_argument &ex) {
        in_log.error("Error reading/writing dataref '%s'", in_item->name.c_str());
        in_log.error(ex.what());
    } catch (std::out_of_range &ex) {
        in_log.error("Error reading/writing dataref '%s'", in_item->name.c_str());
        in_log.error(ex.what());
    }

    return {};
}


/**
 * Toggle a float dataref between on and off
 */
std::string data::toggle_float(text_logger &in_log,
                               const data_item *in_item,
                               std::string_view in_value_on,
                               std::string_view in_value_off)
{
    // read current value
    float value = read_float(in_item);

    try {
        if (value == std::stof(in_value_on.data())) {
            in_log.debug(" --> Set dataref '%s' to value '%s'", in_item->name.c_str(), in_value_off.data());
            write_float(in_item, std::stof(in_value_off.data()));

            return in_value_off.data();
        } else {
            in_log.debug(" --> Set dataref '%s' to value '%s'", in_item->name.c_str(), in_value_on.data());
            write_float(in_item, std::stof(in_value_on.data()));

            return in_value_on.data();
        }
    } catch (std::invalid_argument &ex) {
        in_log.error("Error reading/writing dataref '%s'", in_item->name.c_str());
        in_log.error(ex.what());
    } catch (std::out_of_range &ex) {
        in_log.error("Error reading/writing dataref '%s'", in_item->name.c_str());
        in_log.error(ex.what());
    }

    return {};
}


/**
 * Toggle a float array dataref between on and off
 */
std::string data::toggle_float_array(text_logger &in_log,
                                     const data_item *in_item,
                                     int in_index,
                                     std::string_view in_value_on,
                                     std::string_view in_value_off)
{
    if (in_index < 0)
        return {};

    // read current value
    float value = read_float_array(in_log, in_item, in_index);

    try {
        if (value == std::stof(in_value_on.data())) {
            in_log.debug(" --> Set dataref '%s[%i]' to value '%s'", in_item->name.c_str(), in_index, in_value_off.data());
            write_float_array(in_item, in_index, std::stof(in_value_off.data()));

            return in_value_off.data();
        } else {
            in_log.debug(" --> Set dataref '%s[%i]' to value '%s'", in_item->name.c_str(), in_index, in_value_on.data());
            write_float_array(in_item, in_index, std::stof(in_value_on.data()));

            return in_value_on.data();
        }
    } catch (std::invalid_argument &ex) {
        in_log.error("Error reading/writing dataref '%s'", in_item->name.c_str());
        in_log.error(ex.what());
    } catch (std::out_of_range &ex) {
        in_log.error("Error reading/writing dataref '%s'", in_item->name.c_str());
        in_log.error(ex.what());
    }

    return {};
}


/**
 * Toggle an double dataref between on and off
 */
std::string data::toggle_double(text_logger &in_log,
                                const data_item *in_item,
                                std::string_view in_value_on,
                                std::string_view in_value_off)
{
    // read current value
    double value = read_double(in_item);

    try {
        if (value == std::stod(in_value_on.data())) {
            in_log.debug(" --> Set dataref '%s' to value '%s'", in_item->name.c_str(), in_value_off.data());
            write_double(in_item, std::stod(in_value_off.data()));

            return in_value_off.data();
        } else {
            in_log.debug(" --> Set dataref '%s' to value '%s'", in_item->name.c_str(), in_value_on.data());
            write_double(in_item, std::stod(in_value_on.data()));

            return in_value_on.data();
        }
    } catch (std::invalid_argument &ex) {
        in_log.error("Error reading/writing dataref '%s'", in_item->name.c_str());
        in_log.error(ex.what());
    } catch (std::out_of_range &ex) {
        in_log.error("Error reading/writing dataref '%s'", in_item->name.c_str());
        in_log.error(ex.what());
    }

    return {};
}


/**
 * Toggle a nyte dataref between on and off
 */
std::string data::toggle_byte(text_logger &in_log,
                              const data_item *in_item,
                              std::string_view in_value_on,
                              std::string_view in_value_off)
{
    std::string value = read_byte(in_item);

    if (value == in_value_on.data()) {
        in_log.debug(" --> Set dataref '%s' to value '%s'", in_item->name.c_str(), in_value_off.data());
        write_byte(in_item, in_value_off);

        return in_value_off.data();
    } else {
        in_log.debug(" --> Set dataref '%s' to value '%s'", in_item->name.c_str(), in_value_on.data());
        write_byte(in_item, in_value_on);

        return in_value_on.data();
    }
}

} // Namespace xmidictrl