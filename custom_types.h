/*
 *  Copyright (c) 2010，XRouter GROUP
 *  All rights reserved.
 *
 *  Name        : customtype.h
 *  Description :
 *
 *  Version     : 1.0
 *  Author      : Xu Guanglin
 *  Created on  : Dec 2, 2010
 */

#ifndef CUSTOMTYPE_H_
#define CUSTOMTYPE_H_

#define TRIGGER_TYPE_BROADCAST 0
#define TRIGGER_TYPE_MULTICAST 1

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

struct UserArguments {
	char interface[256];
	int needDeamon;
	int needDhcp;
	char dhcpClient[256];
	char username[256];
	char password[256];
	int triggerType;
	char pidFile[256];
};

#endif /* CUSTOMTYPE_H_ */
