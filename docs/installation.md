# Installation

XMidiCtrl has to be installed in the **plugin** directory of your X-Plane installation. The **plugin** directory is
located in the **Resources** directory:
```
../X-Plane 11/Resources/plugins/
```

1. Download the latest release from the [XMidiCtrl GitHub](https://github.com/mauer/xmidictrl/releases) webpage. Please 
download the binary package and not the source code:
   ![Release Download](assets/github_release_download.png)
2. Extract the downloaded zip file to some temporary directory
3. Move the folder **XMidiCtrl** into your plugins directory

The installation of XMidiCtrl includes several directories:

```
XMidiCtrl
|- win_x64                     --> plugin binary for Microsoft Windows
|- templates                   --> template configurations for MIDI devices
|  |- Behringer X-Touch Mini   --> configuration and template for the Behringer X-Touch Mini
|- examples                    --> example configurations for various aircrafts 
```

All settings for the plugin are stored outside the plugin directory! You will find those in the **XMidiCtrl** folder 
within the **preferences** folder:

```
../X-Plane 11/Output/preferences/XMidiCtrl
```

# MacOS Security Warning
Please bear in mind that this plugin is not officially signed by Apple Inc., which will likely raise a security popup 
the first time you run X-Plane after the plugin installation.

The message will be something like:
`"XMidiCtrl.xpl" cannot be opened because the developer cannot be verified.`

**To solve this:**
* Click `Cancel`. The plugin will not load this time. 
* Now, open you macOS system settings and go to `Security & Privacy`, page `General`. You will see a note saying that 
_XMidiCtrl_ was blocked.
* Click on the provided `Allow Anyway` button. 
* Restart X-Plane. 
* You will now see a different security warning: `macOS cannot verify the developer of "XMidiCtrl.xpl". Are you sure you want to open it?`
* Click Open. X-Plane and the XMidiCtrl plugin will now load.

From now on, X-Plane and XMidiCtrl will start just fine. You will need to repeat this procedure only with the next plugin update.