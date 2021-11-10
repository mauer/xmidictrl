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

#ifndef DATA_H
#define DATA_H

// Standard
#include <map>
#include <memory>

// X-Plane SDK
#include "XPLMDataAccess.h"

// XMidiCtrl
#include "DataItem.h"

namespace XMidiCtrl {

class Data {
public:
	Data();
    ~Data();

    typedef std::shared_ptr<Data> ptr;

    std::string readByte(std::string_view name);
    bool toggle(std::string_view name, std::string_view valueOn, std::string_view valueOff);

private:
    DataItem::ptr retrieveData(std::string_view name);

    void toggleInteger(const DataItem::ptr& dataItem, std::string_view valueOn, std::string_view valueOff);
    void toggleFloat(const DataItem::ptr& dataItem, std::string_view valueOn, std::string_view valueOff);
    void toggleDouble(const DataItem::ptr& dataItem, std::string_view valueOn, std::string_view valueOff);
    void toggleByte(const DataItem::ptr& dataItem, std::string_view valueOn, std::string_view valueOff);

    std::map<std::string, DataItem::ptr> m_dataCache;
};

} // Namespace XMidiCtrl

#endif // DATA_H