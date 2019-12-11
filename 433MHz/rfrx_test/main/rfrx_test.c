//C Headers
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"//......................
// #include "esp_spi_flash.h"
#include "esp_log.h"//......................
#include "driver/uart.h"//..................
#include "string.h"//.......................

//#include "SRA18.h"
//#include "driver/include/driver/gpio.h"

#include "driver/gpio.h"
#include "sdkconfig.h"

static const int RX_BUF_SIZE = 1024;
#define RXD_PIN (GPIO_NUM_5)
#define TXD_PIN (GPIO_NUM_4)


//UART configurations
// const int uart_num = UART_NUM_2;
// uart_config_t uart_config = {
//     .baud_rate = 115200,
//     .data_bits = UART_DATA_8_BITS,
//     .parity = UART_PARITY_DISABLE,
//     .stop_bits = UART_STOP_BITS_1,
//     .flow_ctrl = UART_HW_FLOWCTRL_CTS_RTS,
//     .rx_flow_ctrl_thresh = 122,
// };
// Configure UART parameters
// ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));


// #define RF_TX 17
// #define RF_RX 25	
// bool binary[7];
// char arr[] = "hello";

// void calc_char(){
// 	int val = 0;
// 	for(int j=0;j<7;j++){
// 		if(binary[j]==1){
// 			val = val + pow(2,j);
// 		}
// 	}
// 	char c = (val);
// 	printf("%c\n",c);
// }

//UART configurations
void init(void) {
    const uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    // We won't use a buffer for sending data.
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
}

// void receive_pulses(void *arg)
// {
// 	while(1){
// 		// binary[7] = {0};
// 		// for(int i=0;i<7;i++){
// 		// 	binary[i] = gpio_get_level(RF_RX);
// 		// }
// 		// calc_char();
// 		// printf("%c\n",c);

// 		printf("%d \n",gpio_get_level(RF_RX));
//     	// vTaskDelay(40/portTICK_PERIOD_MS);
// 	}
	
// }


static void rx_task(void *arg)
{
    static const char *RX_TASK_TAG = "RX_TASK";
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE+1);
    while (1) {
        const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 1000 / portTICK_RATE_MS);
        if (rxBytes > 0) {
            data[rxBytes] = 0;
            ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%s'", rxBytes, data);
            ESP_LOG_BUFFER_HEXDUMP(RX_TASK_TAG, data, rxBytes, ESP_LOG_INFO);
        }
    }
    free(data);
}


void app_main(void)
{
	// gpio_set_direction(RF_RX,GPIO_MODE_INPUT);
 //    gpio_set_level(RF_RX,0);
	// xTaskCreate(&receive_pulses,"transmit_pulses",4096,NULL,1,NULL);
	// xTaskCreatePinnedToCore(&calc_char,"calc_char",4096,NULL,1,NULL,0);
	// xTaskCreate(&calc_char,"calc task",4096,NULL,1,NULL);

	init();
    xTaskCreate(rx_task, "uart_rx_task", 1024*2, NULL, configMAX_PRIORITIES, NULL);
}
