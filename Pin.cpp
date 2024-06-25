#include "Pin.h"

Pin::Pin(uint pin, bool direction, gpio_slew_rate slewRate, gpio_drive_strength driveStrength) {
    mPin = pin;
    if(gpio_get_function(mPin) != GPIO_FUNC_SIO) {
        gpio_init(mPin);
        gpio_set_dir(mPin, direction);
        gpio_set_slew_rate(mPin, slewRate);
        gpio_set_drive_strength(mPin, driveStrength);
    }
}

Pin::~Pin() {
    gpio_deinit(mPin);
}

void Pin::setPull(bool up, bool down) const {
    gpio_set_pulls(mPin, up, down);
}

bool Pin::pulledUp() const {
    return gpio_is_pulled_up(mPin);
}

bool Pin::pulledDown() const {
    return gpio_is_pulled_down(mPin);
}

void Pin::setSlewRate(gpio_slew_rate rate) const {
    gpio_set_slew_rate(mPin, rate);
}

gpio_slew_rate Pin::slewRate() const {
    return gpio_get_slew_rate(mPin);
}

void Pin::setDriveStrength(gpio_drive_strength strength) const {
    gpio_set_drive_strength(mPin, strength);
}

gpio_drive_strength Pin::driveStrength() const {
    return gpio_get_drive_strength(mPin);
}

void Pin::setHysteresis(bool enabled) const {
    gpio_set_input_hysteresis_enabled(mPin, enabled);
}

bool Pin::hysteresis() const {
    return gpio_is_input_hysteresis_enabled(mPin);
}

void Pin::setDirection(bool direction) const {
    gpio_set_dir(mPin, direction);
}

bool Pin::direction() const {
    return (bool)gpio_get_dir(mPin);
}

void Pin::setGlobalInterruptHandler(gpio_irq_callback_t func) {
    gpio_set_irq_callback(func);
}

void Pin::enableInterrupts(bool enabled, uint32_t mask) const {
    gpio_set_irq_enabled(mPin, mask, enabled);
    if(enabled) irq_set_enabled(IO_IRQ_BANK0, true);
}

uint32_t Pin::pendingInterrupts() const {
    return gpio_get_irq_event_mask(mPin);
}

void Pin::setInputOverride(gpio_override override) const {
    gpio_set_inover(mPin, override);
}

void Pin::setOutputOverride(gpio_override override) const {
    gpio_set_outover(mPin, override);
}

void Pin::setOutputEnabledOverride(gpio_override override) const {
    gpio_set_oeover(mPin, override);
}

void Pin::setIrqOverride(gpio_override override) const {
    gpio_set_irqover(mPin, override);
}

void Pin::setOutputEnable(bool enabled) const {
    if(enabled) hw_clear_bits(&padsbank0_hw->io[mPin], PADS_BANK0_GPIO0_OD_BITS);
    else        hw_set_bits(&padsbank0_hw->io[mPin], PADS_BANK0_GPIO0_OD_BITS);
}

void Pin::setInputEnable(bool enabled) const {
    if(enabled) hw_set_bits(&padsbank0_hw->io[mPin], PADS_BANK0_GPIO0_IE_BITS);
    else        hw_clear_bits(&padsbank0_hw->io[mPin], PADS_BANK0_GPIO0_IE_BITS);
}

gpio_override Pin::inputOverride() const {
    io_rw_32 ctrl = iobank0_hw->io[mPin].ctrl;
    return (gpio_override)((ctrl >> IO_BANK0_GPIO0_CTRL_INOVER_LSB) & 0x3);
}

gpio_override Pin::outputOverride() const {
    io_rw_32 ctrl = iobank0_hw->io[mPin].ctrl;
    return (gpio_override)((ctrl >> IO_BANK0_GPIO0_CTRL_OUTOVER_LSB) & 0x3);
}

gpio_override Pin::outputEnabledOverride() const {
    io_rw_32 ctrl = iobank0_hw->io[mPin].ctrl;
    return (gpio_override)((ctrl >> IO_BANK0_GPIO0_CTRL_OEOVER_LSB) & 0x3);
}

gpio_override Pin::irqOverride() const {
    io_rw_32 ctrl = iobank0_hw->io[mPin].ctrl;
    return (gpio_override)((ctrl >> IO_BANK0_GPIO0_CTRL_IRQOVER_LSB) & 0x3);
}

bool Pin::outputEnabled() const {
    return padsbank0_hw->io[mPin] & PADS_BANK0_GPIO0_OD_BITS == 0;
}

bool Pin::inputEnabled() const {
    return padsbank0_hw->io[mPin] & PADS_BANK0_GPIO0_IE_BITS != 0;
}

bool Pin::rawValue() const {
    return (((iobank0_hw->io[mPin].status & IO_BANK0_GPIO0_STATUS_INFROMPAD_BITS) >> IO_BANK0_GPIO0_STATUS_INFROMPAD_LSB) & 1) == 1;
}

io_ro_32* Pin::pinStatus() const {
    return &iobank0_hw->io[mPin].status;
}

io_rw_32* Pin::pinControl() const {
    return &iobank0_hw->io[mPin].ctrl;
}

Pin::operator bool() const {
    return gpio_get(mPin);
}

Pin::operator int() const {
    return (int)gpio_get(mPin);
}

Pin &Pin::operator=(const bool &state) {
    gpio_put(mPin, state);
    return *this;
}

Pin &Pin::operator=(const int &state) {
    gpio_put(mPin, state != 0);
    return *this;
}
