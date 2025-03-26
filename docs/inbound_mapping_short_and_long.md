# Inbound Mapping Type: Short & long

## Description

This mapping simulates a push and pull button, as found in Airbus aircraft. A short button press will execute the push
command, while a longer button press (~1 sec) will execute the pull command.

## Required Parameters

It is required to define an action for the short as well as the long action. Those can be either command or dataref
actions.

| Parameter     | Description                                                                              |
|---------------|------------------------------------------------------------------------------------------|
| command_short | Defines the command to be executed for a short action                                    |
| command_long  | Defines the command to be executed for a long action                                     |
| dataref_short | Defines the dataref to be used for a short action                                        |
| dataref_long  | Defines the dataref to be used for a long action                                         |
| values_short  | Required if *dataref_short* is being used, lists the values the user can toggle throught |
| values_long   | Required if *dataref_long* is being used, lists the values the user can toggle throught  |

## Optional Parameters

| Parameter     | Description                                     |
|---------------|-------------------------------------------------|
| label_short   | Defines the label to be used for a short action |
| label_long    | Defines the label to be used for a long action  |

## Examples

```
{ ch = 11, cc = 14, type = "snl", command_short = "AirbusFBW/PushAltitude", command_long = "AirbusFBW/PullAltitude" }
```
*If the button for Channel 11 and Control Change 14 is pressed and immediately released the command
'AirbusFBW/PushAltitude' will be executed. If the button is pressed for a longer time (~one second) then command
'AirbusFBW/PullAltitude' will be executed.*