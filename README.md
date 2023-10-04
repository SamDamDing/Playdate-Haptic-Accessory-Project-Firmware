# Playdate Haptic Accessory Project Firmware

This is firmware for the Teensy 4.1.

The Teensy listens to prints over Serial from the Playdate, parses them, and drives the DRV2605L.

# Waveform Sequence Construction

`AT+WAVEFORM=` followed by the slot number and waveform effect id constructs the waveform sequence.

For example,

`AT+WAVEFORM=0,1:1,3:2,5` 

Plays the sequence:

`Effect ID: 1 - Strong Click - 100%` set in `Slot 0`

`Effect ID: 3 - Strong Click - 30%` set in `Slot 1`

`Effect ID: 5 - Sharp Click - 60%` set in `Slot 2`

# Playing the Waveform Sequence
`AT+GO` plays the waveform
