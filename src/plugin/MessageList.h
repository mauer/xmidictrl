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

#ifndef MESSAGELIST_H
#define MESSAGELIST_H

// Standard
#include <memory>
#include <string>
#include <vector>

// XMidiCtrl
#include "Logger.h"
#include "Message.h"
#include "Settings.h"

namespace XMidiCtrl {

class MessageList {
public:
    explicit MessageList(Settings::ptr settings);
    ~MessageList();

    typedef std::shared_ptr<MessageList> ptr;

    void addMessage(MessageType type, std::string_view text);

    void clear();
    unsigned int size();

private:
    std::vector<Message::ptr> m_list;

    Settings::ptr m_settings;
};

} // Namespace XMidiCtrl

#endif // MESSAGELIST_H