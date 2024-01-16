# MIDI Controller plugin for X-Plane 11.50+ / 12

[![build-all-platforms](https://github.com/mauer/xmidictrl/actions/workflows/build.yml/badge.svg)](https://github.com/mauer/xmidictrl/actions/workflows/build.yml)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=mauer_xmidictrl&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=mauer_xmidictrl)
[![pages-build-deployment](https://github.com/mauer/xmidictrl/actions/workflows/pages/pages-build-deployment/badge.svg)](https://github.com/mauer/xmidictrl/actions/workflows/pages/pages-build-deployment)

XMidiCtrl is a plugin for X-Plane 11/12 and allows the connection of MIDI devices to commands and datarefs without the
need of additional software.

The following devices are officially supported:

* Behringer X Touch Mini

The following devices are known to work:

* Behringer BCR2000
* Behringer X-Touch Compact
* Loupedeck Live

Most MIDI controllers should work, as long as they can send and receive standard MIDI messages.

## Supported Operating Systems

This version supports Microsoft Windows 10/11, Linux and macOS 10.15+.

## Logging

XMidiCtrl does not log directly in the **Log.txt** of X-Plane. Instead, it's using its own logfile called
**XMidiCtrl_Log.txt**. The logfile gets written into the XMidiCtrl preference folder, which can be found here:

```
<X-Plane Path>/Output/preferences/XMidiCtrl
```

## Templates

I included a template for the *Behringer X-Touch Mini* in the **templates** folder of the installation package. This 
template includes an example mapping and further information about the supported mapping options. I have also included 
my hardware configuration of the *Behringer X-Touch Mini* in the same folder. You can apply those settings using the 
*X-Touch Editor* from Behringer.

To use the template file, simply copy it into your aircraft folder and start adding commands and/or datarefs for your 
buttons and encoders.

## Usage

When a new aircraft is loaded, the plugin will search in the aircraft directory for a file called **XMidiCtrl.toml**. 
The profile uses the TOML language and holds information about the MIDI devices to be used and the mappings of the 
encoders and buttons.  Further information about the TOML syntax can be found [here](https://toml.io/en/).

The configuration of XMidiCtrl might be a bit intimidating at the beginning, but it is quite straight forward. Please
don't hesitate to contact me if you run into problems.

## Examples

Some examples for aircraft profiles can be found in the **examples** folder of the XMidiCtrl installation.

## Documentation

A detailed documentation of the plugin and all mapping options can be found [here](https://mauer.github.io/xmidictrl/#/).

## Support

If you need support or want to chat about the plugin then please visit the official support [forum](https://forums.x-pilot.com/forums/forum/274-xmidictrl/).

## Thanks

Many thanks to [JetBrains](https://www.jetbrains.com/) for sponsoring a free license of 
[CLion](https://www.jetbrains.com/clion/) for the development of XMidiCtrl.

## License

XMidiCtrl is written by Marco Auer and released under the GNU Affero General Public License .

&copy; 2021 - 2024 Marco Auer
