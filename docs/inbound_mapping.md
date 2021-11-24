# Inbound Mappings

## Mapping Types
Each incoming MIDI Message can be mapped to one of the following functions:

| Mapping Type | Name        | Description and usage                                                            |
|:------------:|:-----------:|----------------------------------------------------------------------------------|
| cmd          | Command     | Executes a X-Plane command                                                       |
| pnp          | Push & Pull | Simulates a Push & Pull button, which can execute two different commands         |   
| drf          | Dataref     | Toggles a given Dataref between two values                                       |
| enc          | Encoder     | Executes different commands for up/down and fast up/fast down                    |
| sld          | Slider      | Executes up to three different commands, depending on the location of the slider |
| int          | Internal    | Performs an internal command (not implemented yet)                               |

###
### Mapping Type: Command
Executes an X-Plane command when a button is pressed. The command will be executed as long as the button is being
pressed down. This behaviour is useful to map the CLR button of the G530 for example, as pressing the button for a
longer time returns to the map screen.

| Variable | Description                                          |
|----------|------------------------------------------------------|
| command  | Defines the X-Plane command to be executed |

**Example:**
```
{ cc = 67, type = "cmd", command = "sim/radios/com1_standy_flip" }
```
*Flips between active and standby frequency for COM1 when Control Change message is equal to 67*

###
### Mapping Type: Push & Pull
This mapping simulates a push and pull button, as found in Airbus aircrafts. A short button press will execute the push
command, while a longer button press will execute the pull command.

| Variable     | Description                                          |
|--------------|------------------------------------------------------|
| command_push | Defines the command to be executed for a push action |
| command_pull | Defines the command to be executed for a pull action |

**Example:**
```
{ cc = 14, type = "pnp", command_push = "AirbusFBW/PushAltitude", command_pull = "AirbusFBW/PullAltitude" }
```
*If the button for Control Change message 14 is pressed and immediately released the command AirbusFBW/PushAltitude will
be executed. If the button is pressed longer then command AirbusFBW/PullAltitude will be executed*

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
### Mapping: Encoder
Mapping for an encoder (knob). You can define a command for up (right turn) and down (left turn). In addition, it's
possible to define command for fast up and down, if the knob was turned rather fast. This can be useful in some
aircraft. Normally, defining up and down should be sufficient

| Variable          | Description                                                                                |
|-------------------|--------------------------------------------------------------------------------------------|
| command_up        | Command which will be executed when the knob is turn to the right                          |
| command_down      | Command which will be executed when the knob is turn to the left                           |
| command_fast_up   | Command which will be executed when the knob is turn quite fast to the right               |
| command_fast_down | Command which will be executed when the knob is turn quite fast to the left                |

**Examples:**
```
{ cc = 1, type = "enc", command_up = "sim/instruments/barometer_up", command_down = "sim/instruments/barometer_down" }

{ cc = 4, type = "enc", command_up = "sim/autopilot/airspeed_up", command_down = "sim/autopilot/airspeed_down", command_fast_up = "FlyWithLua/airspeed_sel_10_up", command_fast_up = "FlyWithLua/airspeed_sel_10_down" }
```
*The first example maps the altimeter setting to a knob. The second example maps the heading knob and uses two custom 
Lua commands for a faster heading up and down*

###
### Internal
*Not implemented in the current version*