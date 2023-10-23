# Plugin Commands

XMidiCtrl adds a few custom commands to X-Plane, that allow you to bind certain functions of the plugin to joystick
buttons or keyboard events.

## Custom Commands

| Command                        | Description                                             |
|--------------------------------|---------------------------------------------------------|
| xmidictrl/show_log_window      | Show the log messages window                            |
| xmidictrl/show_messages_window | Show the MIDI messages window                           |
| xmidictrl/show_profile_window  | Show the aircraft profile window                        |
| xmidictrl/reload_profile       | Reload the aircraft profile                             |
| xmidictrl/toggle_sublayer      | Toggle the dataref 'xmidictrl/sublayer' between 0 and 1 | 

## Custom MIDI Commands
Those commands can be used together with a [virtual MIDI device](virtual_device.md).

| Command                             | Description                                                                |
|-------------------------------------|----------------------------------------------------------------------------|
| xmidictrl/midi/send_midi_cc_msg_001 | Send a virtual control change message for the specified channel and CC 1   |
| xmidictrl/midi/send_midi_cc_msg_002 | Send a virtual control change message for the specified channel and CC 2   |
| xmidictrl/midi/send_midi_cc_msg_003 | Send a virtual control change message for the specified channel and CC 3   |
| ...                                 | ...                                                                        |
| xmidictrl/midi/send_midi_cc_msg_125 | Send a virtual control change message for the specified channel and CC 125 |
| xmidictrl/midi/send_midi_cc_msg_126 | Send a virtual control change message for the specified channel and CC 126 |
| xmidictrl/midi/send_midi_cc_msg_127 | Send a virtual control change message for the specified channel and CC 127 |

