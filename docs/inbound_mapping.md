# Inbound Mappings

Incoming MIDI messages can be mapped to one more multiple functions. Easiest way is to map a button to a single X-Plane 
command. However, more advanced options are possible, such as changing datarefs directly.

Incoming mappings are defined for each MIDI device individually using the parameter `mapping_in`.

## Required Parameters

The following parameter is required for each mapping.

| Parameter | Description                            |
|-----------|----------------------------------------|
| ch        | MIDI Channel (Default Value = 11)      |
 | type      | Specifies the mapping type (see below) |  

One of the following parameters is required, depending on the MIDI message type.

| Parameter | MIDI Message Type | Expected Parameter Value |
|-----------|-------------------|--------------------------|
| cc        | Control Change    | Control Change No Number |
| note      | Note On / Off     | Note Number              |
| pitch     | Pitch Bend        | Pitch Bend Number        |
| prg       | Program Change    | Program Change Number    |

## Optional Parameters

| Parameter | Description            |
|-----------|------------------------|
| sl        | Sublayer               |

## Mapping Types

The following mapping types are supported: 

| Mapping Type |   Name    | Description and usage                                                            |
|:------------:|:---------:|----------------------------------------------------------------------------------|
|     cmd      |  Command  | Executes a X-Plane command                                                       |
|     pnp      | Push&pull | Simulates a Push & pull button, which can execute two different commands         |   
|     drf      |  Dataref  | Toggles a given Dataref between two values                                       |
|     enc      |  Encoder  | Executes different commands for up/down or modifies a given dataref              |
|     sld      |  Slider   | Executes up to three different commands, depending on the location of the slider |

## Multi-Map

It is possible to map a MIDI message to multiple mappings. The following example will execute both commands,
when a MIDI message on Channel 11 and Control Change 25 is being received:

```
mapping = [
    { ch = 11, cc = 25, type = "cmd", command = "A300/accept_master_caution" },
    { ch = 11, cc = 25, type = "cmd", command = "A300/accept_master_warning" }
]
```

## Example

Working example for a full profile, including two MIDI devices and several inbound mappings.

```
title   = "Settings for my favourite aircraft"
version = "1.00"

# Behringer X-Touch Mini No. 1
[[device]]
name       = "Behringer X-Touch Mini 1"
port_in    = 0
port_out   = 1
mapping_in = [
  { ch = 11, cc = 1, type = "enc", command_up = "sim/GPS/g1000n1_baro_up", command_down = "sim/GPS/g1000n1_baro_down" },
  { ch = 11, cc = 2, type = "enc", command_up = "sim/autopilot/heading_up", command_down = "sim/autopilot/heading_down" },
  { ch = 11, cc = 3, type = "enc", command_up = "sim/GPS/g1000n1_crs_up", command_down = "sim/GPS/g1000n1_crs_down" },
  { ch = 11, cc = 4, type = "enc", command_up = "sim/GPS/g1000n1_range_up", command_down = "sim/GPS/g1000n1_range_down" }
]

# Behringer X-Touch Mini No. 2
[[device]]
name       = "Behringer X-Touch Mini 2"
port_in    = 1
port_out   = 2
mapping_in = [
  { ch = 10, cc = 1, type = "enc", command_up = "sim/GPS/g1000n3_range_up", command_down = "sim/GPS/g1000n3_range_down" },
  { ch = 10, cc = 2, type = "enc", command_up = "sim/autopilot/altitude_up", command_down = "sim/autopilot/altitude_down" },
  { ch = 10, cc = 3, type = "enc", command_up = "sim/autopilot/vertical_speed_down", command_down = "sim/autopilot/vertical_speed_up" },
  { ch = 10, cc = 4, type = "enc", command_up = "sim/GPS/g1000n3_crs_up", command_down = "sim/GPS/g1000n3_crs_down" }
]   
```
