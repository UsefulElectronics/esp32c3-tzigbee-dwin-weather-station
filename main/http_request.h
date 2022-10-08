
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

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#include "protocol_examples_common.h"
/* MACROS --------------------------------------------------------------------*/

/* Constants that aren't configurable in menuconfig */
#define WEB_SERVER "worldtimeapi.org"
#define WEB_PORT "80"
#define WEB_PATH "/api/timezone/Europe/Istanbul"

/* ENUMORATIONS --------------------------------------------------------------*/

/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* FUNCTIONS DECLARATION -----------------------------------------------------*/
void http_get_task			(void *pvParameters);

void wifiInit_ssdConnect	(void);

#endif /* MAIN_HTTP_REQUEST_H_ */

/**************************  Useful Electronics  ****************END OF FILE***/
