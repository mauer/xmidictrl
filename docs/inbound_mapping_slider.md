# Inbound Mapping Type: Slider

## Description

The mapping for a slider can work with X-Plane commands or a dataref.

## Commands Mapping

The command mapping allows you to define three commands in total. One command when the slider is all the way up,
another one when the slider is all the way down and optionally a command when the slider is right in the middle.

For advanced mappings you can define the data 2 minimum and maximum value as well as a margin which should be used. The
margin sets the range when the commands get executed.

#### Example

| Parameter     | Value |
|---------------|-------|
| Data 2 Min    | 0     |
| Data 2 Max    | 127   |
| Data 2 Margin | 10    |

The commands will be executed when Data 2 has these values:

| Command        | Data 2 Range | Formula                                                |
|----------------|--------------|--------------------------------------------------------|
| Command Up     | 117 .. 127   | `Data 2 Max` - `Data 2 Margin`                         | 
| Command Middle | 53 .. 73     | (`Data 2 Max` - `Data 2 Min`) / `2` -+ `Data 2 Margin` | 
| Command Down   | 00 .. 10     | `Data 2 Min` + `Data 2 Margin`                         |

Each command will be executed on time until the slider leaves the range area and returns.

### Required Parameters

| Parameter    | Description                                                           |
|--------------|-----------------------------------------------------------------------|
| command_up   | Command which will be executed when the slider is right at the top    |
| command_down | Command which will be executed when the slider is right at the bottom |

### Optional Parameters

| Parameter      | Description                                                     |
|----------------|-----------------------------------------------------------------|
| command_middle | Command which will be executed when the slider is in the middle |
| data_2_min     | Minimum value of Data 2                                         |
| data_2_max     | Maximum value of Data 2                                         |
| data_2_margin  | Margin to be used (value has to be between 0 and 25)            |

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