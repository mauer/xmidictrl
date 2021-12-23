# Inbound Mappings

Incoming MIDI messages can be mapped to one more multiple functions. Easiest way is to map a button to a simple X-Plane 
command. However, most advanced options are possible, such as changing DataRefs.

## Common variables
The following variables are required for each mapping.

| Variable | Description                       |
|----------|-----------------------------------|
| ch       | MIDI Channel (Default Value = 11) |
| cc       | Control Change No Number          |

## Mapping Types
The following mapping types are supported: 

| Mapping Type |   Name    | Description and usage                                                            |
|:------------:|:---------:|----------------------------------------------------------------------------------|
| cmd          |  Command  | Executes a X-Plane command                                                       |
| pnp          | Push&pull | Simulates a Push & pull button, which can execute two different commands         |   
| drf          |  Dataref  | Toggles a given Dataref between two values                                       |
| enc          |  Encoder  | Executes different commands for up/down and fast up/fast down                    |
| sld          |  Slider   | Executes up to three different commands, depending on the location of the slider |

###
### Mapping Type: Command
Executes an X-Plane command when a button is pressed. The command will be executed as long as the button is being
pressed down. This behaviour is useful to map the CLR button of the G530, as pressing the button for a longer time 
returns to the map screen.

| Variable | Description                                          |
|----------|------------------------------------------------------|
| command  | Defines the X-Plane command to be executed |

**Example:**
```
{ ch = 11, cc = 67, type = "cmd", command = "sim/radios/com1_standy_flip" }
```
*Flips between active and standby frequency for COM1 when Channel is 11 and Control Change is equal to 67.*

###
### Mapping Type: Push&pull
This mapping simulates a push and pull button, as found in Airbus aircraft. A short button press will execute the push
command, while a longer button press (~1 sec) will execute the pull command.

| Variable     | Description                                          |
|--------------|------------------------------------------------------|
| command_push | Defines the command to be executed for a push action |
| command_pull | Defines the command to be executed for a pull action |

**Example:**
```
{ ch = 11, cc = 14, type = "pnp", command_push = "AirbusFBW/PushAltitude", command_pull = "AirbusFBW/PullAltitude" }
```
*If the button for Channel 11 and Control Change 14 is pressed and immediately released the command 
'AirbusFBW/PushAltitude' will be executed. If the button is pressed for a longer time then command 
'AirbusFBW/PullAltitude' will be executed.*

###
### Mapping Type: DataRef
Toggles a given DataRef between two values. This is very useful when no command was provided by the aircraft developer.

| Variable  | Description                                                                                |
|-----------|--------------------------------------------------------------------------------------------|
| dataref   | Defines the DataRef to be changed                                                          |
| value_on  | Defines the value the DataRef should be set to, if the current value is equal to value_off |
| value_off | Defines the value the DataRef should be set to, if the current value is equal to value_on  |

**Example:**
```
{ ch = 11, cc = 68, type = "drf", dataref = "AirbusFBW/SDENG", value_on = "1", value_off = "0" }
```
*Changes DataRef 'AirbusFBW/SDENG' to value '1' if the current value is '0' and the other way round.*

###
### Mapping: Encoder
Mapping for an encoder (knob). You can define a command for up (right turn) and down (left turn). In addition, it's
possible to define commands for fast up and down, if the knob was turned faster. This can be useful in some
aircraft. Normally, defining up and down should be sufficient.

| Variable          | Description                                                                                |
|-------------------|--------------------------------------------------------------------------------------------|
| command_up        | Command which will be executed when the knob is turn to the right                          |
| command_down      | Command which will be executed when the knob is turn to the left                           |
| command_fast_up   | Command which will be executed when the knob is turn quite fast to the right (optional)    |
| command_fast_down | Command which will be executed when the knob is turn quite fast to the left (optional)     |

**Examples:**
```
{ ch = 11, cc = 1, type = "enc", command_up = "sim/instruments/barometer_up", command_down = "sim/instruments/barometer_down" }

{ ch = 11, cc = 4, type = "enc", command_up = "sim/autopilot/airspeed_up", command_down = "sim/autopilot/airspeed_down", command_fast_up = "FlyWithLua/airspeed_sel_10_up", command_fast_up = "FlyWithLua/airspeed_sel_10_down" }
```
*The first example maps the altimeter setting to a knob. The second example maps the heading knob and uses two custom 
Lua commands for a faster heading up and down.*

###
### Mapping: Slider
I have to admit that the supported mapping for sliders is still rather basic. I will try to improve that with the
next version. Right now you can map three commands to a slider, which will be executed depending on its position.

| Variable          | Description                                                                                |
|-------------------|--------------------------------------------------------------------------------------------|
| command_up        | Command which will be executed when the slider is right at the top                         |
| command_middle    | Command which will be executed when the slider is in the middle (optional)                 |
| command_down      | Command which will be executed when the slider is right at the bottom                      |

**Examples:**
```
{ ch = 11, cc = 0, type = "sld", command_up = "sim/engines/carb_heat_off", command_down = "sim/engines/carb_heat_on" }
```
*Opens carb heat when the slider is all the way up.*

###
### Multi-Map
It is possible to map a MIDI message to multiple mappings. The following example will execute both commands,
when a MIDI message with Channel 11 and Control Change 25 is being received:

```
mapping = [
    { ch = 11, cc = 25, type = "cmd", command = "A300/accept_master_caution" },
    { ch = 11, cc = 25, type = "cmd", command = "A300/accept_master_warning" }
]
```