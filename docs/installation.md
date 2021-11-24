# Installation

XMidiCtrl has to be installed in the **plugin** directory of your X-Plane installation. The **plugin** directory is
located in the **Resources** directory:
```
../X-Plane 11/Resources/plugins/
```

1. Download the latest release from the [xmidictrl GitHub](https://github.com/mauer/xmidictrl/releases) webpage. Please 
download the binary package and not the source code:
   ![Release Download](assets/github_release_download.png)
2. Extract the downloaded zip file to some temporary directory
3. Move the folder **XMidiCtrl** into your plugins directory

The installation of XMidiCtrl includes several directories:

```
XMidiCtrl
|- win_x64                     --> plugin binary for Microsoft Windows
|- templates                   --> template configurations for MIDI devices
|  |- Behringer X-Touch Mini   --> configuration for the Behringer X-Touch Mini
|- examples                    --> example configurations for various aircrafts 
```

All settings for the plugin are stored outside the plugin directory! You will find those in the **XMidiCtrl** folder 
of the **preference** folder:

```
../X-Plane 11/Output/preferences/XMidiCtrl
```

I have chosen this method to keep the upgrade process nice and simple.