# Inbound Mapping Type: Encoder

## Description

The mapping for an Encoder (knob) can use X-Plane commands or a dataref. If you define commands and a dataref, the
dataref will be used.

## Encoder Mode

The plugin supports two encoder modes: *relative* and *range*. If your MIDI device sends a velocity between 0 and 127
when encoders are turned you have to select *range*. Relative mode is the preferred settings as it sends different 
values in relation to the speed the encoder was turned. However, not all MIDI devices support this mode.

## Commands Mapping

You can define a command for up (right turn) and down (left turn). In addition, it's possible to define commands for 
fast up and down, if the knob was turned faster. This can be useful in some aircraft. Defining just up and down should 
be sufficient in most cases.

### Required Parameters

| Parameter    | Description                                                         |
|--------------|---------------------------------------------------------------------|
| command_up   | Command which will be executed when the knob is turned to the right |
| command_down | Command which will be executed when the knob is turned to the left  |

### Optional Parameters

| Parameter         | Description                                                                                                            |
|-------------------|------------------------------------------------------------------------------------------------------------------------|
| delay             | Adds a delay to the execution of the commands. A delay of 3 would execute the command after three turns of the encoder |
| mode              | Range / relative, depending on your MIDI device (default is relative)                                                  |
| command_fast_up   | Command which will be executed when the knob is turned quite fast to the right (relative mode, only)                   |
| command_fast_down | Command which will be executed when the knob is turned quite fast to the left  (relative mode, only)                   |

### Examples

```
{ ch = 11, cc = 1, type = "enc", command_up = "sim/instruments/barometer_up", command_down = "sim/instruments/barometer_down" }

{ ch = 11, cc = 4, type = "enc", delay = 2, command_up = "sim/autopilot/airspeed_up", command_down = "sim/autopilot/airspeed_down", command_fast_up = "FlyWithLua/airspeed_sel_10_up", command_fast_up = "FlyWithLua/airspeed_sel_10_down" }
```
*The first example maps the altimeter setting to a knob. The second example maps the heading knob and uses two custom
Lua commands for a faster heading up and down. It also adds a delay of 2, hence the commands get executed every second turn*

## Dataref Mapping

### Description

You can define the dataref and some modifiers for up and down. In addition, it's possible to define modifiers for fast 
up and down, if the knob was turned faster. This can be useful in some aircraft. Defining just up and down should be 
sufficient in most cases.

The modifiers are expected to be numbers. They can be integers, doubles or floats (see Examples).

### Required Parameters

| Parameter     | Description                                                      |
|---------------|------------------------------------------------------------------|
| dataref       | Dataref which should be modified by the knob                     |
| modifier_up   | Modifier which will be used when the knob is turned to the right |
| modifier_down | Modifier which will be used when the knob is turned to the left  |

### Optional Parameters

| Parameter          | Description                                                                                                       |
|--------------------|-------------------------------------------------------------------------------------------------------------------|
| delay              | Adds a delay to the change of the dataref. A delay of 3 would change the dataref after three turns of the encoder |
| mode               | range / relative, depending on your MIDI device (default is relative)                                             |
| modifier_fast_up   | Modifier which will be used when the knob is turned quite fast to the right (relative mode, only)                 |
| modifier_fast_down | Modifier which will be used when the knob is turned quite fast to the left  (relative mode, only)                 |
| value_min          | Minimum value                                                                                                     |
| value_max          | Maximum value                                                                                                     |
| value_wrap         | Enables value wrapping between `value_min` (inclusive) and `value_max` (exclusive). (default is false)            |

### Examples

```
{ ch = 11, cc = 1, mode = "range", type = "enc", dataref = "ixeg/733/altimeter/altimeter_bug_pilot_ind", modifier_up = 0.02, modifier_down = -0.02 }

{ ch = 11, cc = 4, mode = "relative", type = "enc", dataref = "ixeg/733/altimeter/altimeter_bug_pilot_ind", modifier_up = 0.02, modifier_down = -0.02, modifier_fast_up = 0.20, modifier_fast_down = -0.20 }

{ ch = 11, cc = 5, type = "enc", dataref = "sim/cockpit/radios/nav1_obs_degm", modifier_up = 1, modifier_down = -1, value_min = 1, value_max = 361, value_wrap = true },
```
*The first two examples above change the altimeter bug for the pilot in the IXEG 737-300.
The third example demonstrates the use of `value_min`, `value_max` and `value_wrap` to archive a wrapping nav course.*
