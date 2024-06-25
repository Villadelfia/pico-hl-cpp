#pragma once
#include "pico/stdlib.h"

class Pin {
    uint mPin {};

public:
    Pin(uint pin = 0, bool direction = GPIO_OUT, gpio_slew_rate slewRate = GPIO_SLEW_RATE_FAST, gpio_drive_strength driveStrength = GPIO_DRIVE_STRENGTH_12MA);
    virtual ~Pin();

    // Enable/disable pulls.
    void setPull(bool up, bool down) const;
    bool pulledUp() const;
    bool pulledDown() const;

    // Slew rate, drive strength, and hysteresis.
    void setSlewRate(gpio_slew_rate rate) const;
    gpio_slew_rate slewRate() const;
    void setDriveStrength(gpio_drive_strength strength) const;
    gpio_drive_strength driveStrength() const;
    void setHysteresis(bool enabled) const;
    bool hysteresis() const;

    // Direction. Use GPIO_IN and GPIO_OUT for easy aliases for the bool.
    void setDirection(bool direction) const;
    bool direction() const;

    // IRQ.
    static void setGlobalInterruptHandler(gpio_irq_callback_t func);
    void enableInterrupts(bool enabled, uint32_t mask = 0) const;
    uint32_t pendingInterrupts() const;

    // Override or invert IO on the pin. These operate on a hardware level and it is unlikely that you will need these.
    void setInputOverride(gpio_override override) const;
    void setOutputOverride(gpio_override override) const;
    void setOutputEnabledOverride(gpio_override override) const;
    void setIrqOverride(gpio_override override) const;
    void setOutputEnable(bool enabled) const;
    void setInputEnable(bool enabled) const;
    gpio_override inputOverride() const;
    gpio_override outputOverride() const;
    gpio_override outputEnabledOverride() const;
    gpio_override irqOverride() const;
    bool outputEnabled() const;
    bool inputEnabled() const;

    // Low-level IO access.
    bool rawValue() const;
    io_ro_32* pinStatus() const;
    io_rw_32* pinControl() const;

    // Convenience operator overloads to treat pins as if they were booleans.
    operator bool() const;
    operator int() const;
    Pin& operator=(const bool& state);
    Pin& operator=(const int& state);
};