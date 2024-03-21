# Outbound Mapping Type: Constant

This mapping will always send the same message to the MIDI device. This can be useful to disable or enable certain
functions, such as LEDs.

## Required Parameters

| Parameter | Description                                                                |
|-----------|----------------------------------------------------------------------------|
| data_2    | Defines the data_2 to be sent to the MIDI device (Value between 0 and 127) |

## Examples

```
{ ch = 11, cc = 1, type = "con", data_2 = 0 }
```
*Will send a MIDI message for Channel 11 and Control Change 1 with a data 2 of 0*

```
{ ch = 11, cc = 1, type = "con", data_2 = 127 }
```
*Will send a MIDI message for Channel 11 and Control Change 1 with a data 2 of 127*
