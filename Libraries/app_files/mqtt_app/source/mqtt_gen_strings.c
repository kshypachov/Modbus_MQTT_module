/*
 * mqtt_gen_strings.c
 *
 *  Created on: 26 окт. 2022 г.
 *      Author: kir
 */

#include "mqtt_gen_strings.h"
#include "definition.h"
#include "string.h"
#include "stdio.h"

unsigned char home_assistant_prefix[] = {"homeassistant"};
unsigned char bin_sens[] =				{"binary_sensor"};
unsigned char device_name[] = 			{"biver_"};
unsigned char switch_sens[] = 			{"switch"};
unsigned char input[] = 				{"input"};




unsigned char config_topik_temlate[] = {"%s/%s/biver_%s/%s%u/config"};

unsigned char switch_conf_payload_templ[] = {  //todo change to const char
		"{\n"
			"\t\"device_class\":\"switch\",\n"
			"\t\"expire_after\" : 1800 ,\n"
			"\t\"state_topic\":\"biver_%s/switches\",\n"
			"\t\"command_topic\" : \"biver_%s/sw%u/set\",\n"
			"\t\"value_template\":\"{{ value_json.SW%u }}\",\n"
			"\t\"name\":\"Switch%u\",\n"
			"\t\"unique_id\":\"biver_%s_switch%u\",\n"
			"\t\"device\":{\n"
					"\t\t\"identifiers\":[\"biver_%s\"],\n"
					"\t\t\"name\":\"biver_%s\",\n"
					"\t\t\"model\":\"unit sensors 3i 3o\",\n"
					"\t\t\"manufacturer\":\"Shypachov\",\n"
					"\t\t\"hw_version\" : \"v0.1\",\n"
					"\t\t\"sw_version\" : \"2023-08-18\",\n"
					"\t\t\"configuration_url\" : \"http://%s\"\n"
					"\t}\n"
		"}"
											};

unsigned char input_conf_payload_templ[] = {  //todo change to const char
		"{\n"
			"\t\"device_class\" :\"power\",\n"
			"\t\"expire_after\" : 1800 ,\n"
			"\t\"state_topic\" :\"biver_%s/inputs\",\n"
			"\t\"value_template\":\"{{ value_json.INP%u }}\",\n"
			"\t\"name\":\"Input%u\",\n"
			"\t\"unique_id\":\"biver_%s_input%u\",\n"
			"\t\"device\":{\n"
					"\t\t\"identifiers\":[\"biver_%s\"],\n"
					"\t\t\"name\":\"biver_%s\",\n"
					"\t\t\"model\":\"unit sensors 3i 3o\",\n"
					"\t\t\"manufacturer\":\"Shypachov\",\n"
					"\t\t\"hw_version\" : \"v0.1\",\n"
					"\t\t\"sw_version\" : \"2023-08-18\",\n"
					"\t\t\"configuration_url\" : \"http://%s\"\n"
					"\t}\n"
		"}"
											};



char subscr_topik_template[] 	= {"biver_%s/sw%u/set"	};
char input_status_topik_temp[] 	= {"biver_%s/inputs"	};
char switch_status_topik_temp[] = {"biver_%s/switches"	};

unsigned char unical_id[DEV_UID_LEN] = {'\0'};
unsigned char dev_conf_ip[IP_v4_LEN] = {'\0'};

int set_device_id(unsigned char * dev_id){
	uint8_t cpy_len = 0;
	if (strlen(dev_id) >= DEV_UID_LEN ){
		cpy_len = DEV_UID_LEN;
	}else{
		cpy_len = strlen(dev_id);
	}
	strncpy(unical_id, dev_id, cpy_len);
	return 0;
}

int set_device_conf_ip(unsigned char * chr_ip){

	strcpy(dev_conf_ip, "\0");
	if(strlen(chr_ip) <= IP_v4_LEN){
		strncpy(dev_conf_ip, chr_ip, strlen(chr_ip));
		return 0;
	}
	return -1;
}

int set_home_assistant_prefix(char * prefix_string, uint8_t prefix_len){
//TODO validation need
 	strncpy(home_assistant_prefix, prefix_string, prefix_len);

	return 0;
}

int get_config_topik_string (char * buff, uint8_t topik_type, uint8_t obj_number){

	strcpy(buff, '\0');
	switch (topik_type) {
		case INPUT_TOPIK:
			sprintf(buff, config_topik_temlate, home_assistant_prefix, bin_sens, unical_id, input, obj_number);
			break;

		case OUTPUT_TOPIK:
			sprintf(buff, config_topik_temlate, home_assistant_prefix, switch_sens, unical_id, switch_sens, obj_number);
			break;

		default:
			break;
	}


	return 0;
}

int get_config_payload_string( char * payload, uint8_t payload_type, uint8_t obj_number){

	uint8_t len = 0;
	strcpy(payload, "\0");

	switch (payload_type) {
		case OUT_CONF_PAYLOAD:
			len = sprintf(payload, switch_conf_payload_templ, unical_id, unical_id, obj_number, obj_number, obj_number, unical_id, obj_number, unical_id, unical_id, dev_conf_ip);
			return len;
			break;

		case INP_CONF_PAYLOAD:
			len = sprintf(payload, input_conf_payload_templ, unical_id, obj_number, obj_number,  unical_id, obj_number, unical_id, unical_id, dev_conf_ip);
			return len;
			break;

		default:
			return -1;
			break;
	}
}

int convert_bint_to_JSON_statusIO(char * buff, uint8_t bin, char * on_mess, char * off_mess, char * key_name, uint8_t number_io){

	unsigned char template[] = {"\t\"%s%u\" : \"%s\""};
	uint8_t test;
	unsigned char internal_buff[30];

	strcpy(buff, "\0");
	strcat(buff, "{\n");
	while (number_io > 0){
		test = bin & 0b00000001 << number_io - 1;
		if (test){
			sprintf(internal_buff, template, key_name, number_io, on_mess);
		}else{
			sprintf(internal_buff, template, key_name, number_io, off_mess);
		}
		strcat(buff, internal_buff);

		number_io --;

		if (number_io) {
			strcat(buff, ",\n");
		}
	}

	strcat(buff, "\n}");

	return 0;
}

int generate_comand_topik_for_subscrabe(unsigned char * buff, uint8_t obj_number){

	strcpy(buff, "\0");
	sprintf(buff, subscr_topik_template, unical_id, obj_number);
	return 0;

}

int generate_status_topik(char * buff, mqtt_topik_string_type topik_type){

	strcpy(buff, "\0");

	switch (topik_type) {
		case INPUT_TOPIK:
			sprintf(buff, input_status_topik_temp, unical_id);
			break;

		case OUTPUT_TOPIK:
			sprintf(buff, switch_status_topik_temp, unical_id);
			break;

		default:
			return -1;
			break;
	}

	return 0;
}
