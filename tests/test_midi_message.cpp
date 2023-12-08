//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2023 Marco Auer
//
//   XMidiCtrl is free software: you can redistribute it and/or modify it under the terms of the
//   GNU Affero General Public License as published by the Free Software Foundation, either version 3
//   of the License, or (at your option) any later version.
//
//   XMidiCtrl is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
//   the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU Affero General Public License for more details.
//
//   You should have received a copy of the GNU Affero General Public License along with XMidiCtrl.
//   If not, see <https://www.gnu.org/licenses/>.
//---------------------------------------------------------------------------------------------------------------------

// Standard
#include <vector>

// doctest
#include "doctest/doctest.h"

// XMidiCtrl
#include "midi_message.h"

using namespace xmidictrl;

TEST_CASE("Test Inbound MIDI Message") {
    // create logger
    auto log = new text_logger();

    // create the midi message
    auto msg = new midi_message(*log, midi_direction::in);

    REQUIRE(log != nullptr);
    REQUIRE(msg != nullptr);

    SUBCASE("Create message with invalid parameter") {
        std::vector<unsigned char> midi_msg {186};

        CHECK(!msg->parse_message(&midi_msg));
    }

    SUBCASE("Create a Control Change message") {
        std::vector<unsigned char> midi_msg {186, 25, 127};

        CHECK(msg->parse_message(&midi_msg));
        CHECK(msg->check());

        CHECK(msg->channel() == 11);
        CHECK(msg->type() == midi_msg_type::control_change);
        CHECK(msg->data_2() == 127);
    }

    SUBCASE("Create a Note On message") {
        std::vector<unsigned char> midi_msg {158, 50, 80};

        CHECK(msg->parse_message(&midi_msg));
        CHECK(msg->check());

        CHECK(msg->channel() == 15);
        CHECK(msg->type() == midi_msg_type::note_on);
        CHECK(msg->data_2() == 80);
    }

    SUBCASE("Create a Note Off message") {
        std::vector<unsigned char> midi_msg {143, 50, 0};

        CHECK(msg->parse_message(&midi_msg));
        CHECK(msg->check());

        CHECK(msg->channel() == 16);
        CHECK(msg->type() == midi_msg_type::note_off);
        CHECK(msg->data_2() == 0);
    }
}


TEST_CASE("Test Outbound MIDI Message") {
    // create logger
    auto log = new text_logger();

    // create the midi message
    auto msg = new midi_message(*log, midi_direction::out);

    REQUIRE(log != nullptr);
    REQUIRE(msg != nullptr);
}