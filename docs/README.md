# MIDI Controller plugin for X-Plane
XMidiCtrl is a plugin for X-Plane 11 and allows you to connect MIDI controllers to Commands and DataRefs without the
need of additional software.

Please note that this is still a very early version of the plugin. Although, I use the plugin every day with my 
Behringer X-Touch Mini without any problems. In the future, I would like to add a nicer User Interface, better logging 
and MIDI out to show the state of DataRefs directly on the midi controller (e.g. Autopilot on/off).

The following devices are officially supported:

* Behringer X Touch Mini

Other midi devices should work, as long as they send Control Change messages.

# Supported Operating Systems
This very first version does support Microsoft Windows, only. However, I will add support for Linux and macOS with the
next version.

# Installation
Copy the entire **XMidiCtrl** folder into your **..\X-Plane\Resources\plugins** directory.

# Templates
I included a template for the Behringer X-Touch Mini in the **templates** folder of this addon. This template includes
example mapping and further information about the supported mapping options. I have also included my hardware
configuration of the Behringer X-Touch Mini in the same folder. You can apply those settings using the X-Touch Config
Tool from Behringer.

To use the template file, simply copy it into your aircraft folder and start adding commands for your buttons and
encoders.

# Logging
XMidiCtrl does not log directly in the **Log.txt** of X-Plane. Instead, it's using an own log file called 
**XMidiCtrl_log.txt** You will find it in your X-Plane base folder.

# Usage
When a new aircraft is loaded, the plugin will search in the aircraft directory for a profile file called 
**midictrl.toml**. The profile uses the TOML language and holds information about the MIDI devices to be used and the 
mappings of the encoders and buttons. There is no User Interface to configure the mapping. It has to be done outside 
X-Plane using a text editor or Visual Studio Code. I would recommend Visual Studio Code as it includes syntax 
highlighting for TOML. Further information about the TOML syntax can be found [here](https://toml.io/en/).

It's important to add the correct ports for your MIDI device in the profile. Those can be defined using the port_in and
port_out parameter.

**Example:***
```
# Behringer X-Touch Mini
[[device]]
name = "Behringer X-Touch Mini"
port_in = 0
port_out = 1
```
In order to determine the ports for your MIDI device, you can use the function **Search for MIDI Devices** of the 
plugin. You can find it in the menu **Plugins -> XMidiCtrl -> Search for MIDI Devices**. The dialog will present you
with a list of all connected MIDI devices and their ports.

The configuration of XMidiCtrl might be a bit intimidating at the beginning, but it is quite straight forward. Please
don't hesitate to contact me if you run into problems.

## Mapping Types
Each Control Change Message can be mapped to one of the following function.

| Mapping Type | Name      | Description and Usage                                                         |
|:------------:|-----------|-------------------------------------------------------------------------------|
| cmd          | Command   | Executes a X-Plane command                                                    |
| pnp          | Push&Pull | Simulates a Push&Pull button and can therefore execute two different commands |   
| drf          | DataRef   | Toggles a given DataRef between two values                                    |
| enc          | Encoder   | Executes different commands for up/down and fast up/fast down                 |
| int          | Internal  | Performs internal commands (not implemented yet)                              |

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
{ CC = 67, type = "cmd", command = "sim/radios/com1_standy_flip" }
```
*Flips between active and standby frequency for COM1 when Control Change Message is equal to 67*

###
### Mapping Type: Push&Pull
This mapping simulates a push and pull button, as found in Airbus aircrafts. A short button press will execute the push
command, while a longer button press will execute the pull command.

| Variable     | Description                                          |
|--------------|------------------------------------------------------|
| command_push | Defines the command to be executed for a push action |
| command_pull | Defines the command to be executed for a pull action |

**Example:**
```
{ CC = 14, type = "pnp", command_push = "AirbusFBW/PushAltitude", command_pull = "AirbusFBW/PullAltitude" }
```
*If the button for Control Change Message 14 is pressed and immediately released the command AirbusFBW/PushAltitude will 
be executed. If the button is pressed longer then command AirbusFBW/PullAltitude will be executed*

###
### Mapping Type: DataRef
Toggles a given DataRef between two values. This is very useful when no command was provided by the aircraft developer.

| Variable  | Description                                                                                |
|-----------|--------------------------------------------------------------------------------------------|
| dataref   | Defines the DataRef which should be changed                                                |
| value_on  | Defines the value the DataRef should be set to, if the current value is equal to value_off |
| value_off | Defines the value the DataRef should be set to, if the current value is equal to value_on  |

**Example:**
```
{ CC = 68, type = "drf", dataref = "AirbusFBW/SDENG", value_on = "1", value_off = "0" }
```
*Changes DataRef AirbusFBW/SDENG to value 1 if the current value is 0 and the Control Change Message is equal to 68*

###
### Encoder
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
{ CC = 1, type = "enc", command_up = "sim/instruments/barometer_up", command_down = "sim/instruments/barometer_down" }

{ CC = 4, type = "enc", command_up = "sim/autopilot/airspeed_up", command_down = "sim/autopilot/airspeed_down", command_fast_up = "FlyWithLua/airspeed_sel_10_up", command_fast_up = "FlyWithLua/airspeed_sel_10_down" }
```
*The first example maps the altimeter setting to a knob. The second example maps the heading knob and uses custom lua
commands for a faster heading up and down*

###
### Internal
*Not implemented in the current version*

# Examples
I have included examples of my profile files in the **examples** folder.

## Thanks
Many thanks to [JetBrains](https://www.jetbrains.com/) for sponsoring a free license of 
[CLion](https://www.jetbrains.com/clion/) for the development of XMidiCtrl.

## License
XMidiCtrl is written by Marco Auer and release under the MIT License.

&copy; 2021 Marco Auer