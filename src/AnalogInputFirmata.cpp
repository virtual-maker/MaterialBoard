/*
  AnalogFirmata.h - Firmata library
  Copyright (C) 2006-2008 Hans-Christoph Steiner.  All rights reserved.
  Copyright (C) 2010-2011 Paul Stoffregen.  All rights reserved.
  Copyright (C) 2009 Shigeru Kobayashi.  All rights reserved.
  Copyright (C) 2013 Norbert Truchsess. All rights reserved.
  Copyright (C) 2009-2015 Jeff Hoefs.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.

  Last updated by Jeff Hoefs: November 22nd, 2015
*/

#include <MaterialBoard.h>
#include "AnalogFirmata.h"
#include "AnalogInputFirmata.h"

AnalogInputFirmata *AnalogInputFirmataInstance;

void reportAnalogInputCallback(byte analogPin, int value)
{
  AnalogInputFirmataInstance->reportAnalog(analogPin, value);
}

AnalogInputFirmata::AnalogInputFirmata()
{
  AnalogInputFirmataInstance = this;
  analogInputsToReport = 0;
}

void AnalogInputFirmata::attach()
{
  Firmata.attach(REPORT_ANALOG, reportAnalogInputCallback);
}

// -----------------------------------------------------------------------------
/* sets bits in a bit array (int) to toggle the reporting of the analogIns
 */
//void FirmataClass::setAnalogPinReporting(byte pin, byte state) {
//}
void AnalogInputFirmata::reportAnalog(byte analogPin, int value)
{
  if (analogPin < TOTAL_ANALOG_PINS) {
    if (value == 0) {
      analogInputsToReport = analogInputsToReport & ~ (1 << analogPin);
    } else {
      analogInputsToReport = analogInputsToReport | (1 << analogPin);
      // prevent during system reset or all analog pin values will be reported
      // which may report noise for unconnected analog pins
      if (!Firmata.isResetting()) {
        // Send pin value immediately. This is helpful when connected via
        // ethernet, wi-fi or bluetooth so pin states can be known upon
        // reconnecting.
        Firmata.sendAnalog(analogPin, analogRead(analogPin));
      }
    }
  }
  // TODO: save status to EEPROM here, if changed
}

boolean AnalogInputFirmata::handlePinMode(byte pin, int mode)
{
  if (IS_PIN_ANALOG(pin)) {
    if (mode == PIN_MODE_ANALOG) {
      reportAnalog(PIN_TO_ANALOG(pin), 1); // turn on reporting
      if (IS_PIN_DIGITAL(pin)) {
        pinMode(PIN_TO_DIGITAL(pin), INPUT); // disable output driver
      }
      return true;
    } else {
      reportAnalog(PIN_TO_ANALOG(pin), 0); // turn off reporting
    }
  }
  return false;
}

void AnalogInputFirmata::handleCapability(byte pin)
{
  if (IS_PIN_ANALOG(pin)) {
    Firmata.write(PIN_MODE_ANALOG);
    Firmata.write(10); // 10 = 10-bit resolution
  }
}

boolean AnalogInputFirmata::handleSysex(byte command, byte argc, byte* argv)
{
  return handleAnalogFirmataSysex(command, argc, argv);
}

void AnalogInputFirmata::reset()
{
  // by default, do not report any analog inputs
  analogInputsToReport = 0;
}

void AnalogInputFirmata::report(bool elapsed)
{
  if (!elapsed)
  {
    return;
  }

  byte pin, analogPin;
  /* ANALOGREAD - do all analogReads() at the configured sampling interval */
  for (pin = 0; pin < TOTAL_PINS; pin++) {
    if (IS_PIN_ANALOG(pin) && Firmata.getPinMode(pin) == PIN_MODE_ANALOG) {
      analogPin = PIN_TO_ANALOG(pin);
      if (analogInputsToReport & (1 << analogPin)) {
        Firmata.sendAnalog(analogPin, analogRead(analogPin));
      }
    }
  }
}
