#include <Wire.h>
#include <USBHost_t36.h>  // Include USB Host Library
#include "Adafruit_DRV2605.h"

USBHost myusb;
USBSerial serUSB(myusb);  // Corrected: Pass by reference, not by pointer.
Adafruit_DRV2605 drv;

void setup() {
  Serial.begin(115200);
  drv.begin();
  Serial.println("DRV2605 Ready");
  drv.setMode(DRV2605_MODE_INTTRIG);  // default, internal trigger when sending GO command
  drv.selectLibrary(1);
  myusb.begin();
}

void loop() {
  myusb.Task();
  if (serUSB.available() > 0) {
    String command = serUSB.readStringUntil('\n');
    executeCommand(command);
  }
}

void executeCommand(String command) {
  if (command.startsWith("AT+WAVEFORM=")) {
    String waveforms = command.substring(12);  // get the substring after "AT+WAVEFORM="
    processWaveformSubcommand(waveforms);
  } else if (command.startsWith("AT+GO")) {
    drv.go();
    serUSB.println("Waveform triggered");
  } else {
    serUSB.println("Invalid Command: " + command);
  }
}

void processWaveformSubcommand(String waveforms) {
  int waveformIndex = 0;
  while (waveformIndex < waveforms.length()) {
    int nextWaveformIndex = waveforms.indexOf(':', waveformIndex);  // find the next colon
    if (nextWaveformIndex < 0) nextWaveformIndex = waveforms.length();  // if no more colons, use the end of the string
    
    String waveform = waveforms.substring(waveformIndex, nextWaveformIndex);  // extract the waveform command
    int commaIndex = waveform.indexOf(',');  // find the comma in the waveform command
    if (commaIndex < 0) {
      serUSB.println("Invalid waveform command: " + waveform);
      return;
    }

    int slot = waveform.substring(0, commaIndex).toInt();  // extract the slot number
    int waveformNumber = waveform.substring(commaIndex + 1).toInt();  // extract the waveform number
    if (slot < 0 || slot > 7) {
      serUSB.println("Invalid slot: " + String(slot) + ". Slot should be between 0 and 7.");
      return;
    }

    if (waveformNumber < 1 || waveformNumber > 123) {
      serUSB.println("Invalid waveform: " + String(waveformNumber) + ". Waveform should be between 1 and 123.");
      return;
    }

    drv.setWaveform(slot, waveformNumber);
    serUSB.println("Waveform set at slot " + String(slot) + " to " + String(waveformNumber));

    waveformIndex = nextWaveformIndex + 1;  // move to the next waveform command
  }
}
