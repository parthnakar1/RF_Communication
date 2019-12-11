//C Headers
#include <stdio.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
// #include "esp_spi_flash.h"
#include "esp_log.h"//......................
#include "driver/uart.h"//..................
#include "string.h"//.......................

//#include "SRA18.h"
//#include "driver/include/driver/gpio.h"

#include "driver/gpio.h"
#include "sdkconfig.h"

static const int RX_BUF_SIZE = 1024;

#define TXD_PIN (GPIO_NUM_4)
#define RXD_PIN (GPIO_NUM_5)


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

int sendData(const char* logName, const char* data)
{
    const int len = strlen(data);
    const int txBytes = uart_write_bytes(UART_NUM_1, data, len);
    ESP_LOGI(logName, "Wrote %d bytes", txBytes);
    return txBytes;
}

static void tx_task(void *arg)
{
    static const char *TX_TASK_TAG = "TX_TASK";
    esp_log_level_set(TX_TASK_TAG, ESP_LOG_INFO);
    while (1) {
        sendData(TX_TASK_TAG, "Hello world");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

/*
// #define RF_TX 27
// #define RF_RX 18
char arr[] = "hello";
void transmit_pulses(void *arg)
{
	while(1){
		// for(int i=0;i<5;i++){
		// 	// printf("Char %d is %d\n", i,arr[i]);
		// 	int var = (int)arr[i];
		// 	bool binary[7] = {0};
		// 	for(int j=6;j>=0;j--){
		// 		binary[j] = var%2;
		// 		var = var/2;
		// 	}
		// 	for(int j=0;j<6;j++){
  //   			gpio_set_level(RF_TX,binary[j]);
  //   			vTaskDelay(10/portTICK_PERIOD_MS);
		// 	}
		// }

		gpio_set_level(RF_TX,1);
		// vTaskDelay(40/portTICK_PERIOD_MS);
		gpio_set_level(RF_TX,0);
		// vTaskDelay(40/portTICK_PERIOD_MS);
	}
	
}
*/

void app_main()
{
	// gpio_set_direction(RF_TX,GPIO_MODE_OUTPUT);
 //    gpio_set_level(RF_TX,0);
	// xTaskCreate(&transmit_pulses,"transmit_pulses",4096,NULL,1,NULL);

	// xTaskCreate(&blink_task,"blink task",4096,NULL,1,NULL);
	
	
	init();
    xTaskCreate(tx_task, "uart_tx_task", 1024*2, NULL, configMAX_PRIORITIES-1, NULL);



}
