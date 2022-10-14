
/**
 ******************************************************************************
 * @Channel Link    :  https://www.youtube.com/user/wardzx1
 * @file    		:  main.c
 * @author  		:  Ward Almasarani - Useful Electronics
 * @version 		:  v.1.0
 * @date    		:  Oct 8, 2022
 * @brief   		:
 *
 ******************************************************************************/







/* INCLUDES ------------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/
#include "main.h"
/* ENUMORATIONS --------------------------------------------------------------*/

/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/
static const char *TAG = "main";
/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/
static void peripheral_handler_task(void *pvParameters);
/* FUNCTIONS DECLARATION -----------------------------------------------------*/
/**
 * @brief
 *
 * @param pvParameters
 */
static void peripheral_handler_task(void *pvParameters)
{
	apiWeather_t WeatherParam;


	uartHandler_t diplayBuffer;

	memset(&diplayBuffer, 0, sizeof(uartHandler_t));
	while(1)
	{
		if(xQueueReceive(apiWeather_queue, (void *)&WeatherParam, (portTickType)portMAX_DELAY))
		{
			memcpy(diplayBuffer.uart_txBuffer,
					WeatherParam.weatherDesc,
					strlen(WeatherParam.weatherDesc));

			diplayBuffer.uart_txPacketSize = dwinMakePacket((char*) diplayBuffer.uart_txBuffer, DWIN_WEATHER_TEXT);

			xQueueSendToBack(uartTx_queue, &diplayBuffer, portMAX_DELAY);

			ESP_LOGI(TAG, "place data in uart TX buffer ");

			memset(&diplayBuffer, 0, sizeof(uartHandler_t));

			memcpy(diplayBuffer.uart_txBuffer,
					WeatherParam.timeString,
					strlen(WeatherParam.timeString));

			diplayBuffer.uart_txPacketSize = dwinMakePacket((char*) diplayBuffer.uart_txBuffer, DWIN_TIME);

			xQueueSendToBack(uartTx_queue, &diplayBuffer, portMAX_DELAY);

			memset(&diplayBuffer, 0, sizeof(uartHandler_t));

			memcpy(diplayBuffer.uart_txBuffer,
					WeatherParam.dateString,
					strlen(WeatherParam.dateString));

			diplayBuffer.uart_txPacketSize = dwinMakePacket((char*) diplayBuffer.uart_txBuffer, DWIN_DATE);

			xQueueSendToBack(uartTx_queue, &diplayBuffer, portMAX_DELAY);

			memset(&diplayBuffer, 0, sizeof(uartHandler_t));

			memcpy(diplayBuffer.uart_txBuffer,
					WeatherParam.tempreture, DWIN_TEMRETURE_SIZE);

			diplayBuffer.uart_txPacketSize = dwinMakePacket((char*) diplayBuffer.uart_txBuffer, DWIN_TEMPRETURE);

			xQueueSendToBack(uartTx_queue, &diplayBuffer, portMAX_DELAY);
		}
	}
}


/**
 * @brief
 *
 */
void app_main(void)
{
	ESP_ERROR_CHECK(nvs_flash_init() );

	wifiInit_ssdConnect();

    xTaskCreate(&http_get_task, "http_get_task", 4096, NULL, 5, NULL);
//
    xTaskCreate(&http_json_parser, "json_parser_task", 4096, NULL, 5, NULL);

    xTaskCreate(&peripheral_handler_task, "system_Handler_task", 4096, NULL, 5, NULL);

}
