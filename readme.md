
## RxJoystick

RxJoystick is a simple arduino library that will allow you to use your R/C trasmitter as a wireless joystick
on your computer.

This library was written for a teensy 3.2 but should work on any teensy with 6 or more interrupt capable digital
I/O pins. You will need a receiver that's compatible with and bound to your transmitter. Run the individual channel
outputs from the receiver to any of the interrupt pins on the teensy, and connect the 5v and ground pins from the 
teensy to the 5v and ground rails on the receiver. Set the USB Type in the arduino ide to any joystick type, and flash
your sketch. Your computer should then recognize the controller. No drivers are required.

This was tested with a dx6i and lemon-rx 6 channel receiver but should work with any receiver that outputs a PWM
signal.

### Example Sketch

    #include <RxJoystick.h>

    RxJoystick rxj;

    void setup() {
        // replace pin numbers with the pins you connected.
        rxj.channel(0)->setPin(23);
        rxj.channel(1)->setPin(22);
        rxj.channel(2)->setPin(21);
        rxj.channel(3)->setPin(20);
        rxj.channel(4)->setPin(19);
        rxj.channel(5)->setPin(18);
    }

    void loop() { }

### Manual Calibration

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

    void setup() {
        for (uint8_t i = 0; i < 6; i++) {
            rxj.channel(i)->setPin(pins[i], minValues[i], midValues[i], maxValues[i]);
        }
    }

    void loop() { }


