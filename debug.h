/*
 *  Copyright (c) 2010，XRouter GROUP
 *  All rights reserved.
 *
 *  Name        : debug.h
 *  Description :
 *
 *  Version     : 1.0
 *  Author      : Xu Guanglin
 *  Created on  : Dec 11, 2010
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include 	<stdarg.h>
#include	<stdio.h>

#include "custom_types.h"

#define DEBUG_LOG_PRINTF(FMT, ...) \
	do { \
		char tmp[256]; \
		sprintf(tmp, FMT, __VA_ARGS__); \
		debug("%s", tmp); \
		syslog(LOG_DAEMON|LOG_INFO, "%s", tmp); \
	} while (0);

void debug(const char *fmt, ...);
void debugPacketBytes(u8* packet, int len);

#endif /* DEBUG_H_ */
