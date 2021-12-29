# FAQ

Some often asked questions and problems.

## XMidiCtrl cannot connect to my MIDI device
Make sure to use the correct port and that the MIDI device isn't used by any other 
software. MIDI devices cannot be used from multiple applications at the same time.

## I get the error message "Invalid status '...', only Control Change messages are supported"
Your controller is probably configured to send note on/off messages. Right now XMidiCtrl only supports
control change messages. If you are using the Behringer X-Touch Mini then you can configure which message 
will be sent, by using the X-Touch Tool from Behringer. 