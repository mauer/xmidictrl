# Configuration of XMidiCtrl

XMidiCtrl uses configuration files in the [TOML](https://toml.io/en/) format. I have chosen this format, as it is easy 
to read and offers the option to include comments in the file. I recommend Visual Studio Code from Microsoft to edit
the configuration files as it includes syntax highlighting and checking for TOML.

The configuration for an aircraft is called an **Aircraft Profile** in XMidiCtrl. Those profiles define the MIDI devices 
which should be used, as well as the mappings for each device. You can have a look at the included example profiles in 
the `examples` directory of the XMidiCtrl installation. They should be a good starting point to create your on aircraft 
configurations.

## TOML Syntax

TOML is using a very simple key/value pair syntax. For example, to set the version number in the config file you just 
have to type:

```
version = "1.0"
```

Comments are easily included by putting a hash symbol before the text:

```
# This is the title of my configuration
title = "My aircraft profile for the Cessna 172"

version = "1.0" # current version
```

Please have a look at the [TOML](https://toml.io/en/) webpage for more detailed information. It offers a nice quick 
tour of TOML, which describes the syntax rather nicely.