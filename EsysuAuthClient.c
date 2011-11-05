/*
 *  Copyright (c) 2010，XRouter, GROUP
 *  All rights reserved.
 *
 *  Name        : EsysuAuthClient.c
 *  Description : an 802.1X Client for PAP protocol
 *
 *  Version     : 1.0
 *  Author      : Xu Guanglin
 *  Created on  : Dec 2, 2010
 */

#include "EsysuAuthClient.h"

struct UserArguments usrArgus = { "", 0, 0, "", "", "", TRIGGER_TYPE_BROADCAST,
		"/var/run/EsysuAuthClient.pid" };
u8 interfaceHWAddr[6] = { 0 };
u8 serverHWAddr[6] = { 0 };
int packetId = 0;
int state = 0; /*0:begin 1:sendStart 2:responseUser 3:responsePassword 4:success*/
pcap_t * fpInterface; /* Interface capture handle */
extern struct packetEncap packet;

/**
 * arguments:
 * -n <Interface> 	Specify network interface
 * -d				run as deamon
 * -D <DHCP Client>	Need DHCP after being authenticated
 * -u <username>	Specify username
 * -p <password>	Specify password
 * -t <triggle type> broadcast / multicast
 * -P <pid file>	Store process ID of daemon in file
 * -h				Help information
 *
 */
int main(int argc, char **argv) {
	processArguments(argc, argv);
	registerAllSignalHandlers();
	initDeamonOnDemand();
	startAuthentication();
	return EXIT_SUCCESS;
}

void processArguments(int argc, char** argv) {
	int opt;

	if (argc <= 1) {
		printf("EsysuAuthClient 1.0  --- An 802.1X PAP Client\n");
		printf("mzguanglin@gmail.com (2010)\n");
		printf("-n <Interface> Specify network interface\n");
		printf("-d run as deamon\n");
		printf("-D <DHCP Client> Need DHCP after being authenticated\n");
		printf("-u <username> Specify username\n");
		printf("-p <password> Specify password\n");
		printf("-t <triggle type> broadcast / multicast\n");
		printf("-P <pid file> Store process ID of daemon in file\n");
		printf("-h Help information\n");
		exit(0);
	}

	while ((opt = getopt(argc, argv, ALL_PARAMS)) != -1) {
		switch (opt) {
		case 'n':
//			printf("select n:%s\n", optarg);
			strcpy(usrArgus.interface, optarg);
			break;
		case 'd':
//			printf("select d\n");
			usrArgus.needDeamon = 1;
			break;
		case 'D':
//			printf("select D\n");
			usrArgus.needDhcp = 1;
			strcpy(usrArgus.dhcpClient, optarg);
			break;
		case 'u':
//			printf("select u:%s\n", optarg);
			strcpy(usrArgus.username, optarg);
			break;
		case 'p':
//			printf("select p:%s\n", optarg);
			strcpy(usrArgus.password, optarg);
			break;
		case 't':
//			printf("select t:%s\n", optarg);
			if (!strcmp(optarg, "broadcast")) {
				usrArgus.triggerType = TRIGGER_TYPE_BROADCAST;
			} else if (!strcmp(optarg, "multicast")) {
				usrArgus.triggerType = TRIGGER_TYPE_MULTICAST;
			} else {
				printf(
						"Error: -t argument must be either broadcast or multicast\n");
				exit(0);
			}
			break;
		case 'P':
//			printf("select P:%s\n", optarg);
			strcpy(usrArgus.pidFile, optarg);
			break;
		case 'h':
			printf("EsysuAuthClient 1.0  --- An 802.1X PAP Client\n");
			printf("mzguanglin@gmail.com (2010)\n");
			printf("-n <Interface> Specify network interface\n");
			printf("-d run as deamon\n");
			printf("-D <DHCP Client> Need DHCP after being authenticated\n");
			printf("-u <username> Specify username\n");
			printf("-p <password> Specify password\n");
			printf("-t <triggle type> broadcast / multicast\n");
			printf("-P <pid file> Store process ID of daemon in file\n");
			printf("-h Help information\n");
			exit(0);
			break;
		default:
			printf("Error argument! Type -h for help!\n");
			exit(0);
			break;
		}
	}
}

void registerAllSignalHandlers() {
	registerSignalHandler(SIGTERM, signalHandler);
	registerSignalHandler(SIGINT, signalHandler);
	registerSignalHandler(SIGALRM, signalHandler);
}

void initDeamonOnDemand() {
	if (usrArgus.needDeamon) {
		initDeamon();
		writePidFile(usrArgus.pidFile);
	}
}

void startAuthentication() {
	getInterfaceAddress(usrArgus.interface, interfaceHWAddr);
	openInterface();
	setCaptureFilter();
	triggleAuthentication();
	startCapture();
}

void openInterface() {
	char errbuf[PCAP_ERRBUF_SIZE];
	if ((fpInterface = pcap_open_live(usrArgus.interface, 1950, 0, 100, errbuf))
			!= NULL) {
//		debug("Open %s\n", usrArgus.interface);
	} else {
		DEBUG_LOG_PRINTF("Error: Open %s fail!\n", usrArgus.interface);
		exit(0);
	}
}

void setCaptureFilter() {
	struct bpf_program fcode;
	char filter[256] = { 0 };
	sprintf(filter, "ether proto 0x888e and ether host %x:%x:%x:%x:%x:%x",
			interfaceHWAddr[0], interfaceHWAddr[1], interfaceHWAddr[2],
			interfaceHWAddr[3], interfaceHWAddr[4], interfaceHWAddr[5]);
	if (pcap_compile(fpInterface, &fcode, filter, 1, 0)
			< 0) {
		DEBUG_LOG_PRINTF ("Error: Compile %s filter string fail!\n", usrArgus.interface)
		exit(0);
	}
	if (pcap_setfilter(fpInterface, &fcode) < 0) {
		DEBUG_LOG_PRINTF("Error: Set %s filter fail!\n", usrArgus.interface);
		exit(0);
	}
//	DEBUG_LOG_PRINTF("Set %s capture filter.\n", usrArgus.interface);
}

void startCapture() {
	pcap_loop(fpInterface, 0, packet_handler, NULL);
}

void triggleAuthentication() {
	sendEapolStart();
	putWatchDog();
}

void putWatchDog() {
	alarm(10); // redial if authentication doesn't succeed for 10 seconds
}

void packet_handler(u_char *user, const struct pcap_pkthdr *pkt_header,
		const u_char *pkt_data) {
	void* pbuf;
	pbuf = (void*) pkt_data;

	if (isFailure(pbuf)) {
		DEBUG_LOG_PRINTF("%s: Failure!\n", usrArgus.interface);
		state = 0;
		sleep(5);
		DEBUG_LOG_PRINTF("%s: Redial...\n", usrArgus.interface);
		triggleAuthentication();
		return;
	}

	switch (state) {
	case 1:
		if (isEapUserNameRequest(pbuf)) {
			DEBUG_LOG_PRINTF("%s: Receive EAP_Request\n", usrArgus.interface);
			memcpy(serverHWAddr, ((PDLCHDR) pbuf)->SourMAC, 6);
			packetId = ((PEAPHDR) pbuf)->Id;
			sendUsername();
		}
		break;
	case 2:
		if (isEapPapPasswordRequest(pbuf)) {
			DEBUG_LOG_PRINTF("%s: Receive EAP_Password_Request\n",
					usrArgus.interface);
			packetId = ((PEAPHDR) pbuf)->Id;
			sendPapPassword();
		}
		break;
	case 3:
		if (isSuccess(pbuf)) {
			DEBUG_LOG_PRINTF("%s: Success!\n", usrArgus.interface);
			state = 4;
			if (usrArgus.needDhcp) {
				system(usrArgus.dhcpClient);
				usrArgus.needDhcp = 0; /* just need run one time! */
			}
		}
		break;
	case 4:
		if (isKeepState(pbuf)) {
			debug("%s: Receive EAP_Request\n", usrArgus.interface);
			packetId = ((PEAPHDR) pbuf)->Id;
			sendKeepState();
		}
		break;
	}
}

void sendEapolStart() {
	generateEapolStart();

	if (!pcap_sendpacket(fpInterface, packet.buf, packet.length)) {
		DEBUG_LOG_PRINTF("%s: Send EAPOL_START\n", usrArgus.interface);
		state = 1;
	} else {
		DEBUG_LOG_PRINTF("%s: Error! Send EAPOL_START Failure!\n",
				usrArgus.interface);
	}
}

void sendEapolLogoff() {
	generateEapolLogoff();

	if (!pcap_sendpacket(fpInterface, packet.buf, packet.length)) {
		DEBUG_LOG_PRINTF("%s: Send EAPOL_Logoff\n", usrArgus.interface);
		state = 0;
	} else {
		DEBUG_LOG_PRINTF("%s: Error! Send EAPOL_Logoff Failure!\n",
				usrArgus.interface);
	}
}

void sendUsername() {
	generateUsername();

	if (!pcap_sendpacket(fpInterface, packet.buf, packet.length)) {
		state = 2;
		DEBUG_LOG_PRINTF("%s: Send EAPOL_Response\n", usrArgus.interface);
	} else {
		DEBUG_LOG_PRINTF("%s: Error! Send EAPOL_Response Failure!\n",
				usrArgus.interface);
	}
}

void sendPapPassword() {
	generatePapPassword();

	if (!pcap_sendpacket(fpInterface, packet.buf, packet.length)) {
		state = 3;
		DEBUG_LOG_PRINTF("%s: Send EAPOL_Password\n", usrArgus.interface);
	} else {
		DEBUG_LOG_PRINTF("%s: Error! Send EAPOL_Password Failure!\n",
				usrArgus.interface);
	}
}

void sendKeepState() {
	generateKeepState();

	if (!pcap_sendpacket(fpInterface, packet.buf, packet.length)) {
		debug("%s: Send EAP Response\n", usrArgus.interface);
	} else {
		debug("%s: Error! Send EAP Response Failure!\n", usrArgus.interface);
	}
}

void registerSignalHandler(int signalId, void(*handler)(int)) {
	struct sigaction act;
	act.sa_handler = handler;
	act.sa_flags = SA_RESTART;
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, signalId);
	if (-1 == sigaction(signalId, &act, 0)) {
		perror("sigaction\n");
	}
}

void signalHandler(int sigid) {
	switch (sigid) {
	case SIGTERM:
		sendEapolLogoff();
		exit(0);
		break;
	case SIGINT:
		sendEapolLogoff();
		exit(0);
		break;
	case SIGALRM:
		// redial if authentication doesn't succeed for 10 seconds
		if (state != 4) {
			sendEapolLogoff();
			sleep(2);
			triggleAuthentication();
		}
		break;
	}
}
