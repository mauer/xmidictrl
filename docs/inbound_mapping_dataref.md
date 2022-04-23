# Mapping Type: Dataref

## Description

Toggles a given Dataref between two or more values. This is very useful when no command was provided by the aircraft developer.

## Required Parameters

**Option 1 (for on/off values)**

| Parameter | Description                                                                                |
|-----------|--------------------------------------------------------------------------------------------|
| dataref   | Defines the dataref to be changed                                                          |
| value_on  | Defines the value the dataref should be set to, if the current value is equal to value_off |
| value_off | Defines the value the dataref should be set to, if the current value is equal to value_on  |

**Option 2 (for multiple values)**

| Parameter | Description                                     |
|-----------|-------------------------------------------------|
| dataref   | Defines the Dataref to be changed               |
| values    | List off all values the user can toggle through |

## Optional Paramaters

| Parameter | Description                                                                                       |
|-----------|---------------------------------------------------------------------------------------------------|
| mode      | Defines how the dataref should be changed. Possible values are `toggle` (default) and `momentary` |

### Dataref Mode
By default, the values for a Dataref are toggled (`mode = "toggle"`). That means, if you define two values pressing a button on your MIDI
device will toggle between those two values. This method also works with more than two possible values, such as toggling
through all autobrake settings.

Settings the mode to `momentary` requires you to define exactly two possible values. When you press the button on your
MIDI Device the defined Dataref will be set to `value_on` or the first value in `values`. As soon as you release the 
button Dataref will be set to `value_off` or the second value in `values`.

## Examples

**Example using Option 1**
```
{ ch = 11, cc = 68, type = "drf", dataref = "AirbusFBW/SDENG", value_on = "1", value_off = "0" }
```
*Changes dataref 'AirbusFBW/SDENG' to value '1' if the current value is '0' and the other way round.*

**Same example using Option 2**
```
{ ch = 11, cc = 68, type = "drf", dataref = "AirbusFBW/SDENG", values = ["1", "0"] }
```
*Changes dataref 'AirbusFBW/SDENG' to value '1' if the current value is '0' and the other way round.*

**Example using multiple values**
```
{ ch = 11, cc = 17, type = "drf", dataref = "sim/cockpit2/radios/actuators/HSI_source_select_pilot", values = ["0", "1", "2"] }
```
*Toggles dataref 'sim/cockpit2/radios/actuators/HSI_source_select_pilot' between the values '0', '1' and '2'.*
