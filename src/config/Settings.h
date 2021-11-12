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

#ifndef SETTINGS_H
#define SETTINGS_H

// Standard
#include <memory>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "Config.h"
#include "Types.h"
#include "XPlane.h"

namespace XMidiCtrl {

class Settings : public Config {
public:
    Settings(XPlane::ptr xplane);
    ~Settings();

    typedef std::shared_ptr<Settings> ptr;

    void setLogLevel(const LogLevel logLevel);
    const LogLevel logLevel();

    void setShowMessagesDialog(const bool showDialog);
    const bool showMessagesDialog();

private:
    void saveSettings();

    std::string m_fileName;
};

} // Namespace XMidiCtrl

#endif // SETTINGS_H