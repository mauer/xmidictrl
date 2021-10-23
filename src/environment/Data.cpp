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
 * Toggle a dataref between on and off
 */
bool Data::toggle(std::string_view name, std::string_view valueOn, std::string_view valueOff) {
    DataItem::ptr dataItem = retrieveData(name);

    if (!dataItem->writeable) {
        LOG_ERROR << "DATA :: Dataref '" << name.data() << "' is not writeable" << LOG_END
        return false;
    }

    switch (dataItem->type) {
        case xplmType_Int:
            LOG_DEBUG << "DATA :: Change integer dataref" << LOG_END
            toggleInteger(dataItem, valueOn, valueOff);
            return true;

        case xplmType_Float:
            LOG_DEBUG << "DATA :: Change float dataref" << LOG_END
            toggleFloat(dataItem, valueOn, valueOff);
            return true;

        case xplmType_Double:
            LOG_DEBUG << "DATA :: Change double dataref" << LOG_END
            toggleDouble(dataItem, valueOn, valueOff);
            return true;

        case xplmType_Unknown:
            LOG_ERROR << "DATA :: Could not determine type of dataref '" << name.data() << "'" << LOG_END
            return false;

        default:
            LOG_ERROR << "DATA :: Unknown data type '" << dataItem->type << "'" << LOG_END
            return false;
    }
}


/**
 * Read an integer dataref
 */
bool Data::read(std::string_view name, int &value) {
    DataItem::ptr dataItem = retrieveData(name);

    if (dataItem->dataRef == nullptr)
        return false;

    value = XPLMGetDatai(dataItem->dataRef);

    return true;
}


/**
 * Write an integer dataref
 */
bool Data::write(std::string_view name, int value) {
    DataItem::ptr dataItem = retrieveData(name);

    if (dataItem->dataRef == nullptr)
        return false;

    if (dataItem->writeable) {
        XPLMSetDatai(dataItem->dataRef, value);
        return true;
    } else {
        LOG_ERROR << "DATAREFS :: DataRef '" << name.data() << "' is not writeable" << LOG_END
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
        dataItem = m_dataCache.at(name);
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

        // add new dataref to cache
        if (dataItem->dataRef)
            m_dataCache.emplace(name, dataItem);
    }

    if (!dataItem->dataRef)
        LOG_ERROR << "DATAREFS :: DataRef " << name.data() << " not found" << LOG_END

    return dataItem;
}


/**
 * Toggle an integer dataref between on and off
 */
void Data::toggleInteger(const DataItem::ptr& dataItem, std::string_view valueOn, std::string_view valueOff) {
    // read current value
    int value = XPLMGetDatai(dataItem->dataRef);

    if (value == std::stoi(valueOn.data())) {
        //LOG_DEBUG << "EVENTHANDLER :: Set dataref " << data-> << " to value " << midiEvent->mapping.valueOff << LOG_END
        XPLMSetDatai(dataItem->dataRef, std::stoi(valueOff.data()));
    } else {
        //LOG_DEBUG << "EVENTHANDLER :: Set dataref " << midiEvent->mapping.dataRef << " to value " << midiEvent->mapping.valueOn << LOG_END
        XPLMSetDatai(dataItem->dataRef, std::stoi(valueOn.data()));
    }
}


/**
 * Toggle an integer dataref between on and off
 */
void Data::toggleFloat(const DataItem::ptr& dataItem, std::string_view valueOn, std::string_view valueOff) {

}


/**
 * Toggle an integer dataref between on and off
 */
void Data::toggleDouble(const DataItem::ptr& dataItem, std::string_view valueOn, std::string_view valueOff) {

}

} // Namespace XMidiCtrl