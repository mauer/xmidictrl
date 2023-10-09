# Devices

You can define multiple MIDI devices in your profile. Each device needs several parameters, such as inbound / outbound
ports and some mappings.

# Required Parameters

| Parameter | Description                                                                            |
|-----------|----------------------------------------------------------------------------------------|
| port_in   | Port for inbound MIDI messages (only required if inbound mappings have been defined)   |
| port_out  | Port for outbound MIDI messages (only required if outbound mappings have been defined) |

> You have to define at least port_in or port_out. This depends on your MIDI device and mappings. If you don't plan to
> send any MIDI messages to your device then there is obviously no need to define an outbound port.

# Optional Parameters

| Parameter            | Description                                                                               |
|----------------------|-------------------------------------------------------------------------------------------|
| name                 | It's good practise to define a name for your device                                       |
| mode_note            | on_off = send note on/off messages (default) / on = send note on messages only            | 
| mode_out             | permanent = permanent mode (default) / on_change = on change mode (see description below) |
| mapping_in           | Mappings for inbound messages                                                             |
| mapping_out          | Mappings for outbound messages                                                            |
| outbound_delay       | Delays outbound messages by the specified number of seconds (default 0.5 sec)             |
| default_encoder_mode | Default encoder mode for the device (range/relative), default is relative                 |

# Which ports should be used?

In order to determine the ports for all your MIDI devices, you can open the window **MIDI Devices**. You can find it in 
the X-Plane menu `Plugins -> XMidiCtrl -> Show MIDI Devices`. The window will show you with a list of all connected 
MIDI devices and their ports.

# Parameter mode_out

This parameter defines if the current states for all outbound messages should be sent all the time, or only if the
mapped dataref changes. Set mode_out to 0 if you require constant updates of the buttons on your device and to 1 if
it's enough to send one MIDI message when the dataref changes.

If you are using the Behringer X-Touch Mini then I would recommend the permanent mode. The button states are only set
if the layer (A or B) is currently active. So speaking, if you have a button of layer A mapped to the caution light and
layer B is active when the caution light goes on, the button will not be illuminated when you switch back to layer A. 
If you are in the permanent mode, messages will be sent around every half second. 

> I recommend using the permanent mode. It does not have any impact on the X-Plane performance, as most operations are
> done in a separate worker thread.

# Example
```
# Behringer X-Touch Mini No. 1
[[device]]
name     = "Behringer X-Touch Mini 1"
port_in  = 0
port_out = 1

# Behringer X-Touch Mini No. 2
[[device]]
name     = "Behringer X-Touch Mini 2"
port_in  = 1
port_out = 2
```
