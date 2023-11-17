/*
 * ethernet_init.h
 *
 *  Created on: 15 џэт. 2023 у.
 *      Author: kir
 */

#ifndef IOLIBRARY_ETHERNET_INIT_H_
#define IOLIBRARY_ETHERNET_INIT_H_

#include "stdint.h"
#include "wizchip_conf.h"

void w5500_lib_init(wiz_NetInfo * NetInfoStructure);
void network_init(void);
void Callback_IPConflict(void);
void Callback_IPAssigned(void);
void dhcp_rutine(void);
int link_status(void);
int link_duplex(void);
int link_speed(void);
int assigned_ip(void);
void set_ip_aasigned(void);
int	 w5500_init(void);
void w5500_delay_cb(void (*function)(uint32_t time));

#endif /* 3RD_PART_SOFTWARE_IOLIBRARY_ETHERNET_INIT_H_ */
