# Create a new aircraft profile

The easiest way to create a new aircraft profile, is the use the included template for the *Behringer X-Touch Mini*. 
Simply copy the template file from the templates folder and place it into the **aircraft** or **profiles** folder.

If you decide to place the file into the **profiles** folder, please remember to rename to file according to the 
aircraft. So speaking, if you like to create a profile for the default Boeing 737-800 the file name should be 
**B738_XMidiCtrl.toml**.

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
the ports for all your MIDI devices, you can use the function **Search for MIDI Devices** of the plugin. You can find it 
in the menu **Plugins -> XMidiCtrl -> Search for MIDI Devices**. The dialog will present you with a list of all 
connected MIDI devices and their ports.

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
are MIDI messages, which are sent from the MIDI device to X-Plane. This would happen once a buttons gets pressed on your
device. Outbound mappings allow you to control the lights on your MIDI device, such as highlight button 2 if the 
autopilot is activated.

You don't have to define an outbound mapping. I would actually recommend to start with the inbound mapping first.

The mappings are defined for the devices. Each mapping has to be a separate line. Please note that line breaks are not
supported.

**Example for an inbound mapping:**
```
# Behringer X-Touch Mini
[[device]]
name       = "Behringer X-Touch Mini"
port_in    = 0
port_out   = 1
mapping_in = [
  { CC = 10, type = "cmd", command = "laminar/B738/LDU_control/push_button/MFD_ENG" },
  { CC = 11, type = "cmd", command = "laminar/B738/LDU_control/push_button/MFD_SYS" },
  { CC = 12, type = "cmd", command = "laminar/B738/LDU_control/push_button/MFD_CR" }
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
  { cc = 19, type = "cmd", command = "laminar/B738/autopilot/speed_press" },
  { cc = 20, type = "cmd", command = "laminar/B738/autopilot/vnav_press" },
  { cc = 21, type = "cmd", command = "laminar/B738/autopilot/lnav_press" }
]
mapping_out = [
  # Buttons Upper Row
  { cc = 19, type = "drf", dataref = "laminar/B738/autopilot/speed_pos", value_on="1" },
  { cc = 20, type = "drf", dataref = "laminar/B738/autopilot/vnav_pos",  value_on="1" },
  { cc = 21, type = "drf", dataref = "laminar/B738/autopilot/lnav_pos",  value_on="1" }
]
```