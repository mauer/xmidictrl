# Mapping Type: Command

## Description
Executes an X-Plane command when a button is pressed. The command will be executed as long as the button is being
pressed down. This behaviour is useful to map the CLR button of the G530, as pressing the button for a longer time
returns to the map screen.

## Parameters
| Parameter | Description                                          |
|-----------|------------------------------------------------------|
| command   | Defines the X-Plane command to be executed |

## Examples
```
{ ch = 11, cc = 67, type = "cmd", command = "sim/radios/com1_standy_flip" }
```
*Flips between active and standby frequency for COM1 when Channel is 11 and Control Change is equal to 67.*
