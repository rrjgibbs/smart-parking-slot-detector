# Smart Parking Slot Detector

An Arduino-based two-slot parking entry/exit prototype. Two IR sensors detect vehicle movement, a servo operates the barrier, and a 16×2 I²C LCD displays the parking count and gate status.

> **Project status:** Educational prototype. The sketch has been supplied, but it has not been hardware-tested or independently compiled in this repository.

## How it works

1. **Entry detection:** IR sensor 1 detects an approaching vehicle. If fewer than two slots are recorded as occupied, the servo opens the gate.
2. **Entry confirmation:** IR sensor 2 confirms the vehicle has passed through the entry sequence. The sketch marks the first available slot as occupied.
3. **Exit detection:** IR sensor 2 starts an exit sequence when at least one vehicle is recorded inside.
4. **Exit confirmation:** IR sensor 1 confirms the vehicle has passed out. The sketch marks the first occupied slot as free.
5. **LCD status:** The display shows the parked count, free spaces, entry/exit state, full/empty status, and sensor timeout messages.
6. **Timeout and reset:** If the second sensor does not confirm the movement within 15 seconds, the gate closes and the controller waits until both sensors clear.

## Hardware and pin mapping

| Component | Arduino connection | Purpose |
|---|---|---|
| IR sensor 1 | D2 | Entry-side detection |
| IR sensor 2 | D3 | Exit-side detection |
| Servo signal | D9 | Barrier gate |
| I²C LCD (16×2) | SDA/SCL I²C pins | Parking and gate status |
| I²C LCD address | `0x27` | Configured display address |
| IR sensor outputs | Active LOW | Detection is read when the pin is LOW |

The sketch uses `Servo.h`, `Wire.h`, and `LiquidCrystal_I2C.h`. Install compatible libraries in the Arduino IDE before compiling. I²C pins depend on the specific Arduino board; on an Arduino Uno, SDA is A4 and SCL is A5.

## Configuration in the supplied sketch

- Parking capacity: 2 slots
- Gate closed angle: 115°
- Gate open angle: 160°
- Sensor confirmation period: 100 ms
- Second-sensor timeout: 15 seconds
- Gate close delay: 4 seconds

These are the values currently defined in the sketch. Adjust servo angles to suit the mechanical gate and ensure the servo is not forced against its end stops.

## Serial / baud rate

The supplied parking sketch does **not** call `Serial.begin()` and does not communicate with a Processing dashboard. Therefore, no serial baud rate is currently required. If serial diagnostics or a computer dashboard are added later, configure the same baud rate on both Arduino and host software.

## Important implementation limitations

- Slot occupancy is tracked in software and resets when the Arduino restarts. It is not independently measured at each parking bay.
- The code assumes vehicles pass the two sensors in the expected sequence. Incorrect placement or overlapping sensor detections can produce incorrect counts.
- The exit logic frees the first occupied slot, not necessarily the physical bay the vehicle leaves.
- The servo gate and IR-sensor sequence should be tested with a safe, lightweight model before any physical barrier is used.
- This is a prototype, not a certified parking access-control system.

## Validation checklist

- [ ] Compile with the selected Arduino board and installed libraries.
- [ ] Verify both IR sensors read active LOW when triggered.
- [ ] Confirm the LCD address and I²C wiring.
- [ ] Verify gate-open and gate-closed angles without binding.
- [ ] Test entry, exit, full-capacity, empty-lot, timeout, and sensor-clear recovery.
- [ ] Repeat trials and record missed detections, false detections, and response time.

## Author

Revant Raj Jaiswal · [GitHub](https://github.com/rrjgibbs)
