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

// Standard
#include <stdexcept>
#include <string>

// XMidiCtrl
#include "Data.h"
#include "Logger.h"

namespace XMidiCtrl {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
Data::Data() = default;


/**
 * Destructor
 */
Data::~Data() {
    m_dataCache.clear();
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Read a dataref
 */
std::string Data::readByte(std::string_view name) {
    DataItem::ptr dataItem = retrieveData(name);

    if (dataItem->type != xplmType_Data) {
        LOG_ERROR << "Dataref '" << name << "' is not of the type 'byte'" << LOG_END
        return std::string();
    }

    std::vector<char> value(dataItem->size);
    XPLMGetDatab(dataItem->dataRef, &value[0], 0, value.size());

    return std::string(value.data(), strnlen(value.data(), value.size()));
}


/**
 * Toggle a dataref between on and off
 */
bool Data::toggle(std::string_view name, std::string_view valueOn, std::string_view valueOff) {
    DataItem::ptr dataItem = retrieveData(name);

    if (!dataItem->writeable) {
        LOG_ERROR << "Dataref '" << name.data() << "' is not writeable" << LOG_END
        return false;
    }

    switch (dataItem->type) {
        case xplmType_Int:
            toggleInteger(dataItem, valueOn, valueOff);
            return true;

        case xplmType_Float:
            toggleFloat(dataItem, valueOn, valueOff);
            return true;

        case xplmType_Double:
            toggleDouble(dataItem, valueOn, valueOff);
            return true;

        case xplmType_Data:
            toggleByte(dataItem, valueOn, valueOff);
            return false;

        case xplmType_IntArray:
            LOG_ERROR << "Unsupported type 'IntArray' of dataref '" << name.data() << "'" << LOG_END
            return false;

        case xplmType_FloatArray:
            LOG_ERROR << "Unsupported type 'FloatArray' of dataref '" << name.data() << "'" << LOG_END
            return false;

        case xplmType_Unknown:
            LOG_ERROR << "Could not determine type of dataref '" << name.data() << "'" << LOG_END
            return false;

        default:
            LOG_ERROR << "Unknown type '" << dataItem->type << "' for dataref '" << name.data() << "'" << LOG_END
            return false;
    }
}




//---------------------------------------------------------------------------------------------------------------------
//   PRIVATE
//---------------------------------------------------------------------------------------------------------------------

/**
 * Get the data ref for a dataref string
 */
DataItem::ptr Data::retrieveData(std::string_view name) {
    DataItem::ptr dataItem = std::make_shared<DataItem>();

    // check the cache first
    try {
        dataItem = m_dataCache.at(name.data());
    } catch (std::out_of_range const &) {
        dataItem->dataRef = XPLMFindDataRef(name.data());

        // get type
        if (dataItem->dataRef != nullptr)
            dataItem->type = XPLMGetDataRefTypes(dataItem->dataRef);
        else
            dataItem->type = xplmType_Unknown;

        // check if writeable
        if (dataItem->dataRef != nullptr)
            dataItem->writeable = XPLMCanWriteDataRef(dataItem->dataRef);

        // in case of a byte ref, we have to determine to size
        if (dataItem->type == xplmType_Data)
            dataItem->size = XPLMGetDatab(dataItem->dataRef, nullptr, 0, 0);

        // add new dataref to cache
        if (dataItem->dataRef)
            m_dataCache.emplace(name, dataItem);
    }

    if (!dataItem->dataRef)
        LOG_ERROR << "Dataref '" << name.data() << "' not found" << LOG_END

    return dataItem;
}


/**
 * Toggle an integer dataref between on and off
 */
void Data::toggleInteger(const DataItem::ptr& dataItem, std::string_view valueOn, std::string_view valueOff) {
    // read current value
    int value = XPLMGetDatai(dataItem->dataRef);

    if (value == std::stoi(valueOn.data())) {
        LOG_DEBUG << "Set dataref '" << dataItem->name << "' to value '" << valueOff.data() << "'" << LOG_END
        XPLMSetDatai(dataItem->dataRef, std::stoi(valueOff.data()));
    } else {
        LOG_DEBUG << "Set dataref '" << dataItem->name << "' to value '" << valueOn.data() << "'" << LOG_END
        XPLMSetDatai(dataItem->dataRef, std::stoi(valueOn.data()));
    }
}


/**
 * Toggle a float dataref between on and off
 */
void Data::toggleFloat(const DataItem::ptr& dataItem, std::string_view valueOn, std::string_view valueOff) {
    // read current value
    float value = XPLMGetDatai(dataItem->dataRef);

    if (value == std::stof(valueOn.data())) {
        LOG_DEBUG << "Set dataref '" << dataItem->name << "' to value '" << valueOff.data() << "'" << LOG_END
        XPLMSetDataf(dataItem->dataRef, std::stoi(valueOff.data()));
    } else {
        LOG_DEBUG << "Set dataref '" << dataItem->name << "' to value '" << valueOn.data() << "'" << LOG_END
        XPLMSetDataf(dataItem->dataRef, std::stoi(valueOn.data()));
    }
}


/**
 * Toggle an double dataref between on and off
 */
void Data::toggleDouble(const DataItem::ptr& dataItem, std::string_view valueOn, std::string_view valueOff) {
    // read current value
    double value = XPLMGetDatai(dataItem->dataRef);

    if (value == std::stod(valueOn.data())) {
        LOG_DEBUG << "Set dataref '" << dataItem->name << "' to value '" << valueOff.data() << "'" << LOG_END
        XPLMSetDatad(dataItem->dataRef, std::stoi(valueOff.data()));
    } else {
        LOG_DEBUG << "Set dataref '" << dataItem->name << "' to value '" << valueOn.data() << "'" << LOG_END
        XPLMSetDatad(dataItem->dataRef, std::stoi(valueOn.data()));
    }
}


/**
 * Toggle a nyte dataref between on and off
 */
void Data::toggleByte(const DataItem::ptr& dataItem, std::string_view valueOn, std::string_view valueOff) {
    std::string value = readByte(dataItem->name);

    // read current value

    if (value == valueOn.data()) {
        LOG_DEBUG << "Set dataref '" << dataItem->name << "' to value '" << valueOff.data() << "'" << LOG_END
        XPLMSetDatab(dataItem->dataRef, const_cast<char*>(valueOff.data()), 0, valueOff.size() + 1);
    } else {
        LOG_DEBUG << "Set dataref '" << dataItem->name << "' to value '" << valueOn.data() << "'" << LOG_END
        XPLMSetDatab(dataItem->dataRef, const_cast<char*>(valueOn.data()), 0, valueOn.size() + 1);
    }
}

} // Namespace XMidiCtrl