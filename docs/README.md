# MIDI Controller plugin for X-Plane
XMidiCtrl is a plugin for X-Plane 11 and allows you to connect MIDI controllers to commands and datarefs without the
need of additional software.

The following devices are officially supported:

* Behringer X Touch Mini

Other midi devices should work, as long as they send MIDI Control Change (CC) messages.

# Supported Operating Systems
This version supports Microsoft Windows, macOS and Linux operating systems. Please that I'm unable to test the macOS
build, as I don't own any computers from Apple.

# Logging
XMidiCtrl does not log directly in the **Log.txt** of X-Plane. Instead, it's using an own log file called
**XMidiCtrl_log.txt**. It's located in your X-Plane base folder.

# Templates
I included a template for the *Behringer X-Touch Mini* in the **templates** folder of the installation package. This 
template includes an example mapping and further information about the supported mapping options. I have also included 
my hardware configuration of the *Behringer X-Touch Mini* in the same folder. You can apply those settings using the 
*X-Touch Config Tool* from Behringer.

To use the template file, simply copy it into your aircraft folder and start adding commands or datarefs for your 
buttons and encoders.

# Usage
When a new aircraft is loaded, the plugin will search in the aircraft directory for a profile file called 
**midictrl.toml**. The profile uses the TOML language and holds information about the MIDI devices to be used and the 
mappings of the encoders and buttons.  Further information about the TOML syntax can be found [here](https://toml.io/en/).

The configuration of xmidictrl might be a bit intimidating at the beginning, but it is quite straight forward. Please
don't hesitate to contact me if you run into problems.

# Examples
I have included examples of my profile files in the **examples** folder.

## Thanks
Many thanks to [JetBrains](https://www.jetbrains.com/) for sponsoring a free license of 
[CLion](https://www.jetbrains.com/clion/) for the development of XMidiCtrl.

## License
XMidiCtrl is written by Marco Auer and release under the MIT License.

&copy; 2021 Marco Auer