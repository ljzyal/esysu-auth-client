/*
 *  Copyright (c) 2010，XRouter GROUP
 *  All rights reserved.
 *
 *  Name        : interfaceInfoProvider.c
 *  Description :
 *
 *  Version     : 1.0
 *  Author      : Xu Guanglin
 *  Created on  : Dec 11, 2010
 */

#include	"interfaceInfoProvider.h"

void getInterfaceAddress(char* interfaceName, u8* interfaceHWAddr) {
	register int fd;
	struct ifreq buf;
	strcpy(buf.ifr_name, interfaceName);
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0) {
		if (!(ioctl(fd, SIOCGIFHWADDR, (char *) &buf))) {
			memcpy(interfaceHWAddr, buf.ifr_hwaddr.sa_data, 6);
			close(fd);
//			printf("%s hw address: %x:%x:%x:%x:%x:%x\n", interfaceName,
//					interfaceHWAddr[0], interfaceHWAddr[1], interfaceHWAddr[2],
//					interfaceHWAddr[3], interfaceHWAddr[4], interfaceHWAddr[5]);

		} else {
			close(fd);
			printf("Error: %s has no hw address\n", interfaceName);
			exit(0);
		}
	}
}
