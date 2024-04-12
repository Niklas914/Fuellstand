#include "mbed.h"
#include "mbed_trace.h"

#include "tmp117.hpp"

#define TRACE_GROUP  "main"


#define TMP117_ADDRESS 0x48 << 1//0x90

#define BYTE_TO_BINARY_PATTERN "%c %c %c %c %c%c %c%c%c %c%c %c %c %c %c %c\r\n"

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

using namespace mbed;

// pc(PA_2, PA_3);
// DigitalOut led(LED1);

I2C i2c(PC_1, PC_0);

int main(void){


    TMP117 tmp117(i2c);

    uint16_t value = tmp117.get_config_register();
    printf("config register: %04X\r\n", value);
    printf("Test (binary): "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(value));

    // tmp117.reset_register();
    //tmp117.reset_config_register(0x0220);
    tmp117.reset_register(0x01); //configuration   

    value = tmp117.get_config_register();
    printf("config register after write: %04X\r\n", value);

    

    int count = 0;
    char cmd[2];
    char data[2];
    

    float temperature;

    //Average conversions Einstellen und TESTEN
    TMP117::Conversions daten1 = tmp117.get_averaged_conversions();
    printf("ConversionAverage_test1: %u\r\n", (uint8_t)daten1);
    tmp117.set_AVG(value, 0);

    //tmp117.set_averaged_conversions(TMP117::Conversions::avg8);

    uint16_t value1 = tmp117.get_config_register();
    printf("CONFIG: %04X\r\n", value1);


    TMP117::Conversions daten2 = tmp117.get_averaged_conversions();
    printf("ConversionAverage_test2: %u\r\n", (uint8_t)daten2);

    //Conversion mode auslesen und testen:
    TMP117::Conversion_mode con_conf = tmp117.get_mode_conversions();
    printf("TEST_AUSLESEN VON CONVERSION MODE: erwartet: 0, tatsächlich: %u\r\n", (uint8_t)con_conf);
    tmp117.set_conversion_mode(value, 1);
    TMP117::Conversion_mode con_conf_after = tmp117.get_mode_conversions();
    printf("TEST_AUSLESEN VON CONVERSION MODE: erwartet: 0, tatsächlich: %u\r\n", (uint8_t)con_conf_after);


    value = tmp117.get_config_register();
    printf("config register after changes: %04X\r\n", value);


    //Interrupt-bit configs
    tmp117.selecet_alert_pin_bit(value, 0);
    tmp117.polarity_alert_pin_bit(value, 0);
    tmp117.therm_alert_mode(value, 0);
    tmp117.set_conversion_cycle(value, 1, 0, 1);
    
    value = tmp117.get_config_register();
    printf("config register after write: %04X\r\n", value);

    printf("Test (binary) final: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(value));

    uint16_t temperature_bits = tmp117.get_high_limit();
    printf("MAXIMALE TEMPERATUR IN BINÄR VOR VERÄNDERN: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(temperature_bits));

    tmp117.set_max_temperature_bits(temperature_bits, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0);
    printf("MAXIMALE TEMPERATUR IN BINÄR NACH VERÄNDERN: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(temperature_bits));


    while(1){
        
        printf("Wait for temp value...\r\n");
        temperature = tmp117.readTemperature(cmd, data, &temperature);
        printf("Temperatur: %.2f°C\n", temperature);

        value = tmp117.get_config_register();
        printf("BINÄRES CONFIG REGISTER IN WHILE: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(value));

        printf("Durchlauf: %d\r\n\r\n", count+=1);
    }
}