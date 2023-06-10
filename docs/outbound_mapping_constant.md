# Mapping Type: Constant

This mapping will always send the same message to the MIDI device. This can be useful to disable or enable certain
functions, such as LEDs.

## Required Parameters

| Parameter | Description                                                                  |
|-----------|------------------------------------------------------------------------------|
| velocity  | Defines the velocity to be sent to the MIDI device (Value between 0 and 127) |

## Examples

```
{ ch = 11, cc = 1, type = "con", velocity = 0 }
```
*Will send a MIDI message for Channel 11 and Control Change 1 with a Velocity of 0*

```
{ ch = 11, cc = 1, type = "con", velocity = 127 }
```
*Will send a MIDI message for Channel 11 and Control Change 1 with a Velocity of 127*
