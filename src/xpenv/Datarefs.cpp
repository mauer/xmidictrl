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

// X-Plane Environment
#include "Datarefs.h"
#include "utils/Logger.h"

namespace XPEnv {

//---------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR / DESTRUCTOR
//---------------------------------------------------------------------------------------------------------------------

/**
 * Constructor
 */
Datarefs::Datarefs() = default;


/**
 * Destructor
 */
Datarefs::~Datarefs() {
    m_dataCache.clear();
}




//---------------------------------------------------------------------------------------------------------------------
//   PUBLIC
//---------------------------------------------------------------------------------------------------------------------

/**
 * Toggle a dataref between on and off
 */
bool Datarefs::toggle(std::string_view name, std::string_view valueOn, std::string_view valueOff) {
    std::shared_ptr<Data> data = retrieveData(name);

    if (!data->writeable) {
        LOG_ERROR << "DATAREFS :: Dataref '" << name.data() << "' is not writeable" << LOG_END
        return false;
    }

    switch (data->type) {
        case xplmType_Int:
            LOG_DEBUG << "DATAREFS :: Change integer dataref" << LOG_END
            toggleInteger(data, valueOn, valueOff);
            return true;

        case xplmType_Float:
            LOG_DEBUG << "DATAREFS :: Change float dataref" << LOG_END
            toggleFloat(data, valueOn, valueOff);
            return true;

        case xplmType_Double:
            LOG_DEBUG << "DATAREFS :: Change double dataref" << LOG_END
            toggleDouble(data, valueOn, valueOff);
            return true;

        case xplmType_Unknown:
            LOG_ERROR << "DATAREFS :: Could not determine type of dataref '" << name.data() << "'" << LOG_END
            return false;

        default:
            LOG_ERROR << "DATAREFS :: Unknown data type '" << data->type << "'" << LOG_END
            return false;
    }
}


/**
 * Read an integer dataref
 */
bool Datarefs::read(std::string_view name, int &value) {
    std::shared_ptr<Data> data = retrieveData(name);

    if (data->dataRef == nullptr)
        return false;

    value = XPLMGetDatai(data->dataRef);

    return true;
}


/**
 * Read an integer dataref
 */
bool Datarefs::write(std::string_view name, int value) {
    std::shared_ptr<Data> data = retrieveData(name);

    if (data->dataRef == nullptr)
        return false;

    if (data->writeable) {
        XPLMSetDatai(data->dataRef, value);
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
std::shared_ptr<Data> Datarefs::retrieveData(std::string_view name) {
    std::shared_ptr<Data> data = std::make_shared<Data>();

    // check the cache first
    try {
        data = m_dataCache.at(name);
    } catch (std::out_of_range const &) {
        data->dataRef = XPLMFindDataRef(name.data());

        // get type
        if (data->dataRef != nullptr)
            data->type = XPLMGetDataRefTypes(data->dataRef);
        else
            data->type = xplmType_Unknown;

        // check if writeable
        if (data->dataRef != nullptr)
            data->writeable = XPLMCanWriteDataRef(data->dataRef);

        // add new dataref to cache
        if (data->dataRef)
            m_dataCache.emplace(name, data);
    }

    if (!data->dataRef)
        LOG_ERROR << "DATAREFS :: DataRef " << name.data() << " not found" << LOG_END

    return data;
}


/**
 * Toggle an integer dataref between on and off
 */
void Datarefs::toggleInteger(std::shared_ptr<Data> data, std::string_view valueOn, std::string_view valueOff) {
    // read current value
    int value = XPLMGetDatai(data->dataRef);

    if (value == std::stoi(valueOn.data())) {
        //LOG_DEBUG << "EVENTHANDLER :: Set dataref " << data-> << " to value " << midiEvent->mapping.valueOff << LOG_END
        XPLMSetDatai(data->dataRef, std::stoi(valueOff.data()));
    } else {
        //LOG_DEBUG << "EVENTHANDLER :: Set dataref " << midiEvent->mapping.dataRef << " to value " << midiEvent->mapping.valueOn << LOG_END
        XPLMSetDatai(data->dataRef, std::stoi(valueOn.data()));
    }
}


/**
 * Toggle an integer dataref between on and off
 */
void Datarefs::toggleFloat(std::shared_ptr<Data> data, std::string_view valueOn, std::string_view valueOff) {

}


/**
 * Toggle an integer dataref between on and off
 */
void Datarefs::toggleDouble(std::shared_ptr<Data> data, std::string_view valueOn, std::string_view valueOff) {

}

} // Namespace XPEnv