# Mapping Type: DataRef

## Description

Toggles a given DataRef between two or more values. This is very useful when no command was provided by the aircraft developer.

## Required Parameters

**Option 1 (for on/off values)**

| Parameter | Description                                                                                |
|-----------|--------------------------------------------------------------------------------------------|
| dataref   | Defines the DataRef to be changed                                                          |
| value_on  | Defines the value the DataRef should be set to, if the current value is equal to value_off |
| value_off | Defines the value the DataRef should be set to, if the current value is equal to value_on  |

**Option 2 (for multiple values)**

| Parameter | Description                                                                                |
|-----------|--------------------------------------------------------------------------------------------|
| dataref   | Defines the DataRef to be changed                                                          |
| values    | List off all values the user can toggle through                                            |

## Examples
```
{ ch = 11, cc = 68, type = "drf", dataref = "AirbusFBW/SDENG", value_on = "1", value_off = "0" }
```
*Changes DataRef 'AirbusFBW/SDENG' to value '1' if the current value is '0' and the other way round.*

```
{ ch = 11, cc = 17, type = "drf", dataref = "sim/cockpit2/radios/actuators/HSI_source_select_pilot", values = ["0", "1", "2"] }
```
*Toggles DataRef 'sim/cockpit2/radios/actuators/HSI_source_select_pilot' between the values '0', '1' and '2'.*
