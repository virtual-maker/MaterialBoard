/*
 * MaterialBoard standard sketch, for use of serial communication with VirtualBoard.
 */

#include <MaterialBoard.h>

#define MB_BAUD_RATE (115200)

// Use these defines to easily enable or disable certain modules

// Note that the SERVO module currently is not supported on ESP32. So either disable this or patch the library
#ifndef ESP32
//#define ENABLE_SERVO
#endif

#define ENABLE_I2C
#define ENABLE_SPI
#define ENABLE_ANALOG
#define ENABLE_DIGITAL

#ifdef ENABLE_DIGITAL
#include <DigitalInputFirmata.h>
DigitalInputFirmata digitalInput;

#include <DigitalOutputFirmata.h>
DigitalOutputFirmata digitalOutput;
#endif

#ifdef ENABLE_ANALOG
#include <AnalogInputFirmata.h>
AnalogInputFirmata analogInput;

#include <AnalogOutputFirmata.h>
AnalogOutputFirmata analogOutput;
#include <AnalogWrite.h>
#endif

#ifdef ENABLE_I2C
#include <Wire.h>
#include <I2CFirmata.h>
I2CFirmata i2c;
#endif

#ifdef ENABLE_SPI
#include <SPI.h>
#include <SpiFirmata.h>
SpiFirmata spi;
#endif

#ifdef ENABLE_SERVO
#include <Servo.h>
#include <ServoFirmata.h>
ServoFirmata servo;
#endif

#include <FirmataExt.h>
FirmataExt firmataExt;

#include <FirmataReporting.h>
FirmataReporting reporting;

void systemResetCallback() {
  for (byte i = 0; i < TOTAL_PINS; i++) {
    if (IS_PIN_ANALOG(i)) {
      Firmata.setPinMode(i, PIN_MODE_ANALOG);
    }
    else if (IS_PIN_DIGITAL(i)) {
      Firmata.setPinMode(i, PIN_MODE_OUTPUT);
    }
  }
  firmataExt.reset();

  Firmata.printFirmwareVersion();
  Firmata.printVersion();
}

void initTransport() {
  // Save a couple of seconds by disabling the startup blink sequence.
  Firmata.disableBlinkVersion();
  Firmata.begin(MB_BAUD_RATE);
}

void initFirmata() {
  // Set firmware name and version. The name is automatically derived from the name of this file.
  // Firmata.setFirmwareVersion(FIRMATA_FIRMWARE_MAJOR_VERSION, FIRMATA_FIRMWARE_MINOR_VERSION);
  // The usage of the above shortcut is not recommended, since it stores the full path of the file name in a
  // string constant, using both flash and ram.
  Firmata.setFirmwareNameAndVersion("MaterialBoard", FIRMATA_FIRMWARE_MAJOR_VERSION, FIRMATA_FIRMWARE_MINOR_VERSION);

#ifdef ENABLE_DIGITAL
  firmataExt.addFeature(digitalInput);
  firmataExt.addFeature(digitalOutput);
#endif

#ifdef ENABLE_ANALOG
  firmataExt.addFeature(analogInput);
  firmataExt.addFeature(analogOutput);
#endif

#ifdef ENABLE_SERVO
  firmataExt.addFeature(servo);
#endif

#ifdef ENABLE_I2C
  firmataExt.addFeature(i2c);
#endif

  firmataExt.addFeature(reporting);

#ifdef ENABLE_SPI
  firmataExt.addFeature(spi);
#endif

  firmataExt.attach();
  Firmata.attach(SYSTEM_RESET, systemResetCallback);
}

void setup() {
  initTransport();
  Firmata.sendString(F("Booting device. Stand by..."));
  initFirmata();
  Firmata.parse(SYSTEM_RESET);
  Firmata.sendString(F("Device booted."));
}

void loop() {
  while (Firmata.available()) {
    Firmata.processInput();
    if (!Firmata.isParsingMessage()) {
      break;
    }
  }
  firmataExt.report(reporting.elapsed());
}
