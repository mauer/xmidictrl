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

#ifndef MAPPINGPUSHANDPULL_H
#define MAPPINGPUSHANDPULL_H

// Standard
#include <memory>
#include <string>

// XMidiCtrl
#include "Mapping.h"
#include "MidiEvent.h"

namespace XMidiCtrl {

enum class CommandType {
    Push,
    Pull
};

class MappingPushAndPull : public Mapping {
public:
    explicit MappingPushAndPull(int cc);
    ~MappingPushAndPull() override = default;

    MappingType type() override;

    void setCommandPush(std::string_view commandPush);
    [[nodiscard]] std::string_view commandPush() const;

    void setCommandPull(std::string_view commandPull);
    [[nodiscard]] std::string_view commandPull() const;

    void setCommandType(CommandType commandType);

    bool check() override;
    void execute(Environment::ptr environment, MidiEvent::ptr midiEvent) override;

private:
    CommandType m_commandType;

    std::string m_commandPush;
    std::string m_commandPull;
};

typedef std::shared_ptr<MappingPushAndPull> MappingPushAndPull_;

} // Namespace XMidiCtrl

#endif // MAPPINGCOMMAND_H
