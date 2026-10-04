# Smart Parking Slot Availability Detector

Arduino-based two-slot parking entry/exit prototype using two IR sensors, a servo-operated gate, and a 16x2 I2C LCD.

## Source files

- [Existing firmware sketch](firmware/SmartParkingSlotDetector.ino)
- [Wokwi sketch](sketch.ino)
- [Wokwi circuit diagram](diagram.json)
- [Wokwi libraries](libraries.txt)
- [Original Wokwi simulation](https://wokwi.com/projects/476112975545736193)

## Hardware mapping

| Component | Arduino Uno connection |
|---|---|
| Entry IR sensor | D2 |
| Exit IR sensor | D3 |
| Servo signal | D9 |
| I2C LCD SDA | A4 |
| I2C LCD SCL | A5 |
| LCD I2C address | 0x27 |

## Version note

The Arduino IDE sketch supplied by the project owner and the sketch exported from Wokwi are different versions. Keep them as separate source files unless they are deliberately reconciled and retested. Do not assume the two implementations are identical.

## Libraries

- Servo
- Wire (Arduino core)
- LiquidCrystal_I2C (Wokwi library name: LiquidCrystal I2C)

## Upload and test

1. Open the IDE firmware sketch in Arduino IDE.
2. Install the Servo and LiquidCrystal_I2C libraries if they are not already installed.
3. Select Arduino Uno and the correct serial port, then compile and upload.
4. Open the Wokwi project or load `sketch.ino`, `diagram.json`, and `libraries.txt` together to simulate the circuit.
5. Test entry, exit, full-capacity, timeout, and sensor-clear behavior before using physical hardware.
