# Outbound Mapping Type: Slider

Read the given dataref and sends a MIDI message with a data 2 based on the current value. You can specify a minimum
and maximum value of the dataref as well as of the data 2. XMidiCtrl will calculate the correct velocity based on 
those defined parameters.

This mapping type is useful to control motorized sliders on MIDI devices.

## Required Parameters

| Parameter | Description                                          |
|-----------|------------------------------------------------------|
| dataref   | Defines the dataref which should be checked          |

## Optional Parameters

| Parameter  | Description                                                  |
|------------|--------------------------------------------------------------|
| value_min  | The minimum value the dataref can have (Default 0.0)         |
| value_max  | The maximum value of the dataref (Default 1.0)               |
| data_2_min | The minimum data 2 accepted by the MIDI device (Default 0)   |
| data_2_max | The maximum data 2 accepted by the MIDI device (Default 127) |

## Example

```
{ ch = 11, cc = 50, type = "sld", dataref = "simcoders/rep/cabinventilation/heat_ratio", value_min = 0, value_max = 1, data_2_min = 0, data_2_max = 127}
```
*Will send a MIDI message for Channel 11 and Control Change 50 as soon as the dataref changes. The data 2 will be 
calculated based on the dataref value. A value of 0.5 would result in a data 2 of 64 (rounded).*
