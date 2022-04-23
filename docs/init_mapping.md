# Init Mappings

Init mappings are processed once the aircraft profile has been loaded. They are only sent once and used to initialise
the MIDI device. A typical example would be configuring the encoder lights on the Behringer X-Touch Mini.

All init mappings are defined for each MIDI device individually using the parameter `mapping_out`.

## Example

```
title   = "Settings for my favourite aircraft"
version = "1.00"

# Behringer X-Touch Mini
[[device]]
name       = "Behringer X-Touch Mini"
port_in    = 0
port_out   = 1
mapping_init = [
  # Encoder Lights
  { ch = 1, cc = 1, velocity = 1 },
  { ch = 1, cc = 2, velocity = 2 },
  { ch = 1, cc = 3, velocity = 3 },
  { ch = 1, cc = 4, velocity = 4 },
  { ch = 1, cc = 5, velocity = 4 },
  { ch = 1, cc = 6, velocity = 3 },
  { ch = 1, cc = 7, velocity = 2 },
  { ch = 1, cc = 8, velocity = 1 }
]
```

## Common variables

The following parameter is required for each mapping.

| Parameter | Description                                     |
|-----------|-------------------------------------------------|
| ch        | MIDI Channel (Default Value = 11)               |
| velocity  | Specifies the velocity for the outbound message |  

One of the following parameters is required, depending on the MIDI message type.

| Parameter | MIDI Message Type | Expected Parameter Value |
|-----------|-------------------|--------------------------|
| cc        | Control Change    | Control Change No Number |
| note      | Note On / Off     | Note Number              |
| pitch     | Pitch Bend        | Pitch Bend Number        |
| prgchg    | Program Change    | Program Change Number    |