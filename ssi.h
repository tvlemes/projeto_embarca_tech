#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"

// SSI tags - tag length limited to 8 bytes by default
const char * ssi_tags[] = {"ip", "temp", "humidity", "temp_int" };
volatile float temperature_int = 0.0f;
volatile float temperature_ext = 0.0f;
volatile float humidity_ext = 0.0f;
volatile const char *ip; 

// Set a temperatura interna do processador
void set_temperature_int(float temp_internal) {
  temperature_int = temp_internal;
}

// Set a temperatura externa
void set_temperature_ext(float temp_external) {
  temperature_ext = temp_external;
}

// Set a umidade
void set_humidity_ext(float hum) {
  humidity_ext = hum;
}

// Set a umidade
void set_ip(const char *ip_local) {
  ip = ip_local;
}

u16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen) {
  size_t printed;
  switch (iIndex) {
    case 0: // ip
    {
    printed = snprintf(pcInsert, iInsertLen, "%s", ip);
    }
    break;
  case 1: // temperatura externa
    {
      printed = snprintf(pcInsert, iInsertLen, "%.2f", temperature_ext);
    }
    break;
  case 2: // umidity
    {
    printed = snprintf(pcInsert, iInsertLen, "%.2f", humidity_ext);
    }
    break;
  case 3: // temperatura interna do processador
    {
    printed = snprintf(pcInsert, iInsertLen, "%.2f", temperature_int);
    }
    break;
  
  default:
    printed = 0;
    break;
  }

  return (u16_t)printed;
}

// Initialise the SSI handler
void ssi_init() {
  // Initialise ADC (internal pin)
  // adc_init();
  // adc_set_temp_sensor_enabled(true);
  // adc_select_input(4);

  http_set_ssi_handler(ssi_handler, ssi_tags, LWIP_ARRAYSIZE(ssi_tags));
}
