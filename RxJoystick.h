
#ifndef RXCHANNEL_h
#define RXCHANNEL_h

#include <Arduino.h>

typedef void (*FnPtr)();
typedef void (usb_joystick_class::*JsPtr)(unsigned int);

class RxJoystick {

    public:

        RxJoystick();
        RxJoystick(uint8_t channel);

        /**
         * Set the arduino pin number (not interrupt number) for the channel and initialize it.
         */
        void setPin(uint8_t pin);

        /**
         * Set the calibration data while setting the pin which will also turn off auto calibration.
         */
        void setPin(uint8_t pin, uint16_t minValue, uint16_t midValue, uint16_t maxValue);

        /**
         * Set calibration data for min/mid/max values on the transmitter joysticks.
         *
         * Once this method is called, auto calibrate will be automatically disabled.
         */
        void setCalibration(uint16_t minValue = 0, uint16_t midValue = 0, uint16_t maxValue = 0);

        /**
         * Get the raw pwm value sent from the receiver.
         */
        uint16_t getPulseWidth();

        /**
         * Convert the PWM value to the joystick value.
         *
         * @todo Fix this to support a midrange value in case the range on TX isn't linear.
         */
        uint16_t getJoystickValue();

        /**
         * Get channel by index
         */
        RxJoystick* channel(uint8_t ch);

    protected:

        /**
         * Total number of channels on the receiver that are connected to interrupt pins on teensy.
         */
        const static uint8_t numChannels = 6;
        
        /**
         * Min value of the Joystick library. Used to scale the pwm value to joystick commands.
         */
        const static uint16_t minJoystick = 0;

        /**
         * Max value of the Joystick library. Used to scale the pwm value to joystick commands.
         */
        const static uint16_t maxJoystick = 1023;

        /**
         * The channel number (0-5)
         *
         * Channel 0 - Throttle
         * Channel 1 - Aileron (Roll)
         * Channel 2 - Elevator (Pitch)
         * Channel 3 - Rudder (Yaw)
         * Channel 4 - Gear (Flight mode)
         * Channel 5 - Aux (Collective Pitch)
         */
        uint8_t channel_;

        /**
         * The digital pin number where this channel is plugged into the Teensy.
         */
        uint8_t pin;

        /**
         * The interrupt number for the pin. This is calculated automatically using `digitalPinToInterrupt(int pin)`
         * from the arduino libraries.
         */
        uint8_t interrupt;

        /**
         * The previous value set from the interrupt handler. Used to compare with the current value.
         * 
         * If `prevValue != digitalRead(pin)` then the value just changed from off to on or on to off.
         */
        volatile uint8_t prevValue;

        /**
         * The previous interrupt time in microseconds. Used to calculate the pulse width.
         */
        volatile uint32_t prevTime;

        /**
         * The current pulse width value.
         */
        volatile uint16_t pulseWidth;

        /**
         * The min, mid, and max values for joystick calibration.
         */
        volatile uint16_t calibration[3];

        /**
         * List of channel instances.
         */
        static RxJoystick* channels[];

        /**
         * Whether or not auto calibration is enabled. By default this is enabled. To disable it, you must
         * set this manually by passing it to `setPin()` or calling `setCalibration()`
         */
        bool calibrating;

        /**
         * Handle the interrupt request.
         */
        void handleChange();

        /**
         * Set the joystick value and pass it to the usb interface.
         */
        void setJoystick();

        /**
         * The rest of this is rather annoying but since `attachInterrupt()` doesn't pass the pin number or the 
         * interrupt number, and c++ won't allow a non static class method to be passed as the function pointer 
         * then I'm not really sure how else to handle this.
         */
        static void isr0(), isr1(), isr2(), isr3(), isr4(), isr5();
        const FnPtr handlers[6] = { isr0, isr1, isr2, isr3, isr4, isr5 };
        JsPtr jsHandlers[6] = { 
            &usb_joystick_class::Y,
            &usb_joystick_class::Zrotate,
            &usb_joystick_class::Z,
            &usb_joystick_class::X,
            &usb_joystick_class::sliderRight,
            &usb_joystick_class::sliderLeft 
        };
};

#endif

