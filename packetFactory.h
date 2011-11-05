/*
 *  Copyright (c) 2010，XRouter GROUP
 *  All rights reserved.
 *
 *  Name        : packetGenerator.h
 *  Description :
 *
 *  Version     : 1.0
 *  Author      : Xu Guanglin
 *  Created on  : Dec 11, 2010
 */

#ifndef PACKETGENERATOR_H_
#define PACKETGENERATOR_H_


#include 	<net/if.h>

#include 	<arpa/inet.h>
#include	<string.h>

#include	"custom_types.h"
#include	"packetformats/general.h"
#include	"debug.h"

struct packetEncap {
	u8 buf[256];
	int length;
};

void generateEapolStart();
void generateEapolLogoff();
void generateUsername();
void generatePapPassword();
void generateKeepState();
void setSrcDstTypeVersionCommonHeader(u8* buf);

int isEapUserNameRequest(u8* buf);
int isEapPapPasswordRequest(u8* buf);
int isSuccess(u8* buf);
int isFailure(u8* buf);
int isKeepState(u8* buf);

#endif /* PACKETGENERATOR_H_ */
