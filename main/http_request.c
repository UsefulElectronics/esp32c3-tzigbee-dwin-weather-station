
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

static const char *REQUEST = "GET " WEB_PATH " HTTP/1.0\r\n"
    "Host: "WEB_SERVER":"WEB_PORT"\r\n"
    "User-Agent: esp-idf/1.0 esp32\r\n"
    "\r\n";

QueueHandle_t httpRx_queue;

hHttpPort_t hHttpResponse;
/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/
static void _http_event_handler(esp_http_client_event_t *evt);
/* FUNCTION PROTOTYPES -------------------------------------------------------*/
void http_get_task(void *pvParameters)
{
    const struct addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
    };
    struct addrinfo *res;
    struct in_addr *addr;
//    int s, r;
    int socketId = 0;
    hHttpPort_t hHttpResponse;
    memset(&hHttpResponse, 0, sizeof(hHttpPort_t));
//    char recv_buf[HTTP_RESPONSE_LENGTH_MAX];
    //HTTP response data queue FIFO
    httpRx_queue = xQueueCreate(5, HTTP_RESPONSE_LENGTH_MAX);

    while(1)
    {
        int err = getaddrinfo(WEB_SERVER, WEB_PORT, &hints, &res);

        if(err != 0 || res == NULL) {
            ESP_LOGE(TAG, "DNS lookup failed err=%d res=%p", err, res);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        /* Code to print the resolved IP.

           Note: inet_ntoa is non-reentrant, look at ipaddr_ntoa_r for "real" code */
        addr = &((struct sockaddr_in *)res->ai_addr)->sin_addr;
        ESP_LOGI(TAG, "DNS lookup succeeded. IP=%s", inet_ntoa(*addr));

        socketId = socket(res->ai_family, res->ai_socktype, 0);
        if(socketId < 0) {
            ESP_LOGE(TAG, "... Failed to allocate socket.");
            freeaddrinfo(res);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "... allocated socket");

        if(connect(socketId, res->ai_addr, res->ai_addrlen) != 0) {
            ESP_LOGE(TAG, "... socket connect failed errno=%d", errno);
            close(socketId);
            freeaddrinfo(res);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }

        ESP_LOGI(TAG, "... connected");
        freeaddrinfo(res);

        if (write(socketId, REQUEST, strlen(REQUEST)) < 0) {
            ESP_LOGE(TAG, "... socket send failed");
            close(socketId);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "... socket send success");

        struct timeval receiving_timeout;
        receiving_timeout.tv_sec = 5;
        receiving_timeout.tv_usec = 0;
        if (setsockopt(socketId, SOL_SOCKET, SO_RCVTIMEO, &receiving_timeout,
                sizeof(receiving_timeout)) < 0) {
            ESP_LOGE(TAG, "... failed to set socket receiving timeout");
            close(socketId);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "... set socket receiving timeout success");

        /* Read HTTP response */
        do
        {
        	//Clear HTTP response buffer
            bzero(hHttpResponse.packetBuffer, HTTP_RESPONSE_LENGTH_MAX);
            //Read data received HTTP response and place it in the handler buffer
            hHttpResponse.packetSize = read(socketId, hHttpResponse.packetBuffer, HTTP_RESPONSE_LENGTH_MAX - 1);
            //place data in the queue only if there is data in the http buffer
            if(hHttpResponse.packetSize)
            {
            	//Data obtained from socket read will be placed in the queue so it is accessed from the main layer
            	xQueueSendToBack(httpRx_queue, &hHttpResponse, portMAX_DELAY);
            }


//            for(int i = 0; i < r; i++)
//            {
//                putchar(recv_buf[i]);
//            }
        } while( hHttpResponse.packetSize > 0);


        close(socketId);
        vTaskDelay(61000 / portTICK_PERIOD_MS);

    }
}

static void http_event_handler(esp_http_client_event_t *evt)
{
	switch (evt->event_id)
	{
		case HTTP_EVENT_ON_DATA:

//		    int content_length =  esp_http_client_fetch_headers(evt->client);
//		    int total_read_len = 0, read_len;
		    hHttpResponse.packetSize = evt->data_len;

//		    esp_http_client_close(evt->client);
//		    hHttpResponse.packetBuffer
//		    xQueueSendToBack(httpRx_queue, &hHttpResponse, portMAX_DELAY);


			ESP_LOGI(TAG, "rx size:%d /n rx Content: ", evt->data_len);

			for(int i = 0; i < evt->data_len; i++)
			{
				putchar(hHttpResponse.packetBuffer[i]);
			}

//
//		    xQueueSendToBack(httpRx_queue, &hHttpResponse, portMAX_DELAY);


			break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;

        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            break;

		default:
			break;
	}
}

void httpClientInit(void)
{
	 esp_http_client_config_t config =
	 {
//	        .host 			= WEB_SERVER,
//	        .path 			= WEB_PATH,
			.url			= OPEN_WEATHER_URL,
			.port 			= 80,
//	        .query 			= "esp",
	        .event_handler	= (http_event_handle_cb*) http_event_handler,
	        .user_data		= hHttpResponse.packetBuffer,        // Pass address of local buffer to get response
			.method 		= HTTP_METHOD_GET,
			 .disable_auto_redirect = true,
			 .crt_bundle_attach = esp_crt_bundle_attach,

	    };
	    esp_http_client_handle_t client = esp_http_client_init(&config);

	    esp_http_client_perform(client);

	    esp_http_client_read_response(client, hHttpResponse.packetBuffer,  HTTP_RESPONSE_LENGTH_MAX);

}

void wifiInit_ssdConnect(void)
{
	//Create queue for transfaring data received over HTTP response to the main layer
	httpRx_queue = xQueueCreate(5, HTTP_RESPONSE_LENGTH_MAX);

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());

    httpClientInit();
}

/**************************  Useful Electronics  ****************END OF FILE***/
