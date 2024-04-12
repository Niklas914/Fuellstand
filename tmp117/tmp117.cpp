#include "tmp117.hpp"

#define TMP117_ADDRESS 0x48 << 1//0x90

using namespace mbed;

DigitalOut led(LED1);

TMP117::TMP117(mbed::I2C& i2c):
    _i2c(i2c),
    _alert(PC_12, PinMode::PullNone),
    
    
    interrupt_triggered(true),//true
    high_limit_reached(false)
{
    _alert.fall(callback(this, &TMP117::on_fall));//.fall
}


void TMP117::on_fall(){
    led = !led; 
    interrupt_triggered = true;
}



/*
void TMP117::set_averaged_conversions(TMP117::Conversions::avg8){

    uint16_t config_register = get_config_register();

    set_config_register_bit(config_register, );
    set_config_register_bit(config_register, );

    // set bits in 
    // changed_register = set_conifg_register(value, position);

    set_config_register();
}

*/

void TMP117::set_config_register_bit(uint16_t& value, u_int8_t position, uint8_t bit){  

    uint8_t Tmp117Address = 0x90;
    char ConfigurationReg = 0x01;

    if(bit == 1){
        value = value | (1 << position);
    }else if(bit == 0){
        value = value & ~(1 << position);
    }else{
        printf("Ungültige Eingabe, Bit nur auf 0/1 setzen");
    }
    char data[3];
    data[0] = ConfigurationReg;
    data[1] = (char)(value >> 8);
    data[2] = (char)value; 

    int rc = _i2c.write(Tmp117Address, &data[0], 3);
    if (rc != 0){
        printf("Fehler i2c write");
    }
}

void TMP117::set_high_limit_register_bit(uint16_t& value, u_int8_t position, uint8_t bit){  

    uint8_t Tmp117Address = 0x90;
    char ConfigurationReg = 0x02;

    if(bit == 1){
        value = value | (1 << position);
    }else if(bit == 0){
        value = value & ~(1 << position);
    }else{
        printf("Ungültige Eingabe, Bit nur auf 0/1 setzen");
    }
    char data[3];
    data[0] = ConfigurationReg;
    data[1] = (char)(value >> 8);
    data[2] = (char)value; 

    int rc = _i2c.write(Tmp117Address, &data[0], 3);
    if (rc != 0){
        printf("Fehler i2c write");
    }
}

void TMP117::set_max_temperature_bits(uint16_t& value, uint8_t bit1, uint8_t bit2, uint8_t bit3, uint8_t bit4, uint8_t bit5, uint8_t bit6,
uint8_t bit7, uint8_t bit8, uint8_t bit9, uint8_t bit10, uint8_t bit11, uint8_t bit12, uint8_t bit13, uint8_t bit14, uint8_t bit15, uint8_t bit16){

    set_high_limit_register_bit(value, 0, bit1);
    set_high_limit_register_bit(value, 1, bit2);
    set_high_limit_register_bit(value, 2, bit3);
    set_high_limit_register_bit(value, 3, bit4);
    set_high_limit_register_bit(value, 4, bit5);
    set_high_limit_register_bit(value, 5, bit6);
    set_high_limit_register_bit(value, 6, bit7);
    set_high_limit_register_bit(value, 7, bit8);
    set_high_limit_register_bit(value, 8, bit9);
    set_high_limit_register_bit(value, 9, bit10);
    set_high_limit_register_bit(value, 10, bit11);
    set_high_limit_register_bit(value, 11, bit12);
    set_high_limit_register_bit(value, 12, bit13);
    set_high_limit_register_bit(value, 13, bit14);
    set_high_limit_register_bit(value, 14, bit15);
    set_high_limit_register_bit(value, 15, bit16);

}


uint16_t TMP117::get_high_limit(){

    printf("TMP117::%s\r\n", __func__);
    uint8_t Tmp117Address = (0x48 << 1);//0x90
    char ConfigurationReg = 0x02;

    char data[2];
    int rc = _i2c.write(Tmp117Address, &ConfigurationReg, 1, true);
    if (rc != 0){

        printf("Fehler i2c write");
    }
    rc = _i2c.read(Tmp117Address, data, 2);
    if (rc != 0){

        printf("Fehler i2c read");
    }

    uint16_t value = data[1];
    value = value | (data[0] << 8);

    return value;
}















void TMP117::selecet_alert_pin_bit(uint16_t& value, uint8_t bit){

    if(bit != 0 && bit != 1){
        printf("Falscher Aufruf von Select alert pin bit!");
    }
    set_config_register_bit(value, 2, bit);
}


void TMP117::polarity_alert_pin_bit(uint16_t& value, uint8_t bit){

    if(bit != 0 && bit != 1){
        printf("Falscher Aufruf von Polarity alert pin bit!");
    }    
    set_config_register_bit(value, 3, bit);
}

void TMP117::therm_alert_mode(uint16_t& value, uint8_t bit){

    if(bit != 0 && bit != 1){
        printf("Falscher Aufruf von Therme alert mode!");
    }  
    set_config_register_bit(value, 4, bit);
}

void TMP117::set_conversion_cycle(uint16_t& value, uint8_t bit1, uint8_t bit2, uint8_t bit3){

    if((bit1 != 0 && bit1 != 1) || (bit2 != 0 && bit2 != 1) || (bit3 != 0 && bit3 != 1)){

        printf("Falscher Aufruf Conversion cycle! ");
    }
    set_config_register_bit(value, 7, bit1);
    set_config_register_bit(value, 8, bit2);
    set_config_register_bit(value, 9, bit3);
}

void TMP117::checkTemperature(){

    char cmd[2];
    char data[2];
    float temperature = readTemperature(cmd, data, &temperature);

    if (temperature >= 22.0f){

        on_fall();
        printf("TEMPERATUR SCHWELLE ERREICHT!");
    }

}


float TMP117::readTemperature(char cmd[2], char data[2], float* temperature){
    
    while(interrupt_triggered == false) {

    }
    interrupt_triggered = false;
    
    /*
    while(interrupt_triggered == true){

    }
    */

    cmd[0] = 0x00;
    cmd[1] = 0x01; 
        ///cmd[0] = 0x01;
        ///cmd[1] = 0x00;


    if(_i2c.write(TMP117_ADDRESS, &cmd[1], 1) != 0){//&cmd[1], 1 /// cmd, 2

        printf("fehler i2c write");
    }
    wait_us(1000*1000);

        ///cmd[0] = 0x00;

    if(_i2c.write(TMP117_ADDRESS, &cmd[0], 1, true) != 0){//&cmd[0], 1, true /// cmd, 1, true

        printf("Fehler i2c write");
    } 
    if(_i2c.read(TMP117_ADDRESS, data, 2) != 0){

        printf("Fehler i2c read");
    } 

    //TESTS
    //printf("Data[0]: %d\r\n", data[0]);
    //printf("Data[1]: %d\r\n", data[1]);

    uint16_t temp = data[1]; // 0000 0000 1010 1111
    //printf("TEST1:%d\r\n", temp);
    temp = temp | (data[0] << 8); // 1011 0011 1010 1111
    *temperature = temp * 0.0078125f;
    // temperature = ((data[0] << 8) | data[1]) * 0.0078125f;   
    //printf("TEST2:%d\r\n", temp);
    //printf("Temperatur2: %.2f°C\n", *temperature);



    return *temperature;

}

uint16_t TMP117::get_config_register(){

    printf("TMP117::%s\r\n", __func__);
    uint8_t Tmp117Address = (0x48 << 1);//0x90
    char ConfigurationReg = 0x01;

    char data[2];
    int rc = _i2c.write(Tmp117Address, &ConfigurationReg, 1, true);
    if (rc != 0){

        printf("Fehler i2c write");
    }
    rc = _i2c.read(Tmp117Address, data, 2);
    if (rc != 0){

        printf("Fehler i2c read");
    }

    uint16_t value = data[1];
    value = value | (data[0] << 8);

    return value;
}


void TMP117::set_AVG(uint16_t& value, uint8_t conversions){

    if(conversions == 0){

        set_config_register_bit(value, 5, 0);
        set_config_register_bit(value, 6, 0);
    }else if(conversions == 8){

        set_config_register_bit(value, 5, 1);
        set_config_register_bit(value, 6, 0);

    }else if(conversions == 32){

        set_config_register_bit(value, 5, 0);
        set_config_register_bit(value, 6, 1);
    }else if(conversions == 64){

        set_config_register_bit(value, 5, 1);
        set_config_register_bit(value, 6, 1);
    }
}


void TMP117::set_conversion_mode(uint16_t& value, uint8_t mode){//mode von 1bis 4 nur test

    if(mode == 1){

        set_config_register_bit(value, 10, 0);
        set_config_register_bit(value, 11, 0);

    }else if(mode == 2){

        set_config_register_bit(value, 10, 1);
        set_config_register_bit(value, 11, 0);

    }else if(mode == 3){

        set_config_register_bit(value, 10, 0);
        set_config_register_bit(value, 11, 1);

    }else if(mode == 4){

        set_config_register_bit(value, 10, 1);
        set_config_register_bit(value, 11, 1);

    }
}
 
    
//     if (avg == Conversion::avg64)
//     {
//         set_bits(configRegisterValue, (uint8_t)Conversion::avg64); 
//         set_bit(configRegisterValue, (uint8_t)6); 
//     }
//     // setze bits 

//     write()
// }

TMP117::Conversion_mode TMP117::get_mode_conversions(){

    uint16_t configValue = get_config_register();

    if (get_Bit(configValue, 11) == 0 && get_Bit(configValue, 10) == 0){ // if (configValue & (uint8_t)Conversion::avg8 << 5)
        return Conversion_mode::cc;
    }
    if (get_Bit(configValue, 11) == 0 && get_Bit(configValue, 10) == 1){
        return Conversion_mode::sd;
    }
    else if(get_Bit(configValue, 11) == 1 && get_Bit(configValue, 10) == 0){
        return Conversion_mode::bc;
    }
    else if(get_Bit(configValue, 11) == 1 && get_Bit(configValue, 10) == 1){
        return Conversion_mode::os;
    }
}

TMP117::Conversions TMP117::get_averaged_conversions(){


    uint16_t configValue = get_config_register();

    if (get_Bit(configValue, 6) == 0 && get_Bit(configValue, 5) == 0){ // if (configValue & (uint8_t)Conversion::avg8 << 5)
        return Conversions::noavg;
    }
    else if(get_Bit(configValue, 6) == 0 && get_Bit(configValue, 5) == 1){
        return Conversions::avg8;
    }
    else if(get_Bit(configValue, 6) == 1 && get_Bit(configValue, 5) == 0){
        return Conversions::avg32;
    }
    else if(get_Bit(configValue, 6) == 1 && get_Bit(configValue, 5) == 1){
        return Conversions::avg64;
    }
}


bool TMP117::is_bit_set(uint16_t value, uint8_t position){

    return (value & (1 << position));
}

uint8_t TMP117::get_Bit(uint16_t value, uint8_t position){

    uint16_t be = 1 << position;

    uint16_t tar = be & value;

    if(tar != 0){
        return 1;
    }
    return 0;
}   

/*
void TMP117::reset_register()
{
    printf("TMP117::%s\r\n", __func__);
    uint8_t Tmp117Address = (0x48 << 1);//0x90
    char ResetRegister = 0x06;

    char data[2];
    int rc = _i2c.write(Tmp117Address, &ResetRegister, 1);
    if (rc != 0)
    {
        printf("Fehler i2c write");
    }
}
*/

/*
void TMP117::reset_config_register(uint16_t value)
{
    printf("TMP117::%s\r\n", __func__);
    uint8_t Tmp117Address = (0x48 << 1);//0x90
    // char write_register = 0x01;

    char data[3];
    data[0] = 0x01;
    data[1] = (char)(value >> 8); // 1010 1111 0001 1000 --> 0000 0000 1010 1111
    data[2] = (char)value; 

    int rc = _i2c.write(Tmp117Address, &data[0], 3);
    if (rc != 0)
    {
        printf("Fehler i2c write");
    }
}
*/

void TMP117::reset_register(uint16_t adress){

    uint8_t Tmp117Address = 0x90;
    uint16_t reset;

    switch (adress){

        case 0x00:
            reset = 0x8000;
            break;
        case 0x01:
            reset = 0x0220;
            break;
        case 0x02:
            reset = 0x6000;
            break;
        case 0x03:
            reset = 0x8000;
            break;
        case 0x04:
            reset = 0x0000;
            break;
        case 0x07:
            reset = 0x8000;
            break;
        case 0x0F:
            reset = 0x0117;
            break;
    }

    char data[3];
    data[0] = adress;
    data[1] = (char)(reset >> 8);
    data[2] = (char)reset; 

    if(_i2c.write(Tmp117Address, &data[0], 3) != 0){

        printf("Fehler write !");
    }

}