# Aircraft Profiles

All settings for an aircraft are stored in a profile. This allows you to have several configuration for different
aircraft. It is also possible to use a common profile, which will be used if no aircraft profile has been found. The 
aircraft profile uses the TOML language and holds information about all MIDI devices to be used and the mappings of the 
encoders, buttons and sliders.

Once the aircraft has been loaded in X-Plane, the plugin will search the following locations in this order:

## 1. Aircraft Directory

The plugin will search for a file called **XMidiCtrl.toml** in the directory of the current aircraft.

## 2. Profiles Directory

If no profile has been located in the aircraft directory, it will search for the configuration in the **profiles**
directory:

```
../X-Plane 11/Output/preferences/XMidiCtrl/profiles
```

### 2.1 Profiles Directory - Designer and ICAO

In order to allow individual profiles for the same aircraft from different developers, the plugin will search for a
filename that includes the developer name as well as the ICAO code for the current aircraft.

**Definition:**
```
<Developer> _ <ICAO> _ XMidiCtrl.toml
```

**Example:**
```
ToLiss_A321_XMidiCtrl.toml
```

### 2.2 Profiles Directory - ICAO

If the search was not successful, it will search again, but this time without the name of the developer.

**Definition:**
```
<ICAO> _ XMidiCtrl.toml
```

**Example:**
```
A321_XMidiCtrl.toml
```

## 3. Profiles Directory - Common profile

If the plugin was not able to locate any profile so far, it will search for a file called **XMidiCtrl.toml** in the
**profiles** directory. Obviously, the common profile should not contain any aircraft specific commands or datarefs.
