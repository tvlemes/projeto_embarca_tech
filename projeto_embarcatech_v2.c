/*
* Sistema desenvolvido para o curso de 
* Sistemas Embarcados da EmbarcaTech.
* Autor: Thiago Vilarinho Lemes 
*/

#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "hardware/adc.h"
#include "pico/cyw43_arch.h"
#include "lwip/netif.h"
#include "lwip/ip_addr.h"
#include "ssi.h"
#include "cgi.h"
#include "lwip/apps/httpd.h"
#include <dht.h>
#include "hardware/pio.h"

    // Parte 1: Definições de Constantes e Variáveis
/********************************************************* */
// Credenciais WIFI
const char WIFI_SSID[] = "xxxxxxxx"; // Nome da rede Wi-Fi
const char WIFI_PASSWORD[] = "xxxxxxxx"; // Senha da rede Wi-Fi

// Definições do Display OLED
#define I2C_PORT i2c1
#define SDA_PIN 14 // Pino GPIO SDA
#define SCL_PIN 15 // Pino GPIO SCL
#define BAUDRATE 400000 // Velocidade Baud Rate do I2C aumentada para 400kHz

// Definições do Sensor de Temperatura e Umidade
#define ADC_TEMPERATURE_INT 4 // Pino GPIO Sensor interno de temperatura
static const uint DATA_PIN = 17; // Pino GPIO Sensor externo de temperatura
static const dht_model_t DHT_MODEL = DHT11; // Setando o tipo de Sensor
float temperature_c;
float humidity;

volatile int menu_item = 0; // Item do menu
/********************************************************* */


    // Parte 2: Prototipação das Funções
/********************************************************* */
// Função para inicializar o display OLED
void ss1306_init(void); 
// Função para atualizar o display OLED
void update_menu_display(int menu_item, float temp, float temp_int, float humidity, const volatile  char *ip); 
// Função para converter o valor do ADC para temperatura interna
float adc_to_temperature_internal(uint16_t adc_value);
// Função para inicializar o servidor HTTP
char http_server(void);
/********************************************************* */
 

int main() {

    // Parte 3: Inicialização dos Periféricos 
/********************************************************* */
    stdio_init_all(); // Inicializa a comunicação serial
    ss1306_init();    // Inicializa o display OLED
    adc_init();       // Inicializa o ADC
    cyw43_arch_init(); // Inicializa o módulo Wi-Fi
    cyw43_arch_enable_sta_mode(); // Habilita o modo estação do Wi-Fi
    dht_t dht; // Instanciando o DHT
    dht_init(&dht, DHT_MODEL, pio0, DATA_PIN, true /* pull_up */);
/********************************************************* */


    // Parte 4: Configuração da GPIO do Sensor de Temperatura Interna
/********************************************************* */
    adc_set_temp_sensor_enabled(true); // Habilita o sensor de temperatura interno
/********************************************************* */


    // Parte 5: Configuração do Display e Servidor HTTP
/********************************************************* */
    // Configurações iniciais do display
    ssd1306_clear(); // Limpa o display
    ssd1306_draw_string(10, 20, "Iniciando...", true); // Exibe a mensagem de inicialização
    ssd1306_update(I2C_PORT); // Atualiza o display

    http_server(); // Inicia o servidor HTTP
/********************************************************* */


    // Parte 6: Variáveis para controle do joystick
/********************************************************* */
    uint16_t last_adc_x = 0, last_adc_y = 0; // Última leitura do joystick
    const uint16_t threshold = 2000, dead_zone = 1000; // Limitar a zona morta
/********************************************************* */

    
    while (1) {

        // Parte 7: Faz a leitura do sensor de temperatura DHT11 e do sensor interno de temperatura do microcontrolador 
/********************************************************* */
        // Atualiza o menu no display
        adc_select_input(ADC_TEMPERATURE_INT); // Ativa a GPIO 4 como ADC
        float temperature_int = adc_to_temperature_internal(adc_read()); // Faz a leitura da Temperatura interna
        update_menu_display(menu_item, temperature_ext, temperature_int, humidity_ext, ip); // Atualiza display
        
        dht_start_measurement(&dht); // Faz a leitura do sensor DHT11
        dht_result_t result = dht_finish_measurement_blocking(&dht, &humidity, &temperature_c); // Passa os valores para as variáveis
/********************************************************* */
    

        // Parte 8: Controle do Menu
/********************************************************* */
        // Leitura do joystick
        adc_select_input(0); uint16_t adc_x = adc_read(); // Leitura do eixo X
        adc_select_input(1); uint16_t adc_y = adc_read(); // Leitura do eixo Y
/********************************************************* */
    

        // Parte 9: Verificação de Movimentos do Joystick
/********************************************************* */
        // Verifica movimento do joystick
        if ((adc_x > threshold + dead_zone && adc_x > last_adc_x + dead_zone) || 
            (adc_x < threshold - dead_zone && adc_x < last_adc_x - dead_zone)) {
            menu_item = (menu_item + 1) % 5; // Ciclo entre 0, 1, 2 e 4
            last_adc_x = adc_x;
        }
/********************************************************* */


        // Parte 10: Atualização da página HTTP	
/********************************************************* */
        // Atualiza o servidor HTTP
        set_temperature_int(temperature_int);
        set_temperature_ext(temperature_c);
        set_humidity_ext(humidity);
/********************************************************* */

        sleep_ms(300); // Delay reduzido para melhorar o tempo de resposta
    }
    return 0;
}

    // Parte 11: Função de inicialização do Display OLED
/********************************************************* */
void ss1306_init(void) {
    i2c_init(I2C_PORT, BAUDRATE); // Inicializa o barramento I2C
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C); // Configura o pino SDA
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C); // Configura o pino SCL
    gpio_pull_up(SDA_PIN); // Habilita o pull-up no pino SDA
    gpio_pull_up(SCL_PIN); // Habilita o pull-up no pino SCL

    ssd1306_init(I2C_PORT); // Inicializa o display OLED
    ssd1306_clear(); // Limpa o display

}
/********************************************************* */


    // Parte 12: Função de Atualização do Display
/********************************************************* */
void update_menu_display(int menu_item, float temp, float temp_int, float humidity, const volatile  char *ip) {
    ssd1306_clear(); // Limpa o display
    char buffer[20]; // Buffer para armazenar as strings
    if (menu_item == 0) {
        ssd1306_draw_string(14, 10, "EmbarcaTech", true); // Exibe o nome do projeto
        ssd1306_draw_string(14, 30, "Thiago", true); // Exibe o nome do autor
        ssd1306_draw_string(35, 40, "Vilarinho", true); 
        ssd1306_draw_string(70, 50, "Lemes", true); 
    } else if (menu_item == 1) {
        snprintf(buffer, sizeof(buffer), "%.2f C", temperature_c); // Converte a temperatura para string
        ssd1306_draw_string(10, 20, "Temperatura", true); 
        ssd1306_draw_string(35, 35, "Externa", true);
        ssd1306_draw_string(36, 50, buffer, true); 
    } else if (menu_item == 2) {
        snprintf(buffer, sizeof(buffer), "%.2f%%", humidity); // Converte a umidade do ar para string
        ssd1306_draw_string(0, 20, "Umidade do Ar", true);
        ssd1306_draw_string(38, 40, buffer, true);
    }
    else if (menu_item == 3) {
        snprintf(buffer, sizeof(buffer), "%.2f C", temperature_int); // Converte a temperatura interna para string
        ssd1306_draw_string(10, 20, "Temperatura", true);
        ssd1306_draw_string(35, 35, "Interna", true);
        ssd1306_draw_string(36, 50, buffer, true);
    }
    else if (menu_item == 4) {
        // snprintf(buffer, sizeof(buffer), "", ip); // Converte a temperatura interna para string
        ssd1306_draw_string(60, 20, "IP", true);
        ssd1306_draw_string(5, 35, ip, true);
    }
    ssd1306_update(I2C_PORT); // Atualiza o display
}
/********************************************************* */


    // Parte 13: Função de Conversão de ADC da Temperatura Interna
/********************************************************* */
float adc_to_temperature_internal(uint16_t adc_value) { 
    const float conversion_factor = 3.3f / (1 << 12); 
    float voltage = adc_value * conversion_factor; 
    return 27.0f - (voltage - 0.706f) / 0.001721f;
}
/********************************************************* */


    // Parte 14: Função de Inicialização do Servidor HTTP
/********************************************************* */
char http_server(void) {
    printf("Connecting to Wi-Fi...\n");
    // Aguarda a conexão Wi-Fi
    while (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 5000) != 0) {
        printf("Aguardando conexão Wi-Fi...\n");
    }

    printf("Conectado ao Wi-Fi!\n");
    httpd_init();
    printf("Servidor HTTP inicializado.\n");

    // Obtém o endereço IP da interface de rede padrão
    const ip4_addr_t *ip = netif_ip4_addr(netif_default); // Obtém o endereço IP
    printf("Endereço IP: %s\n", ip4addr_ntoa(ip));
    set_ip(ip4addr_ntoa(ip)); // Armazena o endereço IP

    ssi_init();
    printf("SSI Handler inicializado.\n");
    cgi_init();
    printf("CGI Handler inicializado.\n");
}
/********************************************************* */