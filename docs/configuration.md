# Configuration of XMidiCtrl

XMidiCtrl uses configuration files in the [TOML](https://toml.io/en/) format. I have chosen this format, as it is easy 
to read and offers comments in the configuration. There is no User Interface to configure the mappings. It has to be 
done outside X-Plane using a text editor. I recommend Visual Studio Code as it includes syntax highlighting for TOML.

## TOML Syntax

TOML is using a very simple key/value pair syntax. To set the version number in the config file you just have to type:

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
