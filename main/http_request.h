
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

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"

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


/* ENUMORATIONS --------------------------------------------------------------*/

/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/
typedef struct
{
	uint8_t packetSize;
	char packetBuffer[HTTP_RESPONSE_LENGTH_MAX];
}hHttpPort_t;
/* VARIABLES -----------------------------------------------------------------*/
extern QueueHandle_t httpRx_queue;
/* FUNCTIONS DECLARATION -----------------------------------------------------*/
void http_get_task			(void *pvParameters);

void wifiInit_ssdConnect	(void);

#endif /* MAIN_HTTP_REQUEST_H_ */

/**************************  Useful Electronics  ****************END OF FILE***/
