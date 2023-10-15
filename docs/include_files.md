# Include Files
XMidiCtrl supports includes files in the aircraft profile. This is useful to include general mappings which are used
across various aircraft, such as mappings for the camera control or for avionics.

You can define several include files. In each include it is possible to define init, inbound and outbound mappings.
Includes will be defined for each MIDI device and the mappings will be added just as they were defined in the aircraft
profile itself.

## Includes Path
Include files have to be stored in the includes directory:
```
<X-Plane Path>/Output/preferences/XMidiCtrl/Includes
```

## Examples ##
In this example, we created an include file with mappings for aircraft that use both the GNS 530 and GNS 430.

```
Filename: gns530_430.toml

mapping_in = [
  #--------------------------------------------------------------------------------------------------------------------
  # Layer A
  #--------------------------------------------------------------------------------------------------------------------

  # Encoders
  { ch = 11, cc = 55, sl = "0", type = "enc", command_up = "sim/GPS/g430n1_coarse_up", command_down = "sim/GPS/g430n1_coarse_down" },
  { ch = 11, cc = 55, sl = "1", type = "enc", command_up = "sim/GPS/g430n2_coarse_up", command_down = "sim/GPS/g430n2_coarse_down" },
  { ch = 11, cc = 56, sl = "0", type = "enc", command_up = "sim/GPS/g430n1_fine_up", command_down = "sim/GPS/g430n1_fine_down" },
  { ch = 11, cc = 56, sl = "1", type = "enc", command_up = "sim/GPS/g430n2_fine_up", command_down = "sim/GPS/g430n2_fine_down" },
  { ch = 11, cc = 57, sl = "0", type = "enc", command_up = "sim/GPS/g430n1_chapter_up", command_down = "sim/GPS/g430n1_chapter_dn" },
  { ch = 11, cc = 57, sl = "1", type = "enc", command_up = "sim/GPS/g430n2_chapter_up", command_down = "sim/GPS/g430n2_chapter_dn" },
  { ch = 11, cc = 58, sl = "0", type = "enc", command_up = "sim/GPS/g430n1_page_up", command_down = "sim/GPS/g430n1_page_dn" },
  { ch = 11, cc = 58, sl = "1", type = "enc", command_up = "sim/GPS/g430n2_page_up", command_down = "sim/GPS/g430n2_page_dn" },

  # Encoder Buttons
  { ch = 11, cc = 63, sl = "0", type = "cmd", command = "sim/GPS/g430n1_nav_com_tog" },
  { ch = 11, cc = 63, sl = "1", type = "cmd", command = "sim/GPS/g430n2_nav_com_tog" },
  { ch = 11, cc = 64, sl = "0", type = "cmd", command = "sim/GPS/g430n1_com_ff" },
  { ch = 11, cc = 64, sl = "1", type = "cmd", command = "sim/GPS/g430n2_com_ff" },
  { ch = 11, cc = 65, sl = "0", type = "cmd", command = "sim/GPS/g430n1_nav_ff" },
  { ch = 11, cc = 65, sl = "1", type = "cmd", command = "sim/GPS/g430n2_nav_ff" },
  { ch = 11, cc = 66, sl = "0", type = "cmd", command = "sim/GPS/g430n1_cursor" },
  { ch = 11, cc = 66, sl = "1", type = "cmd", command = "sim/GPS/g430n2_cursor" },

  # Buttons Upper Row
  { ch = 11, cc = 67, type = "cmd", command = "xmidictrl/toggle_sublayer" },
  { ch = 11, cc = 71, sl = "0", type = "cmd", command = "sim/GPS/g430n1_zoom_in" },
  { ch = 11, cc = 71, sl = "1", type = "cmd", command = "sim/GPS/g430n2_zoom_in" },
  { ch = 11, cc = 72, sl = "0", type = "cmd", command = "sim/GPS/g430n1_zoom_out" },
  { ch = 11, cc = 72, sl = "1", type = "cmd", command = "sim/GPS/g430n2_zoom_out" },
  { ch = 11, cc = 73, sl = "0", type = "cmd", command = "sim/GPS/g430n1_direct" },
  { ch = 11, cc = 73, sl = "1", type = "cmd", command = "sim/GPS/g430n2_direct" },
  { ch = 11, cc = 74, sl = "0", type = "cmd", command = "sim/GPS/g430n1_menu" },
  { ch = 11, cc = 74, sl = "1", type = "cmd", command = "sim/GPS/g430n2_menu" },

  # Buttons Lower Row
  { ch = 11, cc = 75, sl = "0", type = "cmd", command = "sim/GPS/g430n1_cdi" },
  { ch = 11, cc = 75, sl = "1", type = "cmd", command = "sim/GPS/g430n2_cdi" },
  { ch = 11, cc = 76, sl = "0", type = "cmd", command = "sim/GPS/g430n1_obs" },
  { ch = 11, cc = 76, sl = "1", type = "cmd", command = "sim/GPS/g430n2_obs" },
  { ch = 11, cc = 77, sl = "0", type = "cmd", command = "sim/GPS/g430n1_msg" },
  { ch = 11, cc = 77, sl = "1", type = "cmd", command = "sim/GPS/g430n2_msg" },
  { ch = 11, cc = 78, sl = "0", type = "cmd", command = "sim/GPS/g430n1_fpl" },
  { ch = 11, cc = 78, sl = "1", type = "cmd", command = "sim/GPS/g430n2_fpl" },
  { ch = 11, cc = 79, sl = "0", type = "cmd", command = "sim/GPS/g430n1_vnav" },
  { ch = 11, cc = 79, sl = "1", type = "cmd", command = "sim/GPS/g430n2_vnav" },
  { ch = 11, cc = 80, sl = "0", type = "cmd", command = "sim/GPS/g430n1_proc" },
  { ch = 11, cc = 80, sl = "1", type = "cmd", command = "sim/GPS/g430n2_proc" },
  { ch = 11, cc = 81, sl = "0", type = "cmd", command = "sim/GPS/g430n1_clr" },
  { ch = 11, cc = 81, sl = "1", type = "cmd", command = "sim/GPS/g430n2_clr" },
  { ch = 11, cc = 82, sl = "0", type = "cmd", command = "sim/GPS/g430n1_ent" },
  { ch = 11, cc = 82, sl = "1", type = "cmd", command = "sim/GPS/g430n2_ent" },
]

mapping_out = [
  #--------------------------------------------------------------------------------------------------------------------
  # Layer B
  #--------------------------------------------------------------------------------------------------------------------

  # Buttons Upper Row
  { ch = 11, cc = 67, type = "drf", dataref = "xmidictrl/sublayer", value_on = "1" }
]
```

After creating the include file you have to add it to the aircraft profile:

```
# Behringer X-Touch Mini
...
[[device]]
name         = "Behringer X-Touch Mini"
include      = "gns530_430"
port_in      = 0
port_out     = 1
...
```

It is also possible to add several includes for a device:

```
[virtual_device]
name       = "Virtual device"
include    = ["camera_airliner", "general_settings"]
mapping_in = [
  { ch = 16, cc = 10, type = "cmd", command = "ixeg/733/lights/left_inboard_LL_on" },
  { ch = 16, cc = 10, type = "cmd", command = "ixeg/733/lights/right_inboard_LL_on" },
  { ch = 16, cc = 10, type = "cmd", command = "ixeg/733/lights/left_outboard_LL_on" },
  { ch = 16, cc = 10, type = "cmd", command = "ixeg/733/lights/right_outboard_LL_on" },
  { ch = 16, cc = 12, type = "cmd", command = "ixeg/733/lights/left_inboard_LL_off" },
  { ch = 16, cc = 12, type = "cmd", command = "ixeg/733/lights/right_inboard_LL_off" },
  { ch = 16, cc = 12, type = "cmd", command = "ixeg/733/lights/left_outboard_LL_off" },
  { ch = 16, cc = 12, type = "cmd", command = "ixeg/733/lights/right_outboard_LL_off" }
]
```
