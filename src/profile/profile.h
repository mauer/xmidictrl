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

#ifndef _PROFILE_H_
#define _PROFILE_H_

// Standard
#include <memory>
#include <string>
#include <vector>

// toml11
#include <toml.hpp>

// XMidiCtrl
#include "config.h"
#include "device_list.h"
#include "map.h"
#include "types.h"
#include "xplane.h"

namespace xmidictrl {

class profile : public config {
public:
    explicit profile(xplane::ptr xplane);
    ~profile() = default;

    bool load();
    void close() override;

    [[nodiscard]] bool has_errors() const;

    void process();

private:
    std::string get_profile_filename();

    void create_device_list();

    void create_inbound_mapping(int dev_no, toml::array settings, std::shared_ptr<device> device);
    void create_outbound_mapping(int dev_no, toml::array settings, std::shared_ptr<device> device);

    static mapping_type translateMapTypeStr(std::string_view typeStr);
    static mapping_type read_mapping_type(toml::value &settings);

    std::shared_ptr<device_list> m_device_list;

    bool m_errors_found {false};
};

} // Namespace XMidiCtrl

#endif // _PROFILE_H_