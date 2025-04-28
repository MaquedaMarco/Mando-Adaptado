#include <HID-Project.h>  // HID library for controller emulation

// Define button pins and corresponding logical button numbers
const int buttonPins[] = {
    0,  // A Button
    1,  // B Button
    2,  // X Button
    3,  // Y Button
    4,  // Left Bumper (LB)
    5,  // Right Bumper (RB)
    6,  // Back Button
    7,  // Start Button
    8,  // Left Stick Click (L3)
    9,  // Right Stick Click (R3)
    10, // D-Pad Up
    11, // D-Pad Down
    12, // D-Pad Left
    13, // D-Pad Right
    14, // Left Trigger (LT)
    15  // Right Trigger (RT)
};

void setup() {
    Serial.begin(115200);
    Gamepad.begin();
    initializePins();
}

void loop() {
    readInputs();
    readAnalogSticks();
    Gamepad.write();
    delay(10);
}

void initializePins() {
    for (int i = 0; i < sizeof(buttonPins) / sizeof(buttonPins[0]); i++) {
        pinMode(buttonPins[i], INPUT_PULLUP);
    }
}

void readInputs() {
    int numButtons = sizeof(buttonPins) / sizeof(buttonPins[0]);
    for (int i = 0; i < numButtons; i++) {
        if (digitalRead(buttonPins[i]) == LOW) {
            Gamepad.press(i + 1); // Logical button numbers start from 1
        } else {
            Gamepad.release(i + 1);
        }
    }
}

void readAnalogSticks() {
    int lx = analogRead(A0); // Left stick X
    int ly = analogRead(A1); // Left stick Y
    int rx = analogRead(A2); // Right stick X
    int ry = analogRead(A3); // Right stick Y

    // Apply deadzone
    lx = applyDeadzone(lx);
    ly = applyDeadzone(ly);
    rx = applyDeadzone(rx);
    ry = applyDeadzone(ry);

    // Map 0-1023 to -32768 to 32767 (16-bit signed range)
    lx = map(lx, 0, 1023, -32768, 32767);
    ly = map(ly, 0, 1023, -32768, 32767);
    rx = map(rx, 0, 1023, -32768, 32767);
    ry = map(ry, 0, 1023, -32768, 32767);

    Gamepad.xAxis(lx);
    Gamepad.yAxis(ly);
    Gamepad.rxAxis(rx);
    Gamepad.ryAxis(ry);
}

int applyDeadzone(int value) {
    const int center = 512;
    const int deadzone = 32; // Small deadzone
    if (value > center - deadzone && value < center + deadzone) {
        return center;
    }
    return value;
}
