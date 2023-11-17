/*
 * http_app.h
 *
 *  Created on: 19 февр. 2023 г.
 *      Author: kir
 */

#ifndef HTTP_APP_H_
#define HTTP_APP_H_

#include "definition.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "semphr.h"

#include "httpServer.h"
//#include "httpClient.h"
#include "httpParser.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint8_t http_post_cgi_processor(uint8_t * uri_name, uint8_t * uri, uint8_t * buf, uint16_t * len);

#endif /* HTTP_APP_H_ */
