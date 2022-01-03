# Mapping: Encoder (DataRef)
## Description

Mapping for an Encoder (knob) using an X-Plane DataRef. You can define the DataRef and some modifiers for up and down. 
In addition, it's possible to define modifiers for fast up and down, if the knob was turned faster. This can be useful 
in some aircraft. Defining just up and down should be sufficient in most cases.

The modifiers are expected to be numbers. They can be integers or floats (see Examples).

## Required Parameters

| Parameter     | Description                                                      |
|---------------|------------------------------------------------------------------|
| dataref       | DataRef which should be modified by the knob                     |
| modifier_up   | Modifier which will be used when the knob is turned to the right |
| modifier_down | Modifier which will be used when the knob is turned to the left  |

## Optional Parameters

| Parameter          | Description                                                                 |
|--------------------|-----------------------------------------------------------------------------|
| modifier_fast_up   | Modifier which will be used when the knob is turned quite fast to the right |
| modifier_fast_down | Modifier which will be used when the knob is turned quite fast to the left  |

## Examples

```
{ ch = 11, cc = 1,  type = "end", dataref = "ixeg/733/altimeter/altimeter_bug_pilot_ind", modifier_up = 0.02, modifier_down = -0.02 }

{ ch = 11, cc = 4,  type = "end", dataref = "ixeg/733/altimeter/altimeter_bug_pilot_ind", modifier_up = 0.02, modifier_down = -0.02, modifier_fast_up = 0.20, modifier_fast_down = -0.20 }
```
*The examples above change the altimeter bug for the pilot in the IXEG 737-300.*