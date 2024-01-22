//---------------------------------------------------------------------------------------------------------------------
//   XMidiCtrl - MIDI Controller plugin for X-Plane
//
//   Copyright (c) 2021-2024 Marco Auer
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

#ifndef XMC_TEST_HELPER_H
#define XMC_TEST_HELPER_H

#include <memory>

// XMidiCtrl
#include "env_tests.h"
#include "text_logger.h"

namespace xmidictrl {

struct test_env {
	std::unique_ptr<text_logger> log;
	std::unique_ptr<env_tests> env;
};

class test_helper {
public:
	static std::unique_ptr<test_env> create_environment();
};

} // Namespace xmidictrl

#endif // XMC_TEST_HELPER_H
