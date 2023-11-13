@ECHO OFF
TITLE Update the local copy of XMidiCtrl for testing

ECHO Update local copy of XMidiCtrl

Copy-Item "$pwd\package\XMidiCtrl\win_x64\XMidiCtrl.xpl" -Destination "F:\X-Plane 12\Resources\plugins\XMidiCtrl\win_x64"