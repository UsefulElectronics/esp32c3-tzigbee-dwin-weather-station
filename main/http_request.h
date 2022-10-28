
/**
 ******************************************************************************
 * @Channel Link    :  https://www.youtube.com/user/wardzx1
 * @file    		:  http_request.h
 * @author  		:  Ward Almasarani - Useful Electronics
 * @version 		:  v.1.0
 * @date    		:  Oct 8, 2022
 * @brief   		:
 *
 ******************************************************************************/

#ifndef MAIN_HTTP_REQUEST_H_
#define MAIN_HTTP_REQUEST_H_


/* INCLUDES ------------------------------------------------------------------*/

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"


#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "stdbool.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#include "esp_http_client.h"
#include "esp_crt_bundle.h"

#include "privet_api.h"
#include "protocol_examples_common.h"
/* MACROS --------------------------------------------------------------------*/
#define HTTP_RESPONSE_LENGTH_MAX		1024

#define WIFI_CONNECTED_BIT 				BIT0
#define WIFI_FAIL_BIT      				BIT1
#define EXAMPLE_ESP_MAXIMUM_RETRY  		10

#define SYS_TICK()				xTaskGetTickCount() * portTICK_PERIOD_MS
/* ENUMORATIONS --------------------------------------------------------------*/
typedef enum
{
	URL_WEATHER 	= 0,
	URL_BTC,
	URL_ETH,
	URL_PRAYER
}urlId_e;
/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/
typedef struct
{
	uint8_t packetSize;
	urlId_e urlId;
	bool	available;
	uint32_t	availableTimeout;
	char packetBuffer[HTTP_RESPONSE_LENGTH_MAX];
}hHttpPort_t;
/* VARIABLES -----------------------------------------------------------------*/
extern QueueHandle_t 		httpRx_queue;
extern SemaphoreHandle_t 	httpSem;
extern hHttpPort_t 			hHttpResponse;
/* FUNCTIONS DECLARATION -----------------------------------------------------*/
void http_get_weather_task	(void *pvParameters);

void http_get_bitcoin_task	(void *pvParameters);

void http_get_ethereum_task	(void *pvParameters);

void http_get_prayer_task	(void *pvParameters);

void wifiInit_ssidConnect	(void);

#endif /* MAIN_HTTP_REQUEST_H_ */

/**************************  Useful Electronics  ****************END OF FILE***/
