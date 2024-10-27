# Inbound Mapping Type: Command by value

## Description

Executes an X-Plane command based on the current value of a specified dataref. The command will be executed as long as 
the button is being pressed down. This behaviour is useful to map the CLR button of the G530, as pressing the button 
for a longer time returns to the map screen.

## Required Parameters

| Parameter | Description                                                             |
|-----------|-------------------------------------------------------------------------|
| dataref   | Defines dataref in X-Plane which should be used for the command mapping |

## Optional Parameters

| Parameter  | Description                             |
|------------|-----------------------------------------|
| data_2_on  | Data 2 when button value equal or higher (Default 127) |
| data_2_off | Data 2 when button value equal or less (Default 0)  |

## Examples

```
{ ch = 11, cc = 67, type = "cmd", command = "sim/radios/com1_standy_flip" }
```
*Flips between active and standby frequency for COM1 when Channel is 11 and Control Change is equal to 67.*

```
{ ch = 11, cc = 67, type = "cmd", command = "sim/radios/com1_standy_flip", data_2_on = 1, data_2_off = 0  }
```
*Same as above, but the used data 2 will be 1 for button pressed and 0 for released.*
