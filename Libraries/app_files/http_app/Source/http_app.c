/*
 * http_app.c
 *
 *  Created on: 19 февр. 2023 г.
 *      Author: kir
 */
#include "http_app.h"
#include "data_types.h"

xQueueHandle MQTT_Q;
xQueueHandle HomeAsistant_Q;
xQueueHandle ModbusTCP_Q;

void http_parse_params_init (xQueueHandle xQueue, uint8_t parameters_type){

	if (parameters_type == MQTT){
		if (xQueue){
			MQTT_Q = xQueue;
		}
	}else if (parameters_type == HA){
		if (xQueue){
			HomeAsistant_Q = xQueue;
		}
	}else if (parameters_type == ModBusTCP){
		if (xQueue){
			ModbusTCP_Q = xQueue;
		}
	}
}

void param_from_http_to_Q(char * http_resp, const uint8_t parameters_type){

	MQTT_cred_struct	MQTT_params;
	Home_assist_struct	HA_params;
	ModBusTCP_struct	ModBusTCP_parameters;				// MobBusTCP settins
	uint8_t				ip_addr[4];
	uint16_t			port;

	uint8_t * buf;

	switch (parameters_type) {

		case MQTT:

			xQueuePeek(MQTT_Q ,&MQTT_params,0);
			memset(&MQTT_params, 0, sizeof(MQTT_params));

			if (strstr(get_http_param_value( http_resp, "MQTT"), "on")){
				MQTT_params.enable = 1;
			}else if (strstr(get_http_param_value( http_resp, "MQTT"), "off")) {
				MQTT_params.enable = 0;
			}

			buf = (uint8_t *) get_http_param_value( http_resp, "login");
			if (buf || (strlen(buf) < sizeof(MQTT_params.login) / sizeof(char)) ){
				strcpy(MQTT_params.login, buf);
			}

			buf = (uint8_t *) get_http_param_value( http_resp, "pass");
			if (buf || (strlen(buf) < sizeof(MQTT_params.pass) / sizeof(char)) ){
				strcpy(MQTT_params.pass, buf);
			}

			buf = (uint8_t *) get_http_param_value( http_resp, "ip_addr");
			if (buf || (strlen(buf) <= 15) ) {						 // 15 символов, максимальная длина IPv4
				sscanf(buf, "%d.%d.%d.%d", &ip_addr[0], &ip_addr[1], &ip_addr[2], &ip_addr[3]);
			}

			MQTT_params.ip[0] = ip_addr[0];
			MQTT_params.ip[1] = ip_addr[1];
			MQTT_params.ip[2] = ip_addr[2];
			MQTT_params.ip[3] = ip_addr[3];

			buf = (uint8_t *) get_http_param_value( http_resp, "port");
			if (buf || (strlen(buf) <= 5) ) {						// 5 символов, максимальная длина номера порта
				sscanf(buf, "%d", &port);
			}

			MQTT_params.port = port;

			MQTT_params.save = 1;

			xQueueOverwrite(MQTT_Q, (void *)&MQTT_params);

			//todo save to Queue

			break;

		case ModBusTCP:
			memset(&ModBusTCP_parameters, 0, sizeof(ModBusTCP_parameters));
			if (strstr(get_http_param_value( http_resp, "ModbusTCP"), "on")){
				ModBusTCP_parameters.enable = 1;
			}else if (strstr(get_http_param_value( http_resp, "ModbusTCP"), "off")) {
				ModBusTCP_parameters.enable = 0;
			}
			ModBusTCP_parameters.save = 1;

			xQueueOverwrite(ModbusTCP_Q, (void *)&ModBusTCP_parameters);
			break;

		case HA:

			break;
		default:
			break;
	}
}


uint8_t http_post_cgi_processor(uint8_t * uri_name, uint8_t * uri, uint8_t * buf, uint16_t * len){

	uint8_t ret;
	len = 0;

	if(strcmp((const char *)uri_name, "settings_MQTT.cgi") == 0){
		param_from_http_to_Q((char *)uri, MQTT);
		ret = HTTP_OK;
	}else if (strcmp((const char *)uri_name, "settings_ModbusTCP.cgi") == 0){
		param_from_http_to_Q((char *)uri, ModBusTCP);
		ret = HTTP_OK;
	}


	return ret;
}

