#include "mbed.h"
#include "mbed_trace.h"
#include "fuell.hpp"

#define FDC1004_ADRESS 0b1010000 << 1
#define BYTE_TO_BINARY_PATTERN "%c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c\r\n"
#define BYTE_TO_BINARY_PATTERN_32 "%c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c\r\n"

#define BYTE_TO_BINARY(uint16_t)  \
 ((uint16_t) & 0x8000 ? '1' : '0'), \
  ((uint16_t) & 0x4000 ? '1' : '0'), \
  ((uint16_t) & 0x2000 ? '1' : '0'), \
  ((uint16_t) & 0x1000 ? '1' : '0'), \
  ((uint16_t) & 0x800 ? '1' : '0'), \
  ((uint16_t) & 0x400 ? '1' : '0'), \
  ((uint16_t) & 0x200 ? '1' : '0'), \
  ((uint16_t) & 0x100 ? '1' : '0'), \
  ((uint16_t) & 0x80 ? '1' : '0'), \
  ((uint16_t) & 0x40 ? '1' : '0'), \
  ((uint16_t) & 0x20 ? '1' : '0'), \
  ((uint16_t) & 0x10 ? '1' : '0'), \
  ((uint16_t) & 0x08 ? '1' : '0'), \
  ((uint16_t) & 0x04 ? '1' : '0'), \
  ((uint16_t) & 0x02 ? '1' : '0'), \
  ((uint16_t) & 0x01 ? '1' : '0')

#define BYTE_TO_BINARY_32(uint32_t)  \
 ((uint32_t) & 0x800000000 ? '1' : '0'), \
  ((uint32_t) & 0x40000000 ? '1' : '0'), \
  ((uint32_t) & 0x20000000 ? '1' : '0'), \
  ((uint32_t) & 0x10000000 ? '1' : '0'), \
  ((uint32_t) & 0x8000000 ? '1' : '0'), \
  ((uint32_t) & 0x4000000 ? '1' : '0'), \
  ((uint32_t) & 0x2000000 ? '1' : '0'), \
  ((uint32_t) & 0x1000000 ? '1' : '0'), \
  ((uint32_t) & 0x800000 ? '1' : '0'), \
  ((uint32_t) & 0x400000 ? '1' : '0'), \
  ((uint32_t) & 0x200000 ? '1' : '0'), \
  ((uint32_t) & 0x100000 ? '1' : '0'), \
  ((uint32_t) & 0x80000 ? '1' : '0'), \
  ((uint32_t) & 0x40000 ? '1' : '0'), \
  ((uint32_t) & 0x20000 ? '1' : '0'), \
  ((uint32_t) & 0x10000 ? '1' : '0'),\
  ((uint32_t) & 0x8000 ? '1' : '0'), \
  ((uint32_t) & 0x4000 ? '1' : '0'), \
  ((uint32_t) & 0x2000 ? '1' : '0'), \
  ((uint32_t) & 0x1000 ? '1' : '0'), \
  ((uint32_t) & 0x800 ? '1' : '0'), \
  ((uint32_t) & 0x400 ? '1' : '0'), \
  ((uint32_t) & 0x200 ? '1' : '0'), \
  ((uint32_t) & 0x100 ? '1' : '0'), \
  ((uint32_t) & 0x80 ? '1' : '0'), \
  ((uint32_t) & 0x40 ? '1' : '0'), \
  ((uint32_t) & 0x20 ? '1' : '0'), \
  ((uint32_t) & 0x10 ? '1' : '0'), \
  ((uint32_t) & 0x08 ? '1' : '0'), \
  ((uint32_t) & 0x04 ? '1' : '0'), \
  ((uint32_t) & 0x02 ? '1' : '0'), \
  ((uint32_t) & 0x01 ? '1' : '0')


using namespace mbed;

//I2C i2c(D14, D15);  für kabel
I2C i2c(PC_1, PC_0);

DigitalOut led(LED1);

int main(void){


  FDC1004 fdc1004(i2c);

  uint16_t value = fdc1004.get_config_register();
  printf("CONFIG REGISTER YY: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(value)); 

  fdc1004.reset(value);

  value = fdc1004.get_config_register();

  while((value & 0x8000) != 0){

    //wait_us(3000*1000);

  }
  
 //value = fdc1004.get_config_register();




 
  fdc1004.set_repeated_measurements(value, true);

  printf("After reset, only repeated " BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(value));


  fdc1004.enable_measurement_1(value);//Messungen aktivieren
  fdc1004.enable_measurement_2(value);
  fdc1004.enable_measurement_3(value);
  fdc1004.enable_measurement_4(value);

  fdc1004.set_measurement_channel_config(FDC1004::Register::ConfigMeasurementReg1, FDC1004::Channel::CIN1, FDC1004::Channel::DISABLED);
  // uint measureChannel1 = fdc1004.get_measurement_channel_config(FDC1004::Register::ConfigMeasurementReg1);

  uint16_t testausgabe_config1 = fdc1004.get_CONF_MEAS1();
  printf("Configurationsregister zur Messung 1: " BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(testausgabe_config1));

  fdc1004.set_measurement_channel_config(FDC1004::Register::ConfigMeasurementReg2, FDC1004::Channel::CIN2, FDC1004::Channel::DISABLED);


  fdc1004.set_measurement_channel_config(FDC1004::Register::ConfigMeasurementReg3, FDC1004::Channel::CIN3, FDC1004::Channel::DISABLED);
  fdc1004.set_measurement_channel_config(FDC1004::Register::ConfigMeasurementReg4, FDC1004::Channel::CIN4, FDC1004::Channel::DISABLED);
  

  //printf("config register: %04X\r\n", value);
  printf("EWARTET; CONFIG REGISTER 4 5 6 7 gesetzt: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(value));
  
  
  //value = 0;
  //fdc1004.activate_differential_measurements_1(value);
  printf("Config 1 Measurement register  "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(value));                      

  //fdc1004.activate_differential_measurements_2(value);
  //fdc1004.activate_differential_measurements_3(value);
  //fdc1004.activate_differential_measurements_4(value);





  //value = fdc1004.get_config_register();
  
  // printf("CONFIG REGISTER AFTER RESET: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(value));
  // value = 0;
  // value = fdc1004.get_config_register();
  
  // printf("NOCHMAL CONFIG REGISTER AFTER RESET; DAVOR NOCH GETCONFIG: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(value));

  //offset1 = fdc1004.get_CONF_MEAS1();
  //printf("CONF_MEAS1:  "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(offset1));

  printf("config register vor while:  " BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(value));

  uint32_t measure1, measure2, measure3, measure4;

  int count = 0;
  while(1){


    measure1 = fdc1004.getMeasure1();
    measure2 = fdc1004.getMeasure2();
    measure3 = fdc1004.getMeasure3();
    measure4 = fdc1004.getMeasure4();

    printf("Measure1: %fpF\r\n", (measure1/pow(2, 19)));
    printf("Measure2: %fpF\r\n", (measure2/pow(2, 19)));
    printf("Measure3: %fpF\r\n", (measure3/pow(2, 19)));
    printf("Measure4: %fpF\r\n", (measure4/pow(2, 19)));

    printf("Durchlauf: %d\r\n\r\n", count+=1);

    wait_us(4000*1000);

    //averaging
    /*
    uint16_t 
    
    
    
    */
  }


/*
  value = fdc1004.get_MEAS1_MSB();
  printf("MEAS1MSB: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(value)); 

  value = fdc1004.get_MEAS1_LSB();
  printf("MEAS1LSB: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(value)); 

  value = fdc1004.get_MEAS2_MSB();
  printf("MEAS2MSB "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(value)); 

  value = fdc1004.get_CONF_MEAS1();
  printf("0x08 "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(value)); 

  value = fdc1004.get_CONF_MEAS2();
  printf("0x09 "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(value)); 

  value = fdc1004.get_config_register();
  printf("CONFIG REGISTER YY: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(value)); 

  value = fdc1004.get_OFFSET_CAL_CIN4();
  printf("0x10: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(value)); 

  value = fdc1004.get_GAIN_CAL_CIN2();
  printf("0x12 "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(value)); 

  value = fdc1004.get_GAIN_CAL_CIN4();
  printf("0x14 "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(value)); 

 */
}
