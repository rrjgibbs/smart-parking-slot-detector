#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Smart Parking Slot Detector — two-slot Wokwi prototype
// IR1 (D2): entry-side sensor; IR2 (D3): inner/exit-side sensor.
// IR modules are assumed active LOW. No serial interface is used.

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo gateServo;

const byte IR1_PIN = 2;
const byte IR2_PIN = 3;
const byte SERVO_PIN = 9;

const byte TOTAL_SLOTS = 2;
const int GATE_CLOSED = 115;
const int GATE_OPEN = 160;

const unsigned long SENSOR_CONFIRM_MS = 100;
const unsigned long SENSOR_TIMEOUT_MS = 15000;
const unsigned long GATE_CLOSE_DELAY_MS = 4000;

enum State { IDLE, ENTRY, EXITING, WAIT_CLEAR };
State state = IDLE;

bool slotOccupied[TOTAL_SLOTS] = {false, false};
bool gateOpen = false;
bool entryConfirmed = false;
bool exitConfirmed = false;

struct Sensor {
  byte pin;
  bool stableActive;
  bool ready;
  unsigned long activeSince;
  bool event;
};

Sensor ir1 = {IR1_PIN, false, true, 0, false};
Sensor ir2 = {IR2_PIN, false, true, 0, false};

unsigned long operationStartedAt = 0;
unsigned long gateCloseAt = 0;

byte parkedCount() {
  return (slotOccupied[0] ? 1 : 0) + (slotOccupied[1] ? 1 : 0);
}

void openGate() {
  gateServo.write(GATE_OPEN);
  gateOpen = true;
}

void closeGate() {
  gateServo.write(GATE_CLOSED);
  gateOpen = false;
}

void showMainStatus() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PARKED: ");
  lcd.print(parkedCount());
  lcd.print("/");
  lcd.print(TOTAL_SLOTS);
  lcd.setCursor(0, 1);
  if (parkedCount() == TOTAL_SLOTS) {
    lcd.print("PARKING FULL");
  } else {
    lcd.print("FREE: ");
    lcd.print(TOTAL_SLOTS - parkedCount());
    lcd.print(" READY");
  }
}

void showMessage(const char *line1, const char *line2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

void updateSensor(Sensor &s, unsigned long now) {
  s.event = false;
  const bool active = (digitalRead(s.pin) == LOW);

  if (!active) {
    s.activeSince = 0;
    s.stableActive = false;
    s.ready = true;
    return;
  }

  if (s.activeSince == 0) s.activeSince = now;
  if (!s.stableActive && s.ready &&
      now - s.activeSince >= SENSOR_CONFIRM_MS) {
    s.stableActive = true;
    s.ready = false;
    s.event = true;
  }
}

void resetSensorForNextPass(Sensor &s) {
  // Do not create an event for a sensor that is already blocked.
  s.event = false;
  s.stableActive = (digitalRead(s.pin) == LOW);
  s.ready = !s.stableActive;
  s.activeSince = s.stableActive ? millis() : 0;
}

void bookNextSlot() {
  for (byte i = 0; i < TOTAL_SLOTS; i++) {
    if (!slotOccupied[i]) {
      slotOccupied[i] = true;
      char message[17];
      snprintf(message, sizeof(message), "SLOT %u BOOKED", i + 1);
      showMessage(message, parkedCount() == TOTAL_SLOTS
                            ? "PARKING FULL"
                            : "SPACE RECORDED");
      return;
    }
  }
}

void freeNextSlot() {
  // This prototype tracks count/order, not the physical identity of a bay.
  for (byte i = 0; i < TOTAL_SLOTS; i++) {
    if (slotOccupied[i]) {
      slotOccupied[i] = false;
      char message[17];
      snprintf(message, sizeof(message), "SLOT %u NOW FREE", i + 1);
      showMessage(message, "EXIT RECORDED");
      return;
    }
  }
}

void setup() {
  pinMode(IR1_PIN, INPUT_PULLUP);
  pinMode(IR2_PIN, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();

  gateServo.attach(SERVO_PIN);
  closeGate();

  showMessage("SMART PARKING", "SYSTEM READY");
  delay(1200);
  showMainStatus();
}

void loop() {
  const unsigned long now = millis();
  updateSensor(ir1, now);
  updateSensor(ir2, now);

  switch (state) {
    case IDLE:
      if (ir1.event) {
        if (parkedCount() >= TOTAL_SLOTS) {
          closeGate();
          showMessage("PARKING FULL", "ENTRY DENIED");
          state = WAIT_CLEAR;
        } else {
          openGate();
          operationStartedAt = now;
          gateCloseAt = 0;
          entryConfirmed = false;
          resetSensorForNextPass(ir2);
          showMessage("ENTRY DETECTED", "WAITING IR2");
          state = ENTRY;
        }
      } else if (ir2.event) {
        if (parkedCount() == 0) {
          closeGate();
          showMessage("PARKING EMPTY", "NO CAR INSIDE");
          state = WAIT_CLEAR;
        } else {
          openGate();
          operationStartedAt = now;
          gateCloseAt = 0;
          exitConfirmed = false;
          resetSensorForNextPass(ir1);
          showMessage("EXIT DETECTED", "WAITING IR1");
          state = EXITING;
        }
      }
      break;

    case ENTRY:
      if (ir2.event && !entryConfirmed) {
        entryConfirmed = true;
        bookNextSlot();
        gateCloseAt = millis() + GATE_CLOSE_DELAY_MS;
      }

      if (!entryConfirmed && now - operationStartedAt >= SENSOR_TIMEOUT_MS) {
        closeGate();
        showMessage("SENSOR TIMEOUT", "ENTRY CANCELLED");
        state = WAIT_CLEAR;
      } else if (entryConfirmed && gateOpen &&
                 (long)(now - gateCloseAt) >= 0) {
        closeGate();
      }

      if (entryConfirmed && !gateOpen) state = WAIT_CLEAR;
      break;

    case EXITING:
      if (ir1.event && !exitConfirmed) {
        exitConfirmed = true;
        freeNextSlot();
        gateCloseAt = millis() + GATE_CLOSE_DELAY_MS;
      }

      if (!exitConfirmed && now - operationStartedAt >= SENSOR_TIMEOUT_MS) {
        closeGate();
        showMessage("SENSOR TIMEOUT", "EXIT CANCELLED");
        state = WAIT_CLEAR;
      } else if (exitConfirmed && gateOpen &&
                 (long)(now - gateCloseAt) >= 0) {
        closeGate();
      }

      if (exitConfirmed && !gateOpen) state = WAIT_CLEAR;
      break;

    case WAIT_CLEAR:
      if (digitalRead(IR1_PIN) == HIGH && digitalRead(IR2_PIN) == HIGH) {
        state = IDLE;
        showMainStatus();
      }
      break;
  }

  delay(5);
}
