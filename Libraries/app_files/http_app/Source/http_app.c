/*
 * http_app.c
 *
 *  Created on: 19 февр. 2023 г.
 *      Author: kir
 */
#include "http_app.h"
#include "data_types.h"
#include <stdio.h>
#include "definition.h"




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
				//TODO change to strncpy
			}

			buf = (uint8_t *) get_http_param_value( http_resp, "pass");
			if (buf || (strlen(buf) < sizeof(MQTT_params.pass) / sizeof(char)) ){
				strcpy(MQTT_params.pass, buf);
				//TODO change to strncpy
			}

			buf = (uint8_t *) get_http_param_value( http_resp, "uri");
			if (buf || (strlen(buf) <= 15) ) {						 // 15 символов, максимальная длина IPv4
				//sscanf(buf, "%d.%d.%d.%d", &ip_addr[0], &ip_addr[1], &ip_addr[2], &ip_addr[3]);
				strncpy(MQTT_params.uri, buf, MAX_DOMAIN_NAME - 1);
			}

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

void param_from_Q_to_JSON(char * buf, const uint8_t parameters_type, uint16_t * len){

	MQTT_cred_struct	MQTT_params;

	unsigned const char template[] = {"{\"login\": \"%s\", \"pass\": \"********\", \"addr\": \"%s\", \"port\": \"%u\", \"enabled\": \"%u\"}"};//{"login": "admin", "pass": "1234", "addr": "10.0.2.20", "port": "1883", "enabled": 1 }
	char * buff_start_addr;
	char tmp_addr[65] = {0,};

	switch (parameters_type) {

		case MQTT:
			buff_start_addr = buf;
			xQueuePeek(MQTT_Q ,&MQTT_params,0);
			//sprintf(tmp_addr, "%u.%u.%u.%u", MQTT_params.ip[0], MQTT_params.ip[1], MQTT_params.ip[2], MQTT_params.ip[3]);
			//strncpy()
			sprintf(buf, (const char *)template, MQTT_params.login, MQTT_params.uri, MQTT_params.port, MQTT_params.enable);

			//len = buf - buff_start_addr;
			*len = strlen(buf);

			break;

		default:
			break;
	}
}


uint8_t http_post_cgi_processor(uint8_t * uri_name, uint8_t * uri, uint8_t * buf, uint16_t * len){

	uint8_t ret;
	len = 0;

	uint16_t tmp;
	uint16_t resvlen = 0;
	uint8_t  tmp_buf[55];


	if(strcmp((const char *)uri_name, "settings_MQTT.cgi") == 0){
		param_from_http_to_Q((char *)uri, MQTT);
		ret = HTTP_OK;
	}else if (strcmp((const char *)uri_name, "settings_ModbusTCP.cgi") == 0){

		param_from_http_to_Q((char *)uri, ModBusTCP);
		ret = HTTP_OK;
	}else if (strcmp((const char *)uri_name, "firmware.cgi") == 0){

		while(1){
			tmp = getSn_RX_RSR(HTTP_SERVER_SOCKET);
			if (tmp > 0){
				resvlen += recv(HTTP_SERVER_SOCKET, tmp_buf, 50);
			}else{
				break;
			}
		}

		ret = HTTP_OK;
	}

	return ret;
}

uint8_t http_get_cgi_processor(uint8_t * uri_name, uint8_t * buf, uint16_t * len){

	uint8_t ret = HTTP_FAILED;

	if(strcmp((const char *)uri_name, "settings_MQTT.cgi") == 0){
		param_from_Q_to_JSON((char *)buf, MQTT, len);
		ret = HTTP_OK;
	}

	return ret;
}
