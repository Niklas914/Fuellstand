#ifndef TMP117_HPP
#define TMP117_HPP

#include <stdio.h>
#include "InterruptIn.h"
#include "mbed.h"
#include "I2C.h"

class TMP117
{

public:
    TMP117(mbed::I2C& i2c);

    void on_fall();

    void selecet_alert_pin_bit(uint16_t& value, uint8_t bit);

    void polarity_alert_pin_bit(uint16_t& value, uint8_t bit);

    void therm_alert_mode(uint16_t& value, uint8_t bit);

    void conversion_mode(uint16_t& value);

    void set_config_register_bit(uint16_t& value, u_int8_t position, uint8_t bit);

    void set_high_limit_register_bit(uint16_t& value, u_int8_t position, uint8_t bit);

    enum class Conversions: uint8_t{

        noavg = 0, 
        avg8 = 1,
        avg32 = 2,
        avg64 = 3,
    };

    enum class Conversion_mode: uint8_t{

        cc = 0,
        sd = 1,
        bc = 2,
        os = 3,
    };

    uint16_t get_config_register();

    uint16_t get_high_limit();

    void set_max_temperature_bits(uint16_t& value, uint8_t bit1, uint8_t bit2, uint8_t bit3, uint8_t bit4, uint8_t bit5, uint8_t bit6,
    uint8_t bit7, uint8_t bit8, uint8_t bit9, uint8_t bit10, uint8_t bit11, uint8_t bit12, uint8_t bit13, uint8_t bit14, uint8_t bit15, uint8_t bit16);

    void set_AVG(uint16_t& value, uint8_t conversions);

    void set_conversion_mode(uint16_t& value, uint8_t mode);

    void set_conversion_cycle(uint16_t& value, uint8_t bit1, uint8_t bit2, uint8_t bit3);

    //uint8_t get_AVG(uint16_t value);

    TMP117::Conversions get_averaged_conversions();

    TMP117::Conversion_mode get_mode_conversions();

    uint8_t get_Bit(uint16_t value, uint8_t position);

    bool is_bit_set(uint16_t value, uint8_t position);

    float readTemperature(char cmd[2], char data[2], float* temperature);

    void reset_register();

    void reset_config_register(uint16_t value);

    void reset_register(uint16_t address);

    void checkTemperature();

private:

    // static constexpr uint8_t Tmp117Address = (0x48 << 1);//0x90
    // static constexpr char ConfigurationReg = 0x01;

    mbed::I2C& _i2c;
    mbed::InterruptIn _alert;
    volatile bool interrupt_triggered;
    volatile bool high_limit_reached;
};

#endif