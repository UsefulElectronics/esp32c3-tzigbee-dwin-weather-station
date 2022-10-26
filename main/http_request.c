
/**
 ******************************************************************************
 * @Channel Link    :  https://www.youtube.com/user/wardzx1
 * @file    		:  http_request.c
 * @author  		:  Ward Almasarani - Useful Electronics
 * @version 		:  v.1.0
 * @date    		:  Oct 8, 2022
 * @brief   		:
 *
 ******************************************************************************/


/* INCLUDES ------------------------------------------------------------------*/
#include "http_request.h"

/* PRIVATE STRUCTRES ---------------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/
static const char *TAG = "HTTP";

QueueHandle_t httpRx_queue;

hHttpPort_t hHttpResponse;


/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/
//static void http_event_handler	(esp_http_client_event_t *evt);
static void httpClientInit		(char* urlString, urlId_e urlId);

/* FUNCTION PROTOTYPES -------------------------------------------------------*/
void http_get_weather_task(void *pvParameters)
{

	httpRx_queue = xQueueCreate(3, sizeof(hHttpPort_t));
    while(1)
    {
    	httpClientInit(OPEN_WEATHER_URL, URL_WEATHER);

        vTaskDelay(3600000 / portTICK_PERIOD_MS);
    }
}

void http_get_bitcoin_task(void *pvParameters)
{

	httpRx_queue = xQueueCreate(3, sizeof(hHttpPort_t));
    while(1)
    {
    	httpClientInit(BITCOIN_URL, URL_BTC);

        vTaskDelay(60000 / portTICK_PERIOD_MS);
    }
}

void http_get_ethereum_task(void *pvParameters)
{

	httpRx_queue = xQueueCreate(3, sizeof(hHttpPort_t));
    while(1)
    {
    	httpClientInit(ETHEREUM_URL, URL_ETH);

        vTaskDelay(60000 / portTICK_PERIOD_MS);
    }
}

void http_get_prayer_task(void *pvParameters)
{

	httpRx_queue = xQueueCreate(3, sizeof(hHttpPort_t));
    while(1)
    {
    	httpClientInit(PRAYER_API, URL_PRAYER);

        vTaskDelay(86400000 / portTICK_PERIOD_MS);
    }
}

static void http_event_handler(esp_http_client_event_t *evt)
{
	switch (evt->event_id)
	{
		case HTTP_EVENT_ON_DATA:
			//Accumulate Json string in the response buffer
		    memcpy(hHttpResponse.packetBuffer + hHttpResponse.packetSize, evt->data, evt->data_len);
		    //Response cursor move forward
		    hHttpResponse.packetSize += evt->data_len;

			break;
        case HTTP_EVENT_ON_CONNECTED:

            ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");

            break;

        case HTTP_EVENT_DISCONNECTED:

            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");

            hHttpResponse.packetSize = strlen(hHttpResponse.packetBuffer);

//            xQueueSendToBack(httpRx_queue, &hHttpResponse, portMAX_DELAY);

            memset(&hHttpResponse, 0, sizeof(hHttpPort_t));



            break;

		default:
			break;
	}
}

void httpClientInit(char* urlString, urlId_e urlId)
{

    esp_http_client_config_t config_get =
    {
        .url = urlString,
        .method = HTTP_METHOD_GET,
        .cert_pem = NULL,
        .event_handler = http_event_handler,
    };

    hHttpResponse.urlId	= urlId;

    esp_http_client_handle_t client = esp_http_client_init(&config_get);
    //Perform HTTP request
    esp_http_client_perform(client);
    //Close socket after getting a response to the GET request
    esp_http_client_cleanup(client);
}


void wifiInit_ssidConnect(void)
{
	//Create queue for transfaring data received over HTTP response to the main layer


    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());


}

/**************************  Useful Electronics  ****************END OF FILE***/
