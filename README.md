# Smart Parking Slot Availability Detector

Arduino-based two-slot parking entry/exit prototype using two IR sensors, a servo-operated gate, and a 16x2 I2C LCD.

## Repository files currently present

- [Firmware sketch](firmware/SmartParkingSlotDetector.ino)
- [Wokwi sketch](sketch.ino)
- [Wokwi circuit diagram](diagram.json)
- [Wokwi libraries](libraries.txt)

## Hardware mapping

| Component | Arduino Uno connection |
|---|---|
| Entry IR sensor | D2 |
| Exit IR sensor | D3 |
| Servo signal | D9 |
| I2C LCD SDA | A4 |
| I2C LCD SCL | A5 |
| LCD I2C address | 0x27 |

## Source version status

The latest Arduino IDE sketch and Wokwi project export supplied separately by the project owner are different from each other and have not yet been uploaded to this repository as their exact original contents. The firmware files currently linked above should not be assumed to be identical to those supplied versions.

The supplied Wokwi project export references: https://wokwi.com/projects/476112975545736193

## Libraries

- Servo
- Wire (Arduino core)
- LiquidCrystal_I2C

Before physical deployment, test entry, exit, full-capacity, timeout, and sensor-clear behavior on the actual hardware.
