
# XMidiCtrl - Changelog

-----------------------------------------------------------------------------------------------------------------------

##  VERSION 0.60

+ Changed mapping error in profile for ZIBO 737-800 (CC 11 -> DH)
+ Changed license from MIT to AGPL
+ Improved column widths in messages window
+ Fixed mapping error in ZIBO example (many thanks to daan_vb for pointing them out)
+ Implemented new feature to use sublayers for HotStart CL650
+ Rewrote Push&Pull mapping, the pull command will be executed while the button is pressed down longer than 0.5 seconds
+ Added option to define two different datarefs for outbound messages, e.g. sim/pilot/caution and sim/copilot/caution
+ Added option to define multiple values for inbound dataref messages, e.g. toggle through autobrake settings
+ Added custom commands to bind most menu entries of XMidiCtrl to buttons and keyboard commands
+ Added threads for sending outbound messages to improve the performance
+ Velocities for button pressed/released can be defined for inbound command mappings
+ Velocities for light on/off can be defined for outbound dataref mappings
+ Improved documentation
+ Added example configuration for Hot Start CL-650, created by Reflected Reality Simulations

-----------------------------------------------------------------------------------------------------------------------

##  VERSION 0.50

+ Reduced plugin size for the Microsoft Windows platform
+ Corrected spelling error in A321 example
+ Corrected encoder_cmd example of XMidiCtrl.toml file for the Behringer X-Touch Mini
+ Enabled multiple mappings for the same control change
+ Enhanced the general logging
+ Enhanced mapping options for sliders (up, middle, down)
+ Added midi outbound messages
+ Added settings window
+ Added messages window
+ Added aircraft profile window
+ Improved documentation
+ Added comprehensive example for the ZIBO 737-800

-----------------------------------------------------------------------------------------------------------------------

## VERSION 0.03

Breaking Changes:
+ Changed parameter names portOut and portIn to port_out and port_in. Please update your config files
+ Changed extension of profiles from cfg to toml (e.g. xmidictrl.toml)
+ Changed parameter names command_up_fast and command_down_fast to command_fast_up and command_fast_down (for Encoders)

-----------------------------------------------------------------------------------------------------------------------

## VERSION 0.02

+ First initial release