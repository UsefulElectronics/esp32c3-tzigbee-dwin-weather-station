
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
static void peripheral_handler_task	(void *pvParameters);
static void time_handle_task		(void *pvParameters);
/* FUNCTIONS DECLARATION -----------------------------------------------------*/

static void time_handle_task(void *pvParameters)
{
	static int timeStamp = 0;

	uartHandler_t diplayBuffer;

	struct tm timestruct;

	uint8_t secondsCounter = 0;

	uint8_t secondsToggle = 0;

	while(1)
	{
		if(timeStamp <= prevTime )
		{
			timeStamp = prevTime;

			secondsCounter = 0;
		}

		if(timeStamp > 0)
		{
			memset(&diplayBuffer, 0, sizeof(uartHandler_t));

			++timeStamp;

			time_unixStampToLocalTime(timeStamp, &timestruct);

			timestruct.tm_hour += 3;
		    if(timestruct.tm_hour >= 24)
		    {
		    	timestruct.tm_hour -= 24;
		    }

			sprintf((char*)diplayBuffer.uart_txBuffer, "%02d:%02d",timestruct.tm_hour, timestruct.tm_min);

			if(secondsToggle)
			{
				diplayBuffer.uart_txBuffer[2] = ' ';
			}

			secondsToggle ^= 1;

			diplayBuffer.uart_txPacketSize = dwinMakePacket((char*)diplayBuffer.uart_txBuffer, DWIN_TIME);

			 xQueueSendToBack(uartTx_queue, &diplayBuffer, portMAX_DELAY);

		}



		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}
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

//			ESP_LOGI(TAG, "formed packet size %d", diplayBuffer.uart_txPacketSize);

			xQueueSendToBack(uartTx_queue, (void *)&diplayBuffer, portMAX_DELAY);
//
//			memset(&diplayBuffer, 0, sizeof(uartHandler_t));
//
//			memcpy(diplayBuffer.uart_txBuffer,
//					WeatherParam.timeString,
//					strlen(WeatherParam.timeString));
//
//			diplayBuffer.uart_txPacketSize = dwinMakePacket((char*) diplayBuffer.uart_txBuffer, DWIN_TIME);
//
//			xQueueSendToBack(uartTx_queue, &diplayBuffer, portMAX_DELAY);
////
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
 * @brief Idle task is running on CPU0 while it is free
 */
bool idle_task_callback(void)
{

	return 0;
}


/**
 * @brief
 *
 */
void app_main(void)
{
	esp_err_t ret = nvs_flash_init() ;
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
      ESP_ERROR_CHECK(nvs_flash_erase());

      ret = nvs_flash_init();
    }

	wifiInit_ssdConnect();

	uart_config();

    xTaskCreate(&uart_transmission_task, "UART_TX_task", 2 * 4096, NULL, 3, NULL);

    xTaskCreate(&http_get_task, "http_get_task",2* 4096, NULL, 4, NULL);

    xTaskCreate(&http_json_parser, "json_parser_task",2 * 4096, NULL, 4, NULL);


    xTaskCreate(&peripheral_handler_task, "system_Handler_task",2 * 4096, NULL, 4, NULL);

    xTaskCreate(&time_handle_task, "system_Handler_task",4096, NULL, 4, NULL);


    esp_register_freertos_idle_hook_for_cpu(idle_task_callback, 0);

}
