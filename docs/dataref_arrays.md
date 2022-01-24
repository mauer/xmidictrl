# Dataref Arrays

XMidiCtrl support Dataref arrays, as well. You can use arrays in all mappings that support Datarefs. When using an array 
you have to specify the index for the required item.

The syntax for using an array would be: 
```
<dataref name> + [ + <index> + ]
``` 

## Example

```
{ ch = 11, cc = 30, type = "drf", dataref = "sim/cockpit2/switches[5]", value_on = "1", value_off = "0" }
```