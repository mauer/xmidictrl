# Inbound Mapping Type: Short & long

## Description

This mapping simulates a push and pull button, as found in Airbus aircraft. A short button press will execute the push
command, while a longer button press (~1 sec) will execute the pull command.

## Required Parameters

| Parameter     | Description                                          |
|---------------|------------------------------------------------------|
| command_short | Defines the command to be executed for a push action |
| command_long  | Defines the command to be executed for a pull action |

## Examples

```
{ ch = 11, cc = 14, type = "snl", command_push = "AirbusFBW/PushAltitude", command_pull = "AirbusFBW/PullAltitude" }
```
*If the button for Channel 11 and Control Change 14 is pressed and immediately released the command
'AirbusFBW/PushAltitude' will be executed. If the button is pressed for a longer time (~one second) then command
'AirbusFBW/PullAltitude' will be executed.*