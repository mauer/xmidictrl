# Init Mappings

Init mappings are processed once the aircraft profile has been loaded. They are sent once only and used to initialise
the MIDI device. A typical example would be configuring the encoder lights on the Behringer X-Touch Mini.

All init mappings are defined for each MIDI device individually using the parameter `mapping_init`.

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
  { ch = 1, cc = 1, value = 1 },
  { ch = 1, cc = 2, value = 2 },
  { ch = 1, cc = 3, value = 3 },
  { ch = 1, cc = 4, value = 4 },
  { ch = 1, cc = 5, value = 4 },
  { ch = 1, cc = 6, value = 3 },
  { ch = 1, cc = 7, value = 2 },
  { ch = 1, cc = 8, value = 1 }
]
```

## Required Parameters

The following parameter are required for each mapping.

| Parameter | Description                                                                                        |
|-----------|----------------------------------------------------------------------------------------------------|
| ch        | MIDI Channel (Default Value = 11)                                                                  |
| value     | Specifies the value for the outbound message. Required for all message types, except Note messages |
| velocity  | Note message only! Specifies the velocity for the outbound message                                 |

One of the following parameters is required, depending on the MIDI message type.

| Parameter | MIDI Message Type | Expected Parameter Value |
|-----------|-------------------|--------------------------|
| cc        | Control Change    | Control Change No Number |
| note      | Note On / Off     | Note Number              |
| pitch     | Pitch Bend        | Pitch Bend Number        |
| prg       | Program Change    | Program Change Number    |