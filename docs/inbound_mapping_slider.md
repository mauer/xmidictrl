# Mapping: Slider

## Description

I have to admit that the supported mapping for sliders is still rather basic. I will try to improve that with the
next version. Right now you can map three commands to a slider, which will be executed depending on its position.

## Required Parameters

| Parameter    | Description                                                          |
|--------------|----------------------------------------------------------------------|
| command_up   | Command which will be executed when the slider is right at the top   |
| command_down | Command which will be executed when the slider is right at the bottom |

## Optional Parameters

| Parameter      | Description                                                     |
|----------------|-----------------------------------------------------------------|
| command_middle | Command which will be executed when the slider is in the middle |

## Examples

```
{ ch = 11, cc = 0, type = "sld", command_up = "sim/engines/carb_heat_off", command_down = "sim/engines/carb_heat_on" }
```
*Opens carb heat when the slider is all the way up.*