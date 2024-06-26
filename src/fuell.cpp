#include "fuell.hpp"

#define FDC1004_ADRESS 0b1010000 << 1



using namespace mbed;

FDC1004::FDC1004(mbed::I2C& i2c) :
    _i2c(i2c)
    //_alert(PC_12, PinMode::PullNone),
      
    //interrupt_triggered(true),//true
    //high_limit_reached(false)
{
   
}

uint16_t FDC1004::get_MEAS1_MSB(){
    return get_register(0x00);
}
uint16_t FDC1004::get_MEAS1_LSB(){
    return get_register(0x01);
}
uint16_t FDC1004::get_MEAS2_MSB(){
    return get_register(0x02);
}
uint16_t FDC1004::get_MEAS2_LSB(){
    return get_register(0x03);
}
uint16_t FDC1004::get_MEAS3_MSB(){
    return get_register(0x04);
}
uint16_t FDC1004::get_MEAS3_LSB(){
    return get_register(0x05);
}
uint16_t FDC1004::get_MEAS4_MSB(){
    return get_register(0x06);
}
uint16_t FDC1004::get_MEAS4_LSB(){
    return get_register(0x07);
}
uint16_t FDC1004::get_CONF_MEAS1(){
    return get_register(0x08);
}
uint16_t FDC1004::get_CONF_MEAS2(){
    return get_register(0x09);
}
uint16_t FDC1004::get_CONF_MEAS3(){
    return get_register(0x0A);
}
uint16_t FDC1004::get_CONF_MEAS4(){
    return get_register(0x0B);
}
uint16_t FDC1004::get_config_register(){
    return get_register(0x0C);
}
uint16_t FDC1004::get_OFFSET_CAL_CIN1(){
    return get_register(0x0D);
}
uint16_t FDC1004::get_OFFSET_CAL_CIN2(){
    return get_register(0x0E);
}
uint16_t FDC1004::get_OFFSET_CAL_CIN3(){
    return get_register(0x0F);
}
uint16_t FDC1004::get_OFFSET_CAL_CIN4(){
    return get_register(0x10);
}
uint16_t FDC1004::get_GAIN_CAL_CIN1(){
    return get_register(0x11);
}
uint16_t FDC1004::get_GAIN_CAL_CIN2(){
    return get_register(0x12);
}
uint16_t FDC1004::get_GAIN_CAL_CIN3(){
    return get_register(0x13);
}
uint16_t FDC1004::get_GAIN_CAL_CIN4(){
    return get_register(0x14);
}
uint16_t FDC1004::get_MANUFACTURER_ID(){
    return get_register(0xFE);
}
uint16_t FDC1004::get_DEVICE_ID(){
    return get_register(0xFF);
}

uint16_t FDC1004::get_register(char registeraddress){

    //printf("FDC1004::%s\r\n", __func__);
    uint8_t FDC1004Address = 0b1010000 << 1;
    
    char data[2];
    int rc = _i2c.write(FDC1004Address, &registeraddress, 1, false);//<==true davor
    if (rc != 0){

        printf("Fehler i2c write\r\n");
    }
    rc = _i2c.read(FDC1004Address, data, 2);
    if (rc != 0){

        printf("Fehler i2c read\r\n");
    }

    uint16_t value = data[1];
    value = value | (data[0] << 8);

    return value;
}


uint32_t FDC1004::getMeasure1(){

    uint16_t meas1_1 = get_MEAS1_MSB();
    uint16_t meas1_2 = get_MEAS1_LSB();
    uint32_t complete_measure_binary1 = meas1_2 >> 8;
    complete_measure_binary1 += meas1_1 << 8;
    return complete_measure_binary1;
}

uint32_t FDC1004::getMeasure2(){

    uint16_t meas2_1 = get_MEAS2_MSB();
    uint16_t meas2_2 = get_MEAS2_LSB();
    uint32_t complete_measure_binary2 = meas2_2 >> 8;
    complete_measure_binary2 += meas2_1 << 8;
    return complete_measure_binary2;
}
uint32_t FDC1004::getMeasure3(){

    uint16_t meas3_1 = get_MEAS3_MSB();
    uint16_t meas3_2 = get_MEAS3_LSB();
    uint32_t complete_measure_binary3 = meas3_2 >> 8;
    complete_measure_binary3 += meas3_1 << 8;
    return complete_measure_binary3;
}
uint32_t FDC1004::getMeasure4(){

    uint16_t meas4_1 = get_MEAS4_MSB();
    uint16_t meas4_2 = get_MEAS4_LSB();
    uint32_t complete_measure_binary4 = meas4_2 >> 8;
    complete_measure_binary4 += meas4_1 << 8;
    return complete_measure_binary4;
}


void FDC1004::reset(){

    uint16_t value = 0;
    set_config_register_bit(value, 15, 1);
}

void FDC1004::set_repeated_measurements(bool set){

    uint16_t value = 0;

    if(set){set_config_register_bit(value, 8, 1);}else{set_config_register_bit(value, 8, 0);}

}


/*
bool FDC1004::set_measurement_offset_capacitance(FDC1004::Register measureConfigReg, uint8_t offset){

    uint16_t value = get_register((char)measureConfigReg);
    if(offset > 31 || offset < 0){
        //printf("Offset im falschen Bereich, maximal 31");
        return false;
    }

    //uint16_t capdac = 0;
    value |= (offset << 5);
    write_register(measureConfigReg, value);
    return true;

}

*/

bool FDC1004::set_measurement_channel_config(FDC1004::Register measureConfigReg, FDC1004::Channel channelA, FDC1004::Channel channelB, uint8_t capdacValue)
{
    // uint16_t value = get_register((char)measureConfigReg);
    uint16_t value = 0x00;
    value |= ((uint16_t)channelA << 13);
    value |= ((uint16_t)channelB << 10);

    bool success = true;

    if (channelB == FDC1004::Channel::CAPDAC){

        if(capdacValue > 31){

            //printf("Offset im falschen Bereich, maximal 31");
            return false;
        }
        value |= (capdacValue << 5);
    }

    write_register(measureConfigReg, value);
    return success;   
}

void FDC1004::set_measurement_rate(FDC1004::MeasurementRate rate){


    uint16_t value = get_config_register();

    value &= ~(0x03 << 10);
    value |= ((uint8_t)rate) << 10;

    if(rate == FDC1004::MeasurementRate::Rate100){
        set_config_register_bit(value, 11, 0);
        set_config_register_bit(value, 10, 1);
    } else if(rate == FDC1004::MeasurementRate::Rate200){
        set_config_register_bit(value, 11, 1);
        set_config_register_bit(value, 10, 0);
    } else if(rate == FDC1004::MeasurementRate::Rate400){

        set_config_register_bit(value, 11, 1);
        set_config_register_bit(value, 10, 1);
    }

    write_register(Register::ConfigRegister, value);
}

void FDC1004::write_register(FDC1004::Register reg, uint16_t& reg_value){

    char data[3];
    data[0] = (uint8_t)reg;
    data[1] = (char)(reg_value >> 8);
    data[2] = (char)reg_value; 

    int rc = _i2c.write(address, &data[0], 3);
    if (rc != 0)
    {
        printf("Fehler i2c write");
    }
}
/*
void FDC1004::measurement_rate(uint16_t& value, uint16_t rate){

    if(rate == 100){
        set_config_register_bit(value, 11, 0);
        set_config_register_bit(value, 10, 1);
    } else if(rate == 200){
        set_config_register_bit(value, 11, 1);
        set_config_register_bit(value, 10, 0);
    } else if(rate == 400){

        set_config_register_bit(value, 11, 1);
        set_config_register_bit(value, 10, 1);
    }
}
*/
void FDC1004::enable_measurement_1(){
    uint16_t value = get_config_register();
    set_config_register_bit(value, 7, 1);
}

void FDC1004::enable_measurement_2(){
    uint16_t value = get_config_register();
    set_config_register_bit(value, 6, 1);
}

void FDC1004::enable_measurement_3(){
    uint16_t value = get_config_register();
    set_config_register_bit(value, 5, 1);
}

void FDC1004::enable_measurement_4(){
    uint16_t value = get_config_register();
    set_config_register_bit(value, 4, 1);
}


void FDC1004::disable_measurement_1(){
    uint16_t value = get_config_register();
    set_config_register_bit(value, 7, 0);
}

void FDC1004::disable_measurement_2(){
    uint16_t value = get_config_register();
    set_config_register_bit(value, 6, 0);
}

void FDC1004::disable_measurement_3(){
    uint16_t value = get_config_register();
    set_config_register_bit(value, 5, 0);
}

void FDC1004::disable_measurement_4(){
    uint16_t value = get_config_register();
    set_config_register_bit(value, 4, 0);
}



void FDC1004::activate_differential_measurements_1(uint16_t& value){


    set_conf_meas1_bit(value, 15, 0);
    set_conf_meas1_bit(value, 14, 0); 
    set_conf_meas1_bit(value, 13, 0); 

    set_conf_meas1_bit(value, 12, 1); 
    set_conf_meas1_bit(value, 11, 1); 
    set_conf_meas1_bit(value, 10, 1);  

}

void FDC1004::activate_differential_measurements_2(uint16_t& value){

    set_conf_meas2_bit(value, 15, 0);
    set_conf_meas2_bit(value, 14, 0); 
    set_conf_meas2_bit(value, 13, 1); 

    set_conf_meas2_bit(value, 12, 1); 
    set_conf_meas2_bit(value, 11, 1); 
    set_conf_meas2_bit(value, 10, 1);                             
}

void FDC1004::activate_differential_measurements_3(uint16_t& value){

    set_conf_meas3_bit(value, 15, 0);
    set_conf_meas3_bit(value, 14, 1); 
    set_conf_meas3_bit(value, 13, 0); 

    set_conf_meas3_bit(value, 12, 1); 
    set_conf_meas3_bit(value, 11, 1); 
    set_conf_meas3_bit(value, 10, 1);                             
}

void FDC1004::activate_differential_measurements_4(uint16_t& value){

    set_conf_meas4_bit(value, 15, 0);
    set_conf_meas4_bit(value, 14, 0); 
    set_conf_meas4_bit(value, 13, 0); 

    set_conf_meas4_bit(value, 12, 1); 
    set_conf_meas4_bit(value, 11, 1); 
    set_conf_meas4_bit(value, 10, 1);                             
}

void FDC1004::set_conf_meas1_bit(uint16_t& value, u_int8_t position, uint8_t bit){  

    set_register_bit(value, position, bit, 0x08);
}

void FDC1004::set_conf_meas2_bit(uint16_t& value, u_int8_t position, uint8_t bit){  

    set_register_bit(value, position, bit, 0x09);
}

void FDC1004::set_conf_meas3_bit(uint16_t& value, u_int8_t position, uint8_t bit){  

    set_register_bit(value, position, bit, 0x0A);
}

void FDC1004::set_conf_meas4_bit(uint16_t& value, u_int8_t position, uint8_t bit){  

    set_register_bit(value, position, bit, 0x0B);
}


void FDC1004::set_config_register_bit(uint16_t& value, u_int8_t position, uint8_t bit){

    set_register_bit(value, position, bit, 0x0C);
}


void FDC1004::set_register_bit(uint16_t& value, u_int8_t position, uint8_t bit, char registeraddress){  

    uint8_t FDC1004Address = 0b1010000 << 1;


    if(bit == 1){
        value = value | (1 << position);
    }else if(bit == 0){
        value = value & ~(1 << position);
    }else{
        printf("Ungültige Eingabe, Bit nur auf 0/1 setzen");
    }
    char data[3];
    data[0] = registeraddress;//ConfigurationReg;
    data[1] = (char)(value >> 8);
    data[2] = (char)value; 

    int rc = _i2c.write(FDC1004Address, &data[0], 3);
    if (rc != 0){
        printf("Fehler i2c write");
    }
}




