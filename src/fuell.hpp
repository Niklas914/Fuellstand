#ifndef FDC1004_HPP
#define FDC1004_HPP

#include <stdio.h>
#include "InterruptIn.h"
#include "mbed.h"
#include "I2C.h"

class FDC1004
{
    static constexpr uint8_t address = 0b1010000 << 1;

public:
    enum class Channel : uint8_t 
    {
        CIN1 = 0,
        CIN2 = 1,
        CIN3 = 2,
        CIN4 = 3,
        CAPDAC = 4,
        DISABLED = 7,
    };
    
    enum class MeasurementRate : uint8_t
    {
        Rate100 = 1,
        Rate200 = 2,
        Rate400 = 3,
    };
    
    enum class Register : uint8_t
    {
        ConfigMeasurementReg1 = 0x08,
        ConfigMeasurementReg2 = 0x09,
        ConfigMeasurementReg3 = 0x0A,
        ConfigMeasurementReg4 = 0x0B,
    };

public:
    FDC1004(mbed::I2C& i2c);

    uint16_t get_register(char registeraddress);

    uint16_t get_config_register();

    uint16_t get_MEAS1_MSB();
    uint16_t get_MEAS1_LSB();
    uint16_t get_MEAS2_MSB();
    uint16_t get_MEAS2_LSB();
    uint16_t get_MEAS3_MSB();
    uint16_t get_MEAS3_LSB();
    uint16_t get_MEAS4_MSB();
    uint16_t get_MEAS4_LSB();
    uint16_t get_CONF_MEAS1();
    uint16_t get_CONF_MEAS2();
    uint16_t get_CONF_MEAS3();
    uint16_t get_CONF_MEAS4();
    uint16_t get_OFFSET_CAL_CIN1();
    uint16_t get_OFFSET_CAL_CIN2();
    uint16_t get_OFFSET_CAL_CIN3();
    uint16_t get_OFFSET_CAL_CIN4();
    uint16_t get_GAIN_CAL_CIN1();
    uint16_t get_GAIN_CAL_CIN2();
    uint16_t get_GAIN_CAL_CIN3();
    uint16_t get_GAIN_CAL_CIN4();
    uint16_t get_MANUFACTURER_ID();
    uint16_t get_DEVICE_ID();

    
    uint32_t getMeasure1();
    uint32_t getMeasure2();
    uint32_t getMeasure3();
    uint32_t getMeasure4();



    void set_measurement_rate(FDC1004::MeasurementRate rate);

    void reset();
    void measurement_rate(uint16_t& value, uint16_t rate);
    void set_repeated_measurements(bool set);

    void enable_measurement_1();
    void enable_measurement_2();
    void enable_measurement_3();
    void enable_measurement_4();

    void disable_measurement_1();
    void disable_measurement_2();
    void disable_measurement_3();
    void disable_measurement_4();


    void set_register_bit(uint16_t& value, u_int8_t position, uint8_t bit, char registeraddress);

    void set_config_register_bit(uint16_t& value, u_int8_t position, uint8_t bit);


    void set_conf_meas1_bit(uint16_t& value, u_int8_t position, uint8_t bit);
    void set_conf_meas2_bit(uint16_t& value, u_int8_t position, uint8_t bit);
    void set_conf_meas3_bit(uint16_t& value, u_int8_t position, uint8_t bit);
    void set_conf_meas4_bit(uint16_t& value, u_int8_t position, uint8_t bit);

    
    void activate_differential_measurements_1(uint16_t& value);
    void activate_differential_measurements_2(uint16_t& value);
    void activate_differential_measurements_3(uint16_t& value);
    void activate_differential_measurements_4(uint16_t& value);

    bool set_measurement_channel_config(
        
        FDC1004::Register configReg, 
        FDC1004::Channel channelA, 
        FDC1004::Channel channelB,
        uint8_t capdacValue = 0);

    bool set_measurement_offset_capacitance(FDC1004::Register configReg, uint8_t offset);




private:    

    void write_register(FDC1004::Register register, uint16_t& value);//uint16_t value daovor, test 

    mbed::I2C& _i2c;
    
};

#endif