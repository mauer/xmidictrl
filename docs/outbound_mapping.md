# Outbound Mappings

## Mapping Types
Each outgoing MIDI Message can be mapped to one of the following functions:

| Mapping Type | Name        | Description and usage                                                         |
|:------------:|:-----------:|-------------------------------------------------------------------------------|
| drf          | Dataref     | Toggles a given Dataref between two values                                    |
| int          | Internal    | Performs an internal command (not implemented yet)                            |

###
### Mapping Type: Dataref
Toggles a given dataref between two values. This is very useful when no command was provided by the aircraft developer.

| Variable  | Description                                                                                |
|-----------|--------------------------------------------------------------------------------------------|
| dataref   | Defines the DataRef which should be changed                                                |
| value_on  | Defines the value the DataRef should be set to, if the current value is equal to value_off |
| value_off | Defines the value the DataRef should be set to, if the current value is equal to value_on  |

**Example:**
```
{ cc = 68, type = "drf", dataref = "AirbusFBW/SDENG", value_on = "1", value_off = "0" }
```
*Changes dataref 'AirbusFBW/SDENG' to value '1' if the current value is '0' and the cc is equal to '68'*

###
### Internal
*Not implemented in the current version*