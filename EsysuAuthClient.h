/*
 *  Copyright (c) 2010，XRouter GROUP
 *  All rights reserved.
 *
 *  Name        : EsysuAuthClient.h
 *  Description :
 *
 *  Version     : 1.0
 *  Author      : Xu Guanglin
 *  Created on  : Dec 2, 2010
 */

#ifndef ESYSUAUTHCLIENT_H_
#define ESYSUAUTHCLIENT_H_

#include 	<stdio.h>
#include 	<stdlib.h>
#include 	<syslog.h>
#include	<string.h>
#include	<unistd.h>
#include 	<sys/ioctl.h>
#include 	<arpa/inet.h>
#include 	<net/if.h>
#include 	<pcap.h>
#include	<signal.h>

#include	"debug.h"
#include	"custom_types.h"
#include	"packetformats/general.h"
#include	"packetFactory.h"
#include	"interfaceInfoProvider.h"
#include	"initDeamon.h"

#define ALL_PARAMS 	"n:dD:u:p:t:P:h"

void processArguments(int argc, char** argv);
void registerAllSignalHandlers();
void initDeamonOnDemand();
void startAuthentication();
void openInterface();
void setCaptureFilter();
void startCapture();
void triggleAuthentication();
void putWatchDog();
void packet_handler(u_char *user, const struct pcap_pkthdr *pkt_header,
		const u_char *pkt_data);

void sendEapolStart();
void sendEapolLogoff();
void sendUsername();
void sendPapPassword();
void sendKeepState();
void registerSignalHandler(int signalId, void (*handler)(int));
void signalHandler(int sigid);

#endif /* ESYSUAUTHCLIENT_H_ */
