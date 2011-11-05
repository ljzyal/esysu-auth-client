/*
 *  Copyright (c) 2010，XRouter GROUP
 *  All rights reserved.
 *
 *  Name        : interfaceInfoProvider.h
 *  Description :
 *
 *  Version     : 1.0
 *  Author      : Xu Guanglin
 *  Created on  : Dec 11, 2010
 */

#ifndef INTERFACEINFOPROVIDER_H_
#define INTERFACEINFOPROVIDER_H_

#include 	<sys/ioctl.h>
#include 	<arpa/inet.h>
#include 	<net/if.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>

#include	"custom_types.h"

void getInterfaceAddress(char* interfaceName, u8* interfaceHWAddr);

#endif /* INTERFACEINFOPROVIDER_H_ */
