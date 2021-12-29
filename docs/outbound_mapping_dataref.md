# Mapping Type: Dataref
Read the given DataRef and sends a MIDI message of the current value. It is also possible to define two DataRefs for a 
mapping. This can be useful if there are caution lights etc. for pilot and copilot.

| Variable  | Description                                                                                     |
|-----------|-------------------------------------------------------------------------------------------------|
| dataref   | Defines the DataRef which should be checked (or multiple DataRefs)                              |
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

```
{ ch = 11, cc = 50, type = "drf", dataref = ["sim/pilot/caution_light" "sim/copilot/caution_light"], value_on = "1" }
```
*Will send a MIDI message with velocity 127 for Channel 11 and Control Change 50 if on of the two DataRefs has a value equal to 1.
A MIDI message with velocity 0 will be sent when both DataRefs are not equal 1 anymore.*
