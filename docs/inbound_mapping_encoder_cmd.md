# Mapping: Encoder (Command)
## Description

Mapping for an Encoder (knob) using X-Plane Commands. You can define a command for up (right turn) and down (left turn). 
In addition, it's possible to define commands for fast up and down, if the knob was turned faster. This can be useful in 
some aircraft. Defining just up and down should be sufficient in most cases.

## Required Parameters

| Parameter    | Description                                                         |
|--------------|---------------------------------------------------------------------|
| command_up   | Command which will be executed when the knob is turned to the right |
| command_down | Command which will be executed when the knob is turned to the left  |

## Optional Parameters

| Parameter         | Description                                                                    |
|-------------------|--------------------------------------------------------------------------------|
| command_fast_up   | Command which will be executed when the knob is turned quite fast to the right |
| command_fast_down | Command which will be executed when the knob is turned quite fast to the left  |

## Examples

```
{ ch = 11, cc = 1, type = "enc", command_up = "sim/instruments/barometer_up", command_down = "sim/instruments/barometer_down" }

{ ch = 11, cc = 4, type = "enc", command_up = "sim/autopilot/airspeed_up", command_down = "sim/autopilot/airspeed_down", command_fast_up = "FlyWithLua/airspeed_sel_10_up", command_fast_up = "FlyWithLua/airspeed_sel_10_down" }
```
*The first example maps the altimeter setting to a knob. The second example maps the heading knob and uses two custom
Lua commands for a faster heading up and down.*