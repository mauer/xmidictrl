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

// Standard
#include <string>

// XMidiCtrl
#include "logger.h"

namespace xmidictrl {

//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Check if a dataref is valid
 */
bool data::check(std::string_view name)
{
    data_item *item = retrieve_data(name);

    if (item == nullptr)
        return false;
    else
        return true;
}


/**
 * Read a dataref
 */
bool data::read(std::string_view name, std::string &value)
{
    value.assign(std::string());

    data_item *item = retrieve_data(name);

    if (item == nullptr)
        return false;

    // check the dataref type
    if (item->type & xplmType_Int) {
        int i = read_int(item);
        value.assign(std::to_string(i));

        return true;

    } else if (item->type & xplmType_Float) {
        std::stringstream ss;

        ss << read_float(item);
        value.assign(ss.str());

        return true;

    } else if (item->type & xplmType_Double) {
        std::stringstream ss;

        ss << read_double(item);
        value.assign(ss.str());

        value.assign(ss.str());

        return true;

    } else if (item->type & xplmType_Data) {
        value.assign(read_byte(item));

        return true;

    } else if (item->type & xplmType_IntArray) {
        int i = read_int_array(item, get_index(name));
        value.assign(std::to_string(i));

        return true;

    } else if (item->type & xplmType_FloatArray) {
        std::stringstream ss;

        ss << read_float_array(item, get_index(name));
        value.assign(ss.str());

        return true;

    } else if (item->type & xplmType_Unknown) {
        LOG_ERROR << "Could not determine type for dataref '" << name.data() << "'" << LOG_END

        return false;

    } else {
        LOG_ERROR << "Unknown type '" << item->type << "' for dataref '" << name.data() << "'" << LOG_END

        return false;
    }
}


/**
 * Read a numeric dataref
 */
bool data::read(std::string_view name, float &value)
{
    value = 0.0f;

    data_item *item = retrieve_data(name);

    if (item == nullptr)
        return false;

    // check the dataref type
    if (item->type & xplmType_Int) {
        int i = read_int(item);
        value = static_cast<float>(i);

        return true;

    } else if (item->type & xplmType_Float) {
        value = read_float(item);

        return true;

    } else if (item->type & xplmType_Double) {
        double d = read_double(item);
        value = static_cast<float>(d);

        return true;

    } else if (item->type & xplmType_Data) {
        LOG_ERROR << "Dataref '" << name.data() << "' is not numeric" << LOG_END
        return false;

    } else if (item->type & xplmType_IntArray) {
        int i = read_int_array(item, get_index(name));
        value = static_cast<float>(i);

        return true;

    } else if (item->type & xplmType_FloatArray) {
        value = read_float_array(item, get_index(name));

        return true;

    } else if (item->type & xplmType_Unknown) {
        LOG_ERROR << "Could not determine type for dataref '" << name.data() << "'" << LOG_END
        return false;

    } else {
        LOG_ERROR << "Unknown type '" << item->type << "' for dataref '" << name.data() << "'" << LOG_END
        return false;
    }
}


/**
 * Read a dataref as float array
 */
bool data::read(std::string_view name, std::vector<float> &values)
{
    data_item *item = retrieve_data(name);
    values = {};

    if (item == nullptr)
        return false;

    if (item->type & xplmType_FloatArray) {
        values = read_float_array(item);
        return true;

    } else {
        LOG_ERROR << "Could not read dataref '" << name.data() << "' as 'FloatArray'" << LOG_END
        return false;
    }
}


/**
 * Read a dataref as integer array
 */
bool data::read(std::string_view name, std::vector<int> &values)
{
    data_item *item = retrieve_data(name);
    values = {};

    if (item == nullptr)
        return false;

    if (item->type & xplmType_IntArray) {
        values = read_int_array(item);
        return true;

    } else {
        LOG_ERROR << "Could not read dataref '" << name.data() << "' as 'IntArray'" << LOG_END
        return false;
    }
}


/**
 * Write an value to a dataref
 */
bool data::write(std::string_view name, std::string_view value)
{
    data_item *item = retrieve_data(name);

    if (item == nullptr)
        return false;

    if (!item->writeable) {
        LOG_ERROR << "Dataref '" << name.data() << "' is not writeable" << LOG_END
        return false;
    }

    try {
        if (item->type & xplmType_Int) {
            write_int(item, std::stoi(value.data()));
            return true;
        } else if (item->type & xplmType_Float) {
            write_float(item, std::stof(value.data()));
            return true;
        } else if (item->type & xplmType_Double) {
            write_double(item, std::stod(value.data()));
            return true;
        } else if (item->type & xplmType_Data) {
            write_byte(item, value);
            return true;
        } else if (item->type & xplmType_IntArray) {
            write_int_array(item, get_index(name), std::stoi(value.data()));
            return true;
        } else if (item->type & xplmType_FloatArray) {
            write_float_array(item, get_index(name), std::stof(value.data()));
            return true;
        } else if (item->type & xplmType_Unknown) {
            LOG_ERROR << "Could not determine type for dataref '" << name.data() << "'" << LOG_END
            return false;
        } else {
            LOG_ERROR << "Unknown type '" << item->type << "' for dataref '" << name.data() << "'" << LOG_END
            return false;
        }
    } catch (std::invalid_argument &ex) {
        LOG_DEBUG << ex.what() << LOG_END
    } catch (std::out_of_range &ex) {
        LOG_DEBUG << ex.what() << LOG_END
    }

    return false;
}


/**
 * Write an value to a numeric dataref
 */
bool data::write(std::string_view name, float value)
{
    data_item *item = retrieve_data(name);

    if (item == nullptr)
        return false;

    if (!item->writeable) {
        LOG_ERROR << "Dataref '" << name.data() << "' is not writeable" << LOG_END
        return false;
    }

    if (item->type & xplmType_Int) {
        write_int(item, static_cast<int>(value));
        return true;
    } else if (item->type & xplmType_Float) {
        write_float(item, value);
        return true;
    } else if (item->type & xplmType_Double) {
        write_double(item, static_cast<double>(value));
        return true;
    } else if (item->type & xplmType_Data) {
        LOG_ERROR << "Dataref '" << name.data() << "' is not numeric" << LOG_END
        return false;
    } else if (item->type & xplmType_IntArray) {
        write_int_array(item, get_index(name), static_cast<int>(value));
        return true;
    } else if (item->type & xplmType_FloatArray) {
        write_float_array(item, get_index(name), value);
        return true;
    } else if (item->type & xplmType_Unknown) {
        LOG_ERROR << "Could not determine type for dataref '" << name.data() << "'" << LOG_END
        return false;
    } else {
        LOG_ERROR << "Unknown type '" << item->type << "' for dataref '" << name.data() << "'" << LOG_END
        return false;
    }
}


/**
 * Toggle a dataref between on and off
 */
bool data::toggle(std::string_view name, std::string_view value_on, std::string_view value_off)
{
    data_item *item = retrieve_data(name);

    if (item == nullptr)
        return false;

    if (!item->writeable) {
        LOG_ERROR << "Dataref '" << name.data() << "' is not writeable" << LOG_END
        return false;
    }

    if (item->type & xplmType_Int) {
        toggle_int(item, value_on, value_off);
        return true;
    } else if (item->type & xplmType_Float) {
        toggle_float(item, value_on, value_off);
        return true;
    } else if (item->type & xplmType_Double) {
        toggle_double(item, value_on, value_off);
        return true;
    } else if (item->type & xplmType_Data) {
        toggle_byte(item, value_on, value_off);
        return true;
    } else if (item->type & xplmType_IntArray) {
        toggle_int_array(item, get_index(name), value_on, value_off);
        return true;
    } else if (item->type & xplmType_FloatArray) {
        toggle_float_array(item, get_index(name), value_on, value_off);
        return true;
    } else if (item->type & xplmType_Unknown) {
        LOG_ERROR << "Could not determine type for dataref '" << name.data() << "'" << LOG_END
        return false;
    } else {
        LOG_ERROR << "Unknown type '" << item->type << "' for dataref '" << name.data() << "'" << LOG_END
        return false;
    }
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Get the data ref for a dataref string
 */
data_item *data::retrieve_data(std::string_view name)
{
    std::string dataref_name = name.data();

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

        if (item->dataref == nullptr) {
            LOG_ERROR << "Dataref '" << dataref_name.c_str() << "' not found" << LOG_END
            return {};
        }

        item->type = XPLMGetDataRefTypes(item->dataref);
        item->writeable = XPLMCanWriteDataRef(item->dataref);

        // in case of a byte ref, we have to determine to size
        if (item->type & xplmType_Data)
            item->size = XPLMGetDatab(item->dataref, nullptr, 0, 0);

        // add new dataref to cache
        LOG_DEBUG << "Dataref '" << dataref_name.c_str() << "' added to internal cache" << LOG_END
        m_data_cache.emplace(dataref_name, std::move(item));
    }

    return m_data_cache.at(dataref_name).get();
}


/**
 * Extract the index from the name
 */
int data::get_index(std::string_view name)
{
    int index = -1;

    // is the dataref an array?
    if (name.back() == ']') {
        int pos = static_cast<int>(name.find('['));

        // read index
        try {
            index = std::stoi(name.substr(pos + 1, name.find(']') - pos - 1).data());
        } catch (std::invalid_argument &ex) {
            LOG_ERROR << "Error reading index from dataref name '" << name << "'" << LOG_END
            LOG_DEBUG << ex.what() << LOG_END
        } catch (std::out_of_range const &ex) {
            LOG_ERROR << "Error reading index from dataref name '" << name << "'" << LOG_END
            LOG_ERROR << ex.what() << LOG_END
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
float data::read_float_array(const data_item *item, int index)
{
    float value = 0.0f;

    if (index > -1)
        XPLMGetDatavf(item->dataref, &value, index, 1);
    else
        LOG_ERROR << "Invalid index '" << index << "' for dataref '" << item->name << "'" << LOG_END

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
int data::read_int_array(const data_item *item, int index)
{
    int value = 0;

    if (index > -1)
        XPLMGetDatavi(item->dataref, &value, index, 1);
    else
        LOG_ERROR << "Invalid index '" << index << "' for dataref '" << item->name << "'" << LOG_END

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
void data::toggle_int(const data_item *item, std::string_view value_on, std::string_view value_off)
{
    // read current value
    int value = read_int(item);

    try {
        if (value == std::stoi(value_on.data())) {
            LOG_DEBUG << "Set dataref '" << item->name << "' to value '" << value_off.data() << "'" << LOG_END
            write_int(item, std::stoi(value_off.data()));
        } else {
            LOG_DEBUG << "Set dataref '" << item->name << "' to value '" << value_on.data() << "'" << LOG_END
            write_int(item, std::stoi(value_on.data()));
        }
    } catch (std::invalid_argument &ex) {
        LOG_DEBUG << ex.what() << LOG_END
    } catch (std::out_of_range &ex) {
        LOG_DEBUG << ex.what() << LOG_END
    }
}


/**
 * Toggle an integer array dataref between on and off
 */
void data::toggle_int_array(const data_item *item, int index, std::string_view value_on, std::string_view value_off)
{
    if (index < 0)
        return;

    // read current value
    int value = read_int_array(item, index);

    try {
        if (value == std::stoi(value_on.data())) {
            LOG_DEBUG << "Set dataref '" << item->name << "[" << index << "]' to value '" << value_off.data() << "'"
                      << LOG_END
            write_int_array(item, index, std::stoi(value_off.data()));
        } else {
            LOG_DEBUG << "Set dataref '" << item->name << "[" << index << "]' to value '" << value_on.data() << "'"
                      << LOG_END
            write_int_array(item, index, std::stoi(value_on.data()));
        }
    } catch (std::invalid_argument &ex) {
        LOG_DEBUG << ex.what() << LOG_END
    } catch (std::out_of_range &ex) {
        LOG_DEBUG << ex.what() << LOG_END
    }
}


/**
 * Toggle a float dataref between on and off
 */
void data::toggle_float(const data_item *item, std::string_view value_on, std::string_view value_off)
{
    // read current value
    float value = read_float(item);

    try {
        if (value == std::stof(value_on.data())) {
            LOG_DEBUG << "Set dataref '" << item->name << "' to value '" << value_off.data() << "'" << LOG_END
            write_float(item, std::stof(value_off.data()));
        } else {
            LOG_DEBUG << "Set dataref '" << item->name << "' to value '" << value_on.data() << "'" << LOG_END
            write_float(item, std::stof(value_on.data()));
        }
    } catch (std::invalid_argument &ex) {
        LOG_DEBUG << ex.what() << LOG_END
    } catch (std::out_of_range &ex) {
        LOG_DEBUG << ex.what() << LOG_END
    }
}


/**
 * Toggle a float array dataref between on and off
 */
void data::toggle_float_array(const data_item *item, int index, std::string_view value_on, std::string_view value_off)
{
    if (index < 0)
        return;

    // read current value
    float value = read_float_array(item, index);

    try {
        if (value == std::stof(value_on.data())) {
            LOG_DEBUG << "Set dataref '" << item->name << "[" << index << "]' to value '" << value_off.data() << "'"
                      << LOG_END
            write_float_array(item, index, std::stof(value_off.data()));
        } else {
            LOG_DEBUG << "Set dataref '" << item->name << "[" << index << "]' to value '" << value_on.data() << "'"
                      << LOG_END
            write_float_array(item, index, std::stof(value_on.data()));
        }
    } catch (std::invalid_argument &ex) {
        LOG_DEBUG << ex.what() << LOG_END
    } catch (std::out_of_range &ex) {
        LOG_DEBUG << ex.what() << LOG_END
    }
}


/**
 * Toggle an double dataref between on and off
 */
void data::toggle_double(const data_item *item, std::string_view value_on, std::string_view value_off)
{
    // read current value
    double value = read_double(item);

    try {
        if (value == std::stod(value_on.data())) {
            LOG_DEBUG << "Set dataref '" << item->name << "' to value '" << value_off.data() << "'" << LOG_END
            write_double(item, std::stod(value_off.data()));
        } else {
            LOG_DEBUG << "Set dataref '" << item->name << "' to value '" << value_on.data() << "'" << LOG_END
            write_double(item, std::stod(value_on.data()));
        }
    } catch (std::invalid_argument &ex) {
        LOG_DEBUG << ex.what() << LOG_END
    } catch (std::out_of_range &ex) {
        LOG_DEBUG << ex.what() << LOG_END
    }
}


/**
 * Toggle a nyte dataref between on and off
 */
void data::toggle_byte(const data_item *item, std::string_view value_on, std::string_view value_off)
{
    std::string value = read_byte(item);

    if (value == value_on.data()) {
        LOG_DEBUG << "Set dataref '" << item->name << "' to value '" << value_off.data() << "'" << LOG_END
        write_byte(item, value_off);
    } else {
        LOG_DEBUG << "Set dataref '" << item->name << "' to value '" << value_on.data() << "'" << LOG_END
        write_byte(item, value_on);
    }
}

} // Namespace xmidictrl