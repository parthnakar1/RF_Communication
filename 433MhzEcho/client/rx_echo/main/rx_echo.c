//C Headers
#include <stdio.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"//......................
#include "esp_log.h"//......................
#include "driver/uart.h"//..................
#include "string.h"//.......................
#include "driver/gpio.h"
#include "sdkconfig.h"


static const int RX_BUF_SIZE = 1024;
#define RXD_PIN (GPIO_NUM_5)
#define TXD_PIN (GPIO_NUM_4)

//Initializations
static const char* START = "@@@";
static const char* ACK = "^^^";
static const char* END = "~~~";
//static const string DATA = "HelloWorld";


//UART configurations
void init(void) {
    const uart_config_t uart_config = {
        .baud_rate = 4800,
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

/* receiver */
static void rx_task(void *arg)
{
    //declarations
    int rxBytes = 0, txBytes, len;
    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE+1);
    while (1) {
        //wait for start bits
        while(data!=START){
            free(data);
            rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 1000 / portTICK_RATE_MS);
            printf("waiting for start bits\n");
        }
        free(data);
        printf("start bits received\n");

        //send ack
        len = strlen(ACK);
        txBytes = uart_write_bytes(UART_NUM_1, ACK, len);
        printf("ack send\n");

        //receive data and end 
        rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 1000 / portTICK_RATE_MS);
        printf("Read %d bytes of data %s\n", rxBytes, data);
        free(data);
        rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 1000 / portTICK_RATE_MS);
        printf("Read %d bytes of end %s\n", rxBytes, data);
        free(data);
    }
}


void app_main(void)
{	
	init();
    xTaskCreate(rx_task, "uart_rx_task", 1024*2, NULL, configMAX_PRIORITIES, NULL);
    //xTaskCreate(tx_task, "uart_tx_task", 1024*2, NULL, configMAX_PRIORITIES-1, NULL);

}
