
#include <RxJoystick.h>

RxJoystick rxj;

const uint8_t pins[] = {
    23, // Throttle
    22, // Aileron (Roll)
    21, // Elevator (Pitch)
    20, // Rudder (Yaw)
    19, // Gear (Flight mode)
    18, // Aux (Collective Pitch)
};

/**
 * Without setting the calibration data manually, calibration will continuously run.
 * After turning on transmitter, you should move all channels to their minimum and maximum
 * for accurate calibration values.
 */
void setup() {

    for (uint8_t i = 0; i < 6; i++) {
        rxj.channel(i)->setPin(pins[i]);
    }
}

void loop() { }

