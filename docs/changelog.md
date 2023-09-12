
# XMidiCtrl - Changelog

-----------------------------------------------------------------------------------------------------------------------

##  VERSION 1.05
+ Fixed check for min/max value when modifier_up is negative
+ Added new outbound message type "constant"
+ Added option to delay encoders (parameter "delay") 
+ Display ACF-Filename in profile window
+ Removed depreciated mappings (cc / mapping)
+ Added virtual device
+ Updated X-Plane SDK to version 4.0.1

-----------------------------------------------------------------------------------------------------------------------

##  VERSION 1.04
+ Added universal binary for macOS
+ Updated X-Plane SDK to version 4.0.0, Beta 1
+ Removed debug screen message when executing XMidiCtrl commands

-----------------------------------------------------------------------------------------------------------------------

##  VERSION 1.03
+ Fixed small error in CL650 example mapping
+ Added default encoder mode in device settings
+ Moved logfile into preference folder where it should have been from the beginning on
+ Added reload button on general page of aircraft profile

-----------------------------------------------------------------------------------------------------------------------

##  VERSION 1.02

+ Improved logging system to fix crashes on Linux platform
+ Fixed mapping in CL650 example
+ Fixed crash on Mac OS when closing info window

-----------------------------------------------------------------------------------------------------------------------

##  VERSION 1.01

+ Fixed segmentation fault in Linux systems
+ Added option to disable the info window globally
+ Adjusted size of profile window

-----------------------------------------------------------------------------------------------------------------------

##  VERSION 1.00

+ Some minor logging improvements

-----------------------------------------------------------------------------------------------------------------------

##  VERSION 0.92

+ Added new mapping type to send init messages once after the aircraft profile has been loaded

-----------------------------------------------------------------------------------------------------------------------

##  VERSION 0.91

+ Added test function when setting up the info window to check the window position immediately
+ Added reset button for info window settings
+ Info window position will be saved correctly now
+ Info window will not block input for other windows

-----------------------------------------------------------------------------------------------------------------------

##  VERSION 0.90

+ Added label to display messages on the screen
+ Added dataref support for Push&Pull mappings
+ Added new parameters send_on / send_off for outbound messages
+ Complete rewrite of the logging
+ Rewrite of settings, profile and messages windows
+ Added example for iniBuilds A310 and Felis B747-200
+ Updated example for ToLiss A321

-----------------------------------------------------------------------------------------------------------------------

##  VERSION 0.70

+ Added support for note, pitch bend and program change midi messages
+ Enhanced mapping for outbound messages (value arrays)
+ Added new mode (momentary) for inbound dataref mappings
+ Improved documentation
+ Fixed mapping problem in ZIBO Boeing 738 example
+ Added new parameters value_min and value_max for encoder dataref mappings
+ Added option to disable the common aircraft profile

-----------------------------------------------------------------------------------------------------------------------

##  VERSION 0.62

+ Sliders can now be defined with a reversed axis by swapping value_min / value_mix (e.g. value_min = "1", value_max = "0")
+ If the connection to a MIDI device is lost, processing outbound messages will be stopped
+ Added new menu item for official support forum
+ Added support for Mac, many thanks to TwinFan (LiveTraffic) for his help!
+ Adjusted readme file
+ Added support for reading and writing datarefs, use format sim/cockpit/switches[2]
+ Included printable overlay for A321 example
+ Included template for Behringer X-Touch Mini overlay

-----------------------------------------------------------------------------------------------------------------------

##  VERSION 0.61

+ Fixed small mapping error in IXEG 737-300 example
+ Fixed debug message for Push&Pull End command, as it was logging the pull instead of the push command
+ Don't show messages window if no aircraft profile was found
+ Improved mapping for Hot Start Challenger 650 (ISI Baro instead of FO Baro)
+ Changed load event for aircraft profiles to prevent dataref error with the Hot Start Challenger 650
+ Internal refactoring for better code readability
+ Added new mapping mode for encoders (range instead of relative)

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
+ Added example configuration for Hot Start TBM 900
+ Added example configuration for IXEG 737-300
+ Added new option to map an encoder to a dataref instead of commands
+ Added new option to map a slider to a dataref instead of commands
+ New option to limit the number of messages to be shown in the messages window (for performance reasons)
+ Added a Linux version of the plugin

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
