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

#ifndef MAPPEDEVENT_H
#define MAPPEDEVENT_H

// Standard
#include <memory>

// XMidiCtrl
#include "Mapping.h"
#include "MidiEvent.h"

namespace XMidiCtrl {

class MappedEvent {
public:
    MappedEvent(MidiEvent::ptr, Mapping::ptr);
    ~MappedEvent();

    typedef std::shared_ptr<MappedEvent> ptr;

    MidiEvent::ptr midiEvent() const;
    Mapping::ptr mapping() const;

private:
    MidiEvent::ptr m_midiEvent;
    Mapping::ptr   m_mapping;
};

} // Namespace XMidiCtrl

#endif // MAPPEDEVENT_H