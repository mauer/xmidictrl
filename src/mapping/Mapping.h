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

#ifndef MAPPING_H
#define MAPPING_H

// Standard
#include <memory>

// XMidiCtrl
#include "Environment.h"
#include "MidiEvent.h"

namespace XMidiCtrl {

// Midi mapping types
enum class MappingType {
    None,
    Command,
    Dataref,
    Slider,
    PushAndPull,
    Encoder,
    Internal
};

class Mapping {
public:
    Mapping(Environment::ptr environment, int controlChange);
    virtual ~Mapping() = default;

    typedef std::shared_ptr<Mapping> ptr;

    virtual MappingType type();

    [[nodiscard]] int controlChange() const;

    virtual bool check();
    virtual void execute(MidiEvent::ptr midiEvent) = 0;

protected:
    Environment::ptr m_environment;

    int m_controlChange;
};

} // Namespace XMidiCtrl

#endif // MAPPING_H
