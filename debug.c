/*
 *  Copyright (c) 2010，XRouter GROUP
 *  All rights reserved.
 *
 *  Name        : debug.c
 *  Description :
 *
 *  Version     : 1.0
 *  Author      : Xu Guanglin
 *  Created on  : Dec 11, 2010
 */

#include "debug.h"

#ifdef __DEBUG__
void debug(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
}

void debugPacketBytes(u8* packet, int len) {
//	int i = 0;
//	for (i = 0; i < len; i++) {
//		debug("%x ", packet[i]);
//	}
//	debug("\n");
}
#else
void debug(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
}
void debugPacketBytes(u8* packet, int len) {
}
#endif
