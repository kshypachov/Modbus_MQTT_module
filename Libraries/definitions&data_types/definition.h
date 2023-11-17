/*
 * definition.h
 *
 *  Created on: 12 янв. 2023 г.
 *      Author: kir
 */

#ifndef DEFINITIONS_DATA_TYPES_DEFINITION_H_
#define DEFINITIONS_DATA_TYPES_DEFINITION_H_
/* used sockets 0, 1, 5,6,7
 * 0 - DHCP
 * 1 - HTTPServer
 * 4 - MQTTClient
 * 5 -
 * 6 -
 * 7 - ModbusTCP
 */

/*--------App def----------------*/
#define delay1s			1000
#define delay10s		delay1s * 10
#define delay5s			delay1s * 5
#define delay3s			delay1s * 3
#define delay0_5s		delay1s / 2
#define delay0_2s		delay1s / 5
#define delay0_1s		delay1s / 10
#define SPI_THREADSAFE
#define UID_BASE		0x1FFF7A10 // адрес в котором хранится уникальный айди процессора

/*-------LittleFS----------------*/
#define LFS_BUF_SIZE 	256

/*-----------Q def---------------*/
#define QUEUE_LENGTH 	1

/*---------NetworkDef------------*/
#define SOCKET_BUFF_SIZE		2048 //buffer allocated in w5500 for socket
#define MAX_DHCP_FAIL			10
#define ETH_LINK_UP				1
#define ETH_LINK_DOWN			0

/*------------------MODBUS DEF----------------------*/
#define MB_TCP_BUF_SIZE  		2048
#define REG_COILS_START			0
#define REG_COILS_NREGS			3
#define REG_DISCRETE_START		0
#define REG_DISCRETE_NREGS		3
#define S_REG_INPUT_START       0
#define S_REG_INPUT_NREGS       10
#define REG_HOLDING_START     	0
#define REG_HOLDING_NREGS     	10
#define MODBUS_PORT				502

/*----------------------DNS--------------------------*/
#define	MAX_DNS_BUF_SIZE		2048
#define MAX_DOMAIN_NAME   		128
#define	MAX_DNS_RETRY     		2
#define	DNS_WAIT_TIME     		3

/*------------LFS_DEFINEs----------------------------*/
#define FLASH_BLOCK_CYCLES 		10000
#define LFS_NO_MALLOC
#define LFS_NO_DEBUG
#define LFS_NO_ERROR
//#define LFS_THREADSAFE

/*---------------HTTP_DEFINEs------------------------*/
#define HTTP_SERVER_NSOCKETS	1
#define HTTP_SERVER_PORT		80
#define MAX_CONTENT_NAME_LEN	30
#define MAX_CONTENT_CALLBACK	3

/*----------------MQTT-------------------------------*/
#define TOPIK_MAX_LEN				128
#define IP_v4_LEN					18
#define DEV_UID_LEN					14
#define MQTT_TEMP_BUF				1024
#define MAX_MQTT_LOGIN_LEN			33
#define MAX_MQTT_PASS_LEN			33
#define NUMBER_SWITCH_TOPIKS		REG_COILS_NREGS
#define NUMBER_INPUT_TOPIKS			REG_DISCRETE_NREGS
#define UPDATE_MQTT_PARAM_INTERVAL	60
#define RELOGIN_MQTT_INTERVAL		3600


/*------------W5500_Sockets--------------------------*/
#define DHCP_SOCKET				0
#define HTTP_SERVER_SOCKET		1
#define MQTT_CLIENT_SOCKET		4
#define START_MODBUS_SOCKET		7
#define	END_MODBUS_SOCKET		7

/*-----------DEBUG_PARAMETERS-----------------------*/
#define DEBUG_FREERTOS_ENABLE
#endif /* DEFINITIONS_DATA_TYPES_DEFINITION_H_ */
