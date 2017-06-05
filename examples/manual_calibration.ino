
#include <RxJoystick.h>

RxJoystick rxj;

const uint16_t minValues[] = { 1000, 1000, 1000, 1000, 1000, 1000 };
const uint16_t midValues[] = { 1500, 1500, 1500, 1500, 1500, 1500 };
const uint16_t maxValues[] = { 2000, 2000, 2000, 2000, 2000, 2000 };

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

