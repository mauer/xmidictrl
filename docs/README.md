# MIDI Controller plugin for X-Plane
XMidiCtrl is an X-Plane 11plugin to use midi controllers within the flight simulator

The following devices are currently supported:

* Behringer X Touch Mini

Other midi devices should work, as long as they send Control Change messages.

# Installation
Place the plugin into the plugin directory of X-Plane 11.

# Usage
When a new aircraft is loaded, the plugin will search in the aircraft directory for a configuration file called 
xmidictrl.cfg

## Mapping Types
Each Control Change Message can be mapped to one of the following function.

| Mapping Type | Name      | Description and Usage                                                         |
|:------------:|-----------|-------------------------------------------------------------------------------|
| CMD          | Command   | Executes a X-Plane command                                                    |
| PNP          | Push&Pull | Simulates a Push&Pull button and can therefore execute two different commands |   
| DRF          | DataRef   | Toggles a given DataRef between two values                                    |
| ENC          | Encoder   | Executes different commands for up/down and fast up/fast down                 |
| INT          | Internal  | Performs internal commands, provided by XMidiCtrl                             |

### Mapping Type: Command
Executes a X-Plane command

| Variable | Description                                          |
|----------|------------------------------------------------------|
| command  | Defines the X-Plane command which should be executed |

**Example:**
```
{ CC = 67, type = "cmd", command = "sim/radios/com1_standy_flip" }
```
*Flips between active and standby frequency for COM1 when Control Change Message is equal to 67*

### Mapping Type: Push&Pull
This mapping simulates a push and pull button, as found in the Airbus A321. A short button press will execute the push
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
be executed. If the button is longer pressed command AirbusFBW/PullAltitude will be executed*

### Mapping Type: DataRef
Toggles a given DataRef between two values

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

### Encoder
tbd

### Internal
tbd

## Thanks
Many thanks to [JetBrains](https://www.jetbrains.com/) for sponsoring a free license of 
[CLion](https://www.jetbrains.com/clion/) for the development of XMidiCtrl.

## License
XMidiCtrl is written by Marco Auer

&copy; 2021 Marco Auer