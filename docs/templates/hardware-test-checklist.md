# Hardware Testing Checklist - VAWT Control System

**Project:** ___________________________  
**Tester:** ____________________________  
**Date:** ______________________________  
**Hardware Revision:** _________________

## Pre-Assembly Checks

- [ ] **Visual inspection of PCB**
  - No shorts between traces
  - No damaged components
  - Silkscreen legible
  
- [ ] **Component verification**
  - All ICs oriented correctly (pin 1 marker)
  - Electrolytic capacitors polarity correct
  - MOSFETs match datasheet (IRFP260N or equivalent)
  - Fuses rated correctly (10A fast-blow for 48V bus)

## Power-On Tests (No Load)

- [ ] **Supply voltage test**
  - Input: _____ VDC (expected: 48V ± 2V)
  - 5V regulator output: _____ V (expected: 5.0 ± 0.1V)
  - 3.3V regulator output: _____ V (expected: 3.3 ± 0.05V)
  
- [ ] **Current draw idle**
  - ESP32 + sensors: _____ mA (expected: <150 mA)
  - No smoke, no excessive heat

## Sensor Calibration

- [ ] **Anemometer**
  - Zero wind: _____ V (expected: 0.4-0.5V offset)
  - 10 m/s reference: _____ V (expected: ~2.0V)
  - Calibration constant: _____ (m/s)/V
  
- [ ] **Hall effect RPM sensor**
  - Manual rotation test: pulses detected ✓ / ✗
  - Bounce filtering working: ✓ / ✗
  
- [ ] **INA226 voltage/current sensor**
  - Bus voltage reading: _____ V (vs multimeter: _____ V)
  - Current reading (known load): _____ A (vs clamp meter: _____ A)

## PWM and Control Tests

- [ ] **DC-DC converter PWM**
  - Frequency: _____ Hz (expected: 20 kHz)
  - Duty cycle range: _____ % to _____ % (expected: 0-90%)
  - Oscilloscope waveform clean: ✓ / ✗
  
- [ ] **Dump load PWM**
  - 0% duty: no current to dump resistor ✓ / ✗
  - 100% duty: full current _____ A (expected: ~20A @ 48V)
  - Heatsink temperature after 5 min: _____ °C (max 60°C)

## Protection Circuit Tests

- [ ] **Overvoltage test**
  - Inject 65V: crowbar triggers ✓ / ✗
  - Voltage clamped below: _____ V (expected: <60V)
  
- [ ] **Overcurrent test**
  - Simulate short: fuse blows ✓ / ✗
  - ESP32 survives: ✓ / ✗
  
- [ ] **Mechanical brake test**
  - Relay clicks when commanded: ✓ / ✗
  - Brake engages physically: ✓ / ✗
  - Spring-return on power loss: ✓ / ✗

## Communication Tests

- [ ] **SD card logging**
  - Card detected: ✓ / ✗
  - CSV file created: ✓ / ✗
  - Data integrity after 1 hour: ✓ / ✗
  
- [ ] **WiFi connectivity**
  - Connects to SSID: ✓ / ✗
  - NTP sync successful: ✓ / ✗
  - MQTT publish working: ✓ / ✗

## Environmental Tests

- [ ] **Humidity test (85% RH, 24 hours)**
  - No condensation in enclosure: ✓ / ✗
  - ESP32 boots normally: ✓ / ✗
  
- [ ] **Temperature test**
  - Cold: _____ °C (expected: 0-5°C operation)
  - Hot: _____ °C (expected: 45-50°C operation)

## Integration Test (Bench)

- [ ] **Variable load test**
  - Connect to resistive load bank
  - Manually vary load from 0-100 Ω
  - MPPT responds correctly: ✓ / ✗
  - Duty cycle increases with load: ✓ / ✗
  
- [ ] **State machine test**
  - IDLE → STANDBY transition: ✓ / ✗
  - STANDBY → MPPT (when wind > 3 m/s): ✓ / ✗
  - MPPT → STALL (when wind > 12 m/s): ✓ / ✗
  - FAULT on overspeed: ✓ / ✗

## Final Approval

- [ ] All tests passed
- [ ] Firmware version logged: __________
- [ ] Hardware revision logged: __________
- [ ] Ready for field deployment

**Tester Signature:** _________________  
**Date:** ____________________________

**Notes/Issues:**
_______________________________________________________________
_______________________________________________________________
_______________________________________________________________
