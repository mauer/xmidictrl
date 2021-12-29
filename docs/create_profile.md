# Create a new aircraft profile

The easiest way to create a new aircraft profile, would be to create a copy of the included template for the 
*Behringer X-Touch Mini*. Simply copy the **XMidiCtrl.toml** file from the templates folder and place it into the 
**aircraft** folder.

You can also store your profiles outside the aircraft folders, if you prefer so. Please have a look at 
[Aircraft Profiles](profile.md) for more detailed information about that.

## Name and Version
The first two lines of a profile are title and version. They don't really have any function and are purely for your
information.

**Example:***
```
title   = "Configuration for the Cessna 172"
version = "1.00"
```

## Inbound and Outbound Ports
After copying the file, it is required to set the inbound and outbound ports for the MIDI device. In order to determine 
the ports for all your MIDI devices, you can use the function **Show MIDI Devices** of the plugin. You can find it 
in the menu **Plugins -> XMidiCtrl -> Show MIDI Devices**. The dialog will show you with a list of all connected MIDI 
devices and their ports.

The ports have to be defined for each device:

**Example:**
```
# Behringer X-Touch Mini
[[device]]
name     = "Behringer X-Touch Mini"
port_in  = 0
port_out = 1
```

## Mappings
Next would be the definition of some mappings. You can create inbound mappings and outbound mappings. Inbound mappings
are MIDI messages, which are sent from the MIDI device to X-Plane. This would happen once a button gets pressed on your
device. Outbound mappings allow you to control the lights on your MIDI device, such as highlight a button if the 
autopilot is activated.

You don't have to define an outbound mapping. I would actually recommend to start with the inbound mapping first. Once
you are happy with it, you can add the outbound mapping.

Each mapping has to be a separate line. Please note that line breaks are not supported by TOML.

**Example for an inbound mapping:**
```
# Behringer X-Touch Mini
[[device]]
name       = "Behringer X-Touch Mini"
port_in    = 0
port_out   = 1
mapping_in = [
  { ch = 11, cc = 10, type = "cmd", command = "laminar/B738/LDU_control/push_button/MFD_ENG" },
  { ch = 11, cc = 11, type = "cmd", command = "laminar/B738/LDU_control/push_button/MFD_SYS" },
  { ch = 11, cc = 12, type = "cmd", command = "laminar/B738/LDU_control/push_button/MFD_CR" }
]
```

**Example including an outbound mapping:**
```
# Behringer X-Touch Mini
[[device]]
name       = "Behringer X-Touch Mini"
port_in    = 0
port_out   = 1
mapping_in = [
  { ch = 11, cc = 19, type = "cmd", command = "laminar/B738/autopilot/speed_press" },
  { ch = 11, cc = 20, type = "cmd", command = "laminar/B738/autopilot/vnav_press" },
  { ch = 11, cc = 21, type = "cmd", command = "laminar/B738/autopilot/lnav_press" }
]
mapping_out = [
  { ch = 11, cc = 19, type = "drf", dataref = "laminar/B738/autopilot/speed_pos", value_on="1" },
  { ch = 11, cc = 20, type = "drf", dataref = "laminar/B738/autopilot/vnav_pos",  value_on="1" },
  { ch = 11, cc = 21, type = "drf", dataref = "laminar/B738/autopilot/lnav_pos",  value_on="1" }
]
```
