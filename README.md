# Smart Parking Slot Detector

A sensor-based embedded-systems project exploring how parking-slot occupancy can be detected and communicated to help distinguish available spaces from occupied ones.

> **Project type:** Educational prototype  
> **Focus:** Embedded systems · Sensor interfacing · Occupancy detection · Smart infrastructure

## Objective

Reduce the uncertainty involved in finding an available parking slot by detecting whether a defined space is occupied and presenting that status clearly.

## Intended system behaviour

1. A sensor monitors a parking slot.
2. A controller reads the sensor signal.
3. The reading is interpreted as **occupied** or **available** using calibrated thresholds or logic appropriate to the selected sensor.
4. The slot status is shown through an indicator or monitoring interface, depending on the implementation.
5. The system is tested under different object positions and environmental conditions.

## Hardware and implementation

The exact sensor, controller, wiring, and communication method should be documented after confirming the components used in the physical build. Avoid assuming that a particular sensor or IoT platform is present.

| Subsystem | Role |
|---|---|
| Occupancy sensor | Detects the presence of a vehicle or object |
| Microcontroller | Reads the sensor and determines slot state |
| Status output | Communicates available/occupied state |
| Optional connectivity | Can send status to a dashboard if implemented |

## Validation plan

Record actual results before claiming performance:

- Test the slot in empty and occupied conditions.
- Repeat tests with different vehicle/object positions.
- Check for false occupied and false available readings.
- Measure response time and reliability across repeated trials.
- Document any sensor blind spots and environmental limitations.

## Safety and limitations

This is an educational prototype, not a certified parking-management or vehicle-safety system. Sensor selection and mounting geometry strongly affect reliability. Do not claim real-world accuracy, cloud connectivity, or a deployed dashboard unless those features have been implemented and tested.

## Evidence to add

- Photograph of the assembled prototype
- Wiring diagram and component list
- Firmware source code
- Interface screenshot, if applicable
- Test table showing trials, correct detections, false detections, and response time

## Future improvements

- Improve sensor placement and calibration.
- Add fault handling for disconnected or inconsistent sensors.
- Build a multi-slot status display if required.
- Add wireless reporting and a dashboard only after the core occupancy detection is reliable.

## Author

Revant Raj Jaiswal · [GitHub](https://github.com/rrjgibbs)
