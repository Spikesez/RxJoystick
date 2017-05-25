
#include "RxJoystick.h"

RxJoystick* RxJoystick::channels[RxJoystick::numChannels];

RxJoystick::RxJoystick() {
    for (uint8_t i = 0; i < numChannels; i++) {
        channels[i] = new RxJoystick(i);
    }
}

RxJoystick::RxJoystick(uint8_t ch): channel_(ch) {}

void RxJoystick::setPin(uint8_t pin) {
    if (this->pin > 0) {
        detachInterrupt(this->interrupt);
    }

    this->pin = pin;
    this->interrupt = digitalPinToInterrupt(this->pin);
    this->calibrating = true;
    pinMode(this->pin, INPUT);
    attachInterrupt(this->interrupt, handlers[this->channel_], CHANGE);
}

void RxJoystick::setPin(uint8_t pin, uint32_t minValue, uint32_t midValue, uint32_t maxValue) {
    this->setPin(pin);
    this->setCalibration(minValue, midValue, maxValue);
}

RxJoystick* RxJoystick::channel(uint8_t ch) {
    return channels[ch];
}

void RxJoystick::setCalibration(uint32_t minValue, uint32_t midValue, uint32_t maxValue) {
    this->calibrating = false;
    this->calibration[0] = minValue;
    this->calibration[1] = midValue;
    this->calibration[2] = maxValue;
}

void RxJoystick::handleChange() {
    uint8_t value = digitalRead(this->pin);

    if (value == this->prevValue) {
        return;
    }

    this->prevValue = value;
    uint32_t interruptTime = micros();

    if (value == 1) {
        prevTime = interruptTime;
        return;
    }

    this->pulseWidth = interruptTime - this->prevTime;

    if (this->calibrating) {
        if (this->calibration[0] == 0 || this->pulseWidth < this->calibration[0]) {
            this->calibration[0] = this->pulseWidth;
        }

        if (this->calibration[2] == 0 || this->pulseWidth > this->calibration[2]) {
            this->calibration[2] = this->pulseWidth;
        }
    }

    this->setJoystick();
}

uint32_t RxJoystick::getJoystickValue() {
    uint32_t val = map(this->pulseWidth, this->calibration[0], this->calibration[2], this->minJoystick, this->maxJoystick);
    return constrain(val, this->minJoystick, this->maxJoystick);
}

void RxJoystick::setJoystick() {
    uint32_t jv = this->getJoystickValue();

    switch (this->channel_) {
        case 0:
            Joystick.Y(jv);
            break;
        case 1:
            Joystick.Zrotate(jv);
            break;
        case 2:
            Joystick.Z(jv);
            break;
        case 3:
            Joystick.X(jv);
            break;
        case 4:
            // @todo this channel should map to a button since it's a toggle switch
            Joystick.sliderRight(jv);
            break;
        case 5:
            Joystick.sliderLeft(jv);
            break;
    }
}

uint32_t RxJoystick::getPulseWidth() { return this->pulseWidth; }

void RxJoystick::isr0() { channels[0]->handleChange(); }
void RxJoystick::isr1() { channels[1]->handleChange(); }
void RxJoystick::isr2() { channels[2]->handleChange(); }
void RxJoystick::isr3() { channels[3]->handleChange(); }
void RxJoystick::isr4() { channels[4]->handleChange(); }
void RxJoystick::isr5() { channels[5]->handleChange(); }

