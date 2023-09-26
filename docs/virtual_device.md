# Virtual Device
XMidiCtrl allows the mapping of a virtual MIDI device. This feature is useful for people who want to map joystick
buttons via the aircraft profile of XMidiCtrl.

## How to use
The plugin creates new commands which can be assigned to joystick buttons using the normal X-Plane joystick mapping.
Each command sends a MIDI message which can be handled by XMidiCtrl. In order to map those messages you simply add 
another device to your aircraft profile.

The channel to be used can be setup in the general settings of the plugin.

## Example
```
title   = "MY profile with a virtual device"
version = "1.00"

# Virtual device
[virtual_device]
name       = "Virtual device"
mapping_in = [
  { ch = 16, cc = 1, type = "cmd", command = "sim/autopilot/fdir_toggle" },
  { ch = 16, cc = 2, type = "cmd", command = "sim/autopilot/heading_sync" },
  ...
]

# Behringer X-Touch Mini
[[device]]
name       = "Behringer X-Touch Mini"
port_in    = 0
port_out   = 1
mapping_in = [
  ...
]  
```