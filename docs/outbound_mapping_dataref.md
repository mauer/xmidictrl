# Mapping Type: Dataref

Read the given dataref and sends a MIDI message of the current value. It is also possible to define two datarefs for a 
mapping. This can be useful if there are caution lights etc. for pilot and copilot.

## Required Parameters

| Parameter | Description                                                                                     |
|-----------|-------------------------------------------------------------------------------------------------|
| dataref   | Defines the dataref which should be checked (or multiple datarefs)                              |
| value_on  | As soon as the dataref will be set to this value, a MIDI message with velocity 127 will be send |
| value_off | As soon as the dataref will be set to this value, a MIDI message with velocity 0 will be send   |

**Note:**
It's not required to define *value_on* and *value_off*. However, at least one of them has to be defined.

## Optional Parameters

| Parameter    | Description                                                 |
|--------------|-------------------------------------------------------------|
| velocity_on  | Velocity which will be sent for an on message (Default 127) |
| velocity_off | Velocity which will be sent for an off message (Default 0)  |
| send_on      | Can be set to 'one' or 'all' (Default 'one')                |
| send_off     | Can be set to 'one' or 'all' (Default 'all')                |

### Parameters send_on / send_off

Both parameters define at which stage an MIDI on/off message should be sent. When set to 'one' the message will be
sent as soon as one of the defined datarefs is set to value_on/value_off. If the parameter is set to 'all', all datarefs
have to be to the value_on/value_off before the message is being sent.

## Examples

```
{ ch = 11, cc = 50, type = "drf", dataref = "sim/autopilot/heading_mode", value_on = "1", value_off = "0" }
```
*Will send a MIDI message for Channel 11 and Control Change 50 if the dataref toggles between values 1 and 0*

```
{ ch = 11, cc = 50, type = "drf", dataref = "sim/autopilot/heading_mode", value_on = "1" }
```
*Will send a MIDI message with velocity 127 for Channel 11 and Control Change 50 if the datarefs value is equal to 1,
otherwise velocity 0 will be used.*

```
{ ch = 11, cc = 50, type = "drf", dataref = "sim/autopilot/heading_mode", value_off = "0" }
```
*Will send a MIDI message with velocity 0 for Channel 11 and Control Change 50 if the datarefs value is equal to 0,
otherwise velocity 127 will be used.*

```
{ ch = 11, cc = 50, type = "drf", dataref = ["sim/pilot/caution_light", "sim/copilot/caution_light"], value_on = "1" }
```
*Will send a MIDI message with velocity 127 for Channel 11 and Control Change 50 if on of the two datarefs has a value equal to 1.
A MIDI message with velocity 0 will be sent when both datarefs are not equal 1 anymore.*
