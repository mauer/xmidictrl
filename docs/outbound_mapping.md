# Outbound Mappings

Outbound mappings are used to set a status on your MIDI controller, such as highlight a button if a certain function
got activated in X-Plane.

All outgoing mappings are defined for each MIDI device individually with the parameter **mapping_out**

## Example

```
title   = "Settings for my favourite aircraft"
version = "1.00"

# Behringer X-Touch Mini
[[device]]
name       = "Behringer X-Touch Mini"
port_in    = 0
port_out   = 1
mapping_out = [
  { ch = 11, cc = 18, type = "drf", dataref = "tbm900/lights/ap/hdg", value_on = "1", value_off = "0" },
  { ch = 11, cc = 19, type = "drf", dataref = "tbm900/lights/ap/apr", value_on = "1", value_off = "0" },
  { ch = 11, cc = 20, type = "drf", dataref = "tbm900/lights/ap/nav", value_on = "1", value_off = "0" },
  { ch = 11, cc = 22, type = "drf", dataref = "tbm900/lights/ap/alt", value_on = "1", value_off = "0" }
]
```

## Common variables

The following variables are required for each mapping.

| Variable | Description                       |
|----------|-----------------------------------|
| ch       | MIDI Channel (Default Value = 11) |
| cc       | Control Change No Number          |

## Mapping Types

The following mapping types are supported:

| Mapping Type |  Name   | Description and usage                                         |
|:------------:|:-------:|---------------------------------------------------------------|
|     drf      | Dataref | Reads the dataref value and sends a MIDI message if necessary |

