
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

/**
 * @brief 	This task handles the time parameter and keeps track of it even if
 * 			the open weather API does not update it. Every one second the time packet is
 * 			sent to the display. Second  is done here as well.
 *
 * @param pvParameters
 */
static void time_handle_task(void *pvParameters)
{
	static int timeStamp = 0;

	uartHandler_t diplayBuffer;

	struct tm timestruct;

	uint8_t secondsToggle = 0;

	while(1)
	{

		if(SYS_TICK() - hHttpResponse.availableTimeout >= 10000 && hHttpResponse.available == false)
		{
			xSemaphoreGive(httpSem);
//			xSemaphoreGiveRecursive(httpSem);

			hHttpResponse.available = true;

			hHttpResponse.availableTimeout = SYS_TICK();
		}

		if(timeStamp <= prevTime )
		{
			timeStamp = prevTime;
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
 * @brief	In this task the data obtained from API JSO parsing is handled to form DWIN display commands
 * 			and passed to the UART TX Queue
 */
static void peripheral_handler_task(void *pvParameters)
{
	apiManager_h WeatherParam;


	uartHandler_t diplayBuffer;

	memset(&diplayBuffer, 0, sizeof(uartHandler_t));
	while(1)
	{
		if(xQueueReceive(apiWeather_queue, (void *)&WeatherParam, (portTickType)portMAX_DELAY))
		{
			memcpy(diplayBuffer.uart_txBuffer, DWIN_STIRNG_CLEAR, DWIN_STIRNG_CLEAR_SIZE);

			diplayBuffer.uart_txPacketSize = DWIN_STIRNG_CLEAR_SIZE;

			xQueueSendToBack(uartTx_queue, (void *)&diplayBuffer, portMAX_DELAY);

			memcpy(diplayBuffer.uart_txBuffer,
					WeatherParam.weatherDesc,
					strlen(WeatherParam.weatherDesc));


			diplayBuffer.uart_txPacketSize = dwinMakePacket((char*) diplayBuffer.uart_txBuffer, DWIN_WEATHER_TEXT);

			xQueueSendToBack(uartTx_queue, (void *)&diplayBuffer, portMAX_DELAY);

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

			memset(&diplayBuffer, 0, sizeof(uartHandler_t));

			memcpy(diplayBuffer.uart_txBuffer,
					WeatherParam.weatherIcon, DWIN_ICON_SIZE);

			diplayBuffer.uart_txPacketSize = dwinMakePacket((char*) diplayBuffer.uart_txBuffer, DWIN_ICON);

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
 * @brief 	System initialization and FreeRTOS task creation is done here
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

	wifiInit_ssidConnect();
	//Wait for WiFi Initialization and connection to be completed.
	vTaskDelay(50/portTICK_PERIOD_MS);

	uart_config();

    xTaskCreate(&uart_transmission_task, "UART_TX_task", 1 * 4096, NULL, 1, NULL);

    xTaskCreate(&http_get_weather_task, "http_get_task",1* 4096, NULL, 1, NULL);

    xTaskCreate(&http_get_bitcoin_task, "http_get_task",1* 4096, NULL, 1, NULL);

    xTaskCreate(&http_get_ethereum_task, "http_get_task",1* 4096, NULL, 1, NULL);

    xTaskCreate(&http_get_prayer_task, "http_get_task",1* 4096, NULL, 1, NULL);

    xTaskCreate(&http_json_parser, "json_parser_task",3 * 4096, NULL, 1, NULL);

    xTaskCreate(&peripheral_handler_task, "system_Handler_task",1 * 4096, NULL, 1, NULL);

    xTaskCreate(&time_handle_task, "system_Handler_task",4096, NULL, 1, NULL);


    esp_register_freertos_idle_hook_for_cpu(idle_task_callback, 0);

}
