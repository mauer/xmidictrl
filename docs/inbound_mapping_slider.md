# Inbound Mapping Type: Slider

## Description

The mapping for a slider can use X-Plane commands or a dataref. If you define commands and a dataref, the
dataref will be used.

## Commands Mapping

The mapping with commands allow you to define command which will be executed when the slider is right at the top, the
bottom and/or in the middle. That's perfect to map three-way switches or even the landing gear. 

### Required Parameters

| Parameter    | Description                                                           |
|--------------|-----------------------------------------------------------------------|
| command_up   | Command which will be executed when the slider is right at the top    |
| command_down | Command which will be executed when the slider is right at the bottom |

### Optional Parameters

| Parameter      | Description                                                     |
|----------------|-----------------------------------------------------------------|
| command_middle | Command which will be executed when the slider is in the middle |

### Examples

```
{ ch = 11, cc = 0, type = "sld", command_up = "sim/engines/carb_heat_off", command_down = "sim/engines/carb_heat_on" }
```
*Opens carb heat when the slider is all the way up.*

## Dataref Mapping

This mapping allows you to map a dataref to the slider. Apart from the dataref itself you can define a minimum and 
maximum value. In most cases those will be 0 and 1, which are also the default values. 

### Required Parameters

| Parameter | Description                      |
|-----------|----------------------------------|
| dataref   | The dataref which should be used |

### Optional Parameters

| Parameter | Description                                    |
|-----------|------------------------------------------------|
| value_min | The minimum value of the dataref (Default 0.0) |
| value_max | The maximum value of the dataref (Default 1.0) |

### Examples

```
{ ch = 11, cc = 0, type = "sld", dataref = "tbm900/knobs/lights/panel_brt", value_min = 0, value_max = 0.90 },
```

*Binds the light knob of the Hot Start TBM 900 to a slider*