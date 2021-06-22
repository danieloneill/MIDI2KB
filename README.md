# MIDI2KB
Simple Windows application to translate MIDI to keyboard inputs for Guitar Hero: World Tour

![Screenshot](https://raw.githubusercontent.com/danieloneill/MIDI2KB/main/midi2kb.png)

## Usage:
On launch it should scan for available MIDI devices identified on your computer. If it doesn't or you plug in after, click Scan Devices.

Once you have it selected, click Connect and check the status bar at the bottom to confirm it's connected.

Now you should be getting rows filling up the table below for each MIDI event from your drums (or whatever).

Order isn't reliably maintained while new rows are added, and the reasons for this are complicated... and one day I'll fix it. Until then, I just hit the pads in order, then assign them after.  (Note: Some pads have multiple hit zones, which will register as different Note IDs, such as centre, rim, or similar, so make sure to add them all!)

If you want to ignore a note, just don't check the Enabled box. Select the key you want for that note, and select a velocity threshold for it. The velocity threshold is just how "hard" a hit needs to be in order to be recognised. I set mine to 20 to avoid ghost hits, but some drum brains can do this with settings if you prefer. In this case, you can leave it at 0.

The "Tag" is just so you know what that entry represents. You can put anything in here and it will be saved.

If for some reason you want to start over, click Reset. If you accidentally click Reset, you'll start over anyway, so ... just pay that some mind.

Once it looks all good, you can click Enabled and launch GH:WT or test it out in Notepad (or whatever).

Finally, "Keypress Duration" is how long the keypress event will fire for each time a pad is hit. In order to hit fast rolls and the like, this should be as low as possible, while still actually registering. I've managed with as low as 37, but the higher your framerate, the lower you can set this value. If it seems like nothing is working or pads rarely register, this number is likely TOO LOW.

## Technical:
This app was developed using a mix of Qt Widgets and Qt Quick. Why? Because I designed it in widgets and then realised halfway through that it would be much simpler to use Quick. Here we are.

Keyboard events are emitted using SendKey with mapped virtual key and virtual scancodes, so it should be compatible with any program which accepts keyboard events.

MIDI is handled by [QMidi](https://github.com/waddlesplash/QMidi), an awesome library written by waddlesplash and is released under the MIT license.

## Known issues:
 * New MIDI events cause Key mappings to get wonky
 * The debug terminal opens (black window) when you open the app.
 * Sometimes the "key off" events don't fire consistently, and if the app is closed in this state, the keyboard state (to Windows) will be in a funky way. Easily fixed by rubbing hand/cat/face across keyboard liberally, restarting the app (and enabling), or restarting Windows.
