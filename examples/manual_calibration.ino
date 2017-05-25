
#include <RxJoystick.h>

RxJoystick rxj;

const uint32_t minValues[] = { 1007, 1006,  975,  991, 1005, 1481 };
const uint32_t midValues[] = { 1493, 1500, 1466, 1492, 1495, 1490 };
const uint32_t maxValues[] = { 1979, 1993, 1956, 1992, 1984, 1489 };

const uint8_t pins[] = {
    23, // Throttle
    22, // Aileron (Roll)
    21, // Elevator (Pitch)
    20, // Rudder (Yaw)
    19, // Gear (Flight mode)
    18, // Aux (Collective Pitch)
};

/**
 * You can pass your calibration values directly to the `setPin()` method. Doing so will turn
 * off the automatic calibration. Currently the mid point values aren't used. This may be
 * supported in a future update.
 */
void setup() {

    for (uint8_t i = 0; i < 6; i++) {
        rxj.channel(i)->setPin(pins[i], minValues[i], midValues[i], maxValues[i]);
    }
}

void loop() { }

