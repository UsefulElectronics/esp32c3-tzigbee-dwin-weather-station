
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
/* FUNCTIONS DECLARATION -----------------------------------------------------*/

void http_json_parser(void *pvParameters)
{
	hHttpPort_t hHttpResponse;

	while(1)
	{
		if(xQueueReceive(httpRx_queue, (void * )&hHttpResponse, (portTickType)portMAX_DELAY))
		{
			ESP_LOGI(TAG, "Packet size=%d JSON: %s.", hHttpResponse.packetSize, hHttpResponse.packetBuffer);
			for(uint8_t i = 0; i < hHttpResponse.packetSize; ++i)
			{
				ESP_LOGI(TAG, "%d", hHttpResponse.packetBuffer[i]);
			}
		}
	}
}


void app_main(void)
{
	ESP_ERROR_CHECK(nvs_flash_init() );

	wifiInit_ssdConnect();

//    xTaskCreate(&http_get_task, "http_get_task", 4096, NULL, 5, NULL);
//
    xTaskCreate(&http_json_parser, "http_get_task", 4096, NULL, 5, NULL);
}
