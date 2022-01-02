# Devices

You can define several MIDI devices in your profile. Each device needs several parameters, such as port in/out and
of course some mappings.

# Required Parameters

| Parameter | Description                         |
|-----------|-------------------------------------|
| port_in   | Port for the inbound MIDI messages  |
| port_out  | Port for the outbound MIDI messages |

# Optional Parameters

| Parameter   | Description                                                               |
|-------------|---------------------------------------------------------------------------|
| name        | It's good practise to define a good name for your device                  |
| mode_out    | 0 = permanent mode (default) / 1 = on change mode (see description below) |
| mapping_in  | Mappings for inbound messages                                             |
| mapping_out | Mappings for outbound messages                                            |

# Which ports should be used?

In order to determine the ports for all your MIDI devices, you can use the function **Show MIDI Devices** of the plugin. 
You can find it in the menu **Plugins -> XMidiCtrl -> Show MIDI Devices**. The dialog will show you with a list of all 
connected MIDI devices and their ports.

# Parameter mode_out

This parameter defines if the current states for all outbound messages should be sent all the time, or only if the
mapped dataref changes. Set mode_out to 0 if you require constant updates of the buttons on your device and to 1 if
it's enough to send one MIDI message when the dataref changes.

If you are using the Behringer X-Touch Mini then I would recommend the permanent mode. The button states are only set
if the layer (A or B) is currently active. So speaking, if you have a button of layer A mapped to the caution light and
 layer B active when the caution light goes on, the button will not be illuminated when you switch back to layer A. If 
you are in the permanent mode, will be sent around every half second. 

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

