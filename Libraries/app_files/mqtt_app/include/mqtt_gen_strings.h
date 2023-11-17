/*
 * mqqt_gen_strings.h
 *
 *  Created on: 26 окт. 2022 г.
 *      Author: kir
 */

#ifndef USERLIB_INC_MQTT_GEN_STRINGS_H_
#define USERLIB_INC_MQTT_GEN_STRINGS_H_


#include <stdint.h>

typedef enum{
	INPUT_TOPIK = 1,
	OUTPUT_TOPIK = 2
}mqtt_topik_string_type;

typedef enum{
	INP_CONF_PAYLOAD = 1,
	OUT_CONF_PAYLOAD = 2
}mqqt_conf_payload_string_type;

int set_device_id(unsigned char * dev_id);
int set_device_conf_ip(unsigned char * chr_ip);
int set_home_assistant_prefix(char * prefix_string, uint8_t prefix_len);
int get_config_topik_string (char * buff, uint8_t topik_type, uint8_t obj_number);
int get_config_payload_string( char * payload, uint8_t payload_type, uint8_t obj_number);
int convert_bint_to_JSON_statusIO(char * buff, uint8_t bin, char * on_mess, char * off_mess, char * key_name, uint8_t number_io);
int generate_comand_topik_for_subscrabe(unsigned char * buff, uint8_t obj_number);
int generate_status_topik(char * buff, mqtt_topik_string_type topik_type);

#endif /* USERLIB_INC_MQTT_GEN_STRINGS_H_ */
