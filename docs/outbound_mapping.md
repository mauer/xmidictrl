# Outbound Mappings
Outbound mappings are used to set a status on your MIDI controller, such as highlight a button if a certain function
got activated in X-Plane.

## Common variables
The following variables are required for each mapping.

| Variable | Description                       |
|----------|-----------------------------------|
| ch       | MIDI Channel (Default Value = 11) |
| cc       | Control Change No Number          |

## Mapping Types
The following mapping types are supported:

| Mapping Type |  Name   | Description and usage                                         |
|:------------:|:-------:|---------------------------------------------------------------|
| drf          | DataRef | Reads the DataRef value and sends a MIDI message if necessary |

###
### Mapping Type: Dataref
Read the given DataRef and sends a MIDI message if the value has been changed. There a different methods to trigger
the creation of the MIDI Message, which will be explained below.

| Variable  | Description                                                                                     |
|-----------|-------------------------------------------------------------------------------------------------|
| dataref   | Defines the DataRef which should be checked                                                     |
| value_on  | As soon as the DataRef will be set to this value, a MIDI message with velocity 127 will be send |
| value_off | As soon as the DataRef will be set to this value, a MIDI message with velocity 0 will be send   |

**Note:**
It's not required to define *value_on* and *value_off*. However, at least one of them has to be defined. 

**Examples:**
```
{ ch = 11, cc = 50, type = "drf", dataref = "sim/autopilot/heading_mode", value_on = "1", value_off = "0" }
```
*Will send a MIDI message for Channel 11 and Control Change 50 if the DataRef toggles between values 1 and 0*

```
{ ch = 11, cc = 50, type = "drf", dataref = "sim/autopilot/heading_mode", value_on = "1" }
```
*Will send a MIDI message with velocity 127 for Channel 11 and Control Change 50 if the DataRef's value is equal to 1,
otherwise velocity 0 will be used.*

```
{ ch = 11, cc = 50, type = "drf", dataref = "sim/autopilot/heading_mode", value_off = "0" }
```
*Will send a MIDI message with velocity 0 for Channel 11 and Control Change 50 if the DataRef's value is equal to 0,
otherwise velocity 127 will be used.*
