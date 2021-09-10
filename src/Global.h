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

#ifndef GLOBAL_H
#define GLOBAL_H

// Standard
#include <map>
#include <string>

// X-Plane SDK
#include "XPLMDataAccess.h"

// Name of the config file
#define XMIDICTRL_CONFIG_FILENAME "xmidictrl.cfg"

// Mapping Types as strings
#define XMIDICTRL_MAPTYPE_COMMAND     "cmd"
#define XMIDICTRL_MAPTYPE_SLIDER      "sld"
#define XMIDICTRL_MAPTYPE_PUSHANDPULL "pnp"
#define XMIDICTRL_MAPTYPE_ENCODER     "enc"
#define XMIDICTRL_MAPTYPE_DATAREF     "drf"
#define XMIDICTRL_MAPTYPE_INTERNAL    "int"

// Midi Stats
#define XMIDICTRL_STATUS_NOTE_BEGIN 154
#define XMIDICTRL_STATUS_NOTE_END   138
#define XMIDICTRL_STATUS_CC         186


namespace XMidiCtrl {

//---------------------------------------------------------------------------------------------------------------------
//   STRUCTS
//---------------------------------------------------------------------------------------------------------------------

// X-Plane Data Details
struct DataDetails {
    XPLMDataRef dataRef;
    XPLMDataTypeID type;
};

} // Namespace XMidiCtrl

#endif // GLOBAL_H