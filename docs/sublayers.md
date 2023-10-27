# Sublayers

Sublayers allow the definition of additional layers in the device mappings. These mappings will be filtered during the execution phase, depending on the value of a defined dataref in X-Plane. This feature works for inbound and outbound mappings.

To enable this feature, the dataref top be read has to be defined for the device. The dataref will be read during every flight loop. If the parameter sl has been set in a mapping, it will be checked against the value of the dataref.

**Examples:**

```
# Behringer X-Touch Mini No. 1
[[device]]
name       = "Behringer X-Touch Mini 1"
port_in    = 0
port_out   = 1
sl_dataref = "xmidictrl/sublayer"
mapping_in = [
  { ch = 11, cc = 2, sl = "0", type = "enc", command_up = "laminar/B738/pilot/barometer_up", command_down = "laminar/B738/pilot/barometer_down" },
  { ch = 11, cc = 2, sl = "1", type = "enc", command_up = "laminar/B738/autopilot/heading_up", command_down = "laminar/B738/autopilot/heading_dn" }
]
```

Example for setting the Barometer 

```
# Behringer X-Touch Mini No. 1
[[device]]
name       = "Behringer X-Touch Mini 1"
port_in    = 0
port_out   = 1
sl_dataref = "xmidictrl/sublayer"
mapping_in = [
  { ch = 11, cc = 1, type = "cmd", command = "xmidictrl/toggle_sublayer" },
  
  { ch = 11, cc = 2, sl = "0", type = "enc", command_up = "sim/instruments/barometer_up", command_down = "sim/instruments/barometer_down" },
  { ch = 11, cc = 2, sl = "1", type = "enc", dataref = "sim/cockpit/misc/barometer_setting", modifier_up = 0.03, modifier_down = -0.03 }
]
```