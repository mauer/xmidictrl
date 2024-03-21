# Inbound Mapping Type: Command

## Description

Executes an X-Plane command when a button is pressed. The command will be executed as long as the button is being
pressed down. This behaviour is useful to map the CLR button of the G530, as pressing the button for a longer time
returns to the map screen.

## Required Parameters

| Parameter | Description                                |
|-----------|--------------------------------------------|
| command   | Defines the X-Plane command to be executed |

## Optional Parameters

| Parameter  | Description                             |
|------------|-----------------------------------------|
| data_2_on  | Data 2 for button pressed (Default 127) |
| data-2_off | Data 2 for button released (Default 0)  |

## Examples

```
{ ch = 11, cc = 67, type = "cmd", command = "sim/radios/com1_standy_flip" }
```
*Flips between active and standby frequency for COM1 when Channel is 11 and Control Change is equal to 67.*

```
{ ch = 11, cc = 67, type = "cmd", command = "sim/radios/com1_standy_flip", data_2_on = 1, data_2_off = 0  }
```
*Same as above, but the used data 2 will be 1 for button pressed and 0 for released.*
