/*
 *  Copyright (c) 2010，XRouter GROUP
 *  All rights reserved.
 *
 *  Name        : packetGenerator.c
 *  Description :
 *
 *  Version     : 1.0
 *  Author      : Xu Guanglin
 *  Created on  : Dec 11, 2010
 */

#include	"packetFactory.h"

struct packetEncap packet = { { 0 }, 256 };
extern int packetId;
extern struct UserArguments usrArgus;
extern u8 interfaceHWAddr[];
extern u8 serverHWAddr[];
extern int state;

void generateEapolStart() {
	memset(packet.buf, 0, packet.length); // clear buffer
	packet.length = 64;
	u8* buf = packet.buf;

	setSrcDstTypeVersionCommonHeader(buf);
	((PEAPOLHDR) buf)->Type = EAPOL_Start;
	((PEAPOLHDR) buf)->Length = htons(0);

	debugPacketBytes(buf, 64);
}

void generateEapolLogoff() {
	memset(packet.buf, 0, packet.length); // clear buffer
	packet.length = 64;
	u8* buf = packet.buf;

	setSrcDstTypeVersionCommonHeader(buf);
	PEAPOLHDR pbuf = (PEAPOLHDR) buf;
	pbuf->Type = EAPOL_Logoff;
	pbuf->Length = htons(0);

	debugPacketBytes(buf, 64);
}

void generateUsername() {
	packet.length = 23+2+28+2+strlen(usrArgus.username);
	memset(packet.buf, 0, packet.length); // clear buffer
	u8* buf = packet.buf;

	setSrcDstTypeVersionCommonHeader(buf);
	((PEAPOLHDR) buf)->Type = EAP_Packet;
	((PEAPOLHDR) buf)->Length = htons(strlen(usrArgus.username) +2+28+2 + 0x05);
	((PEAPHDR) buf)->Code = EAP_Response;
	((PEAPHDR) buf)->Id = packetId;
	((PEAPHDR) buf)->Length = ((PEAPOLHDR) buf)->Length;
	((PEAPHDR) buf)->Type = EAP_Identity;

	// temp fix for version upload (iNode V5.0)
	u8 data[2+28+2] = { 0x06,
		0x07, 0x4f, 0x6a, 0x4d, 0x48, 0x53, 0x30, 0x77,
		0x4e, 0x5a, 0x69, 0x46, 0x34, 0x48, 0x78, 0x63,
		0x30, 0x63, 0x56, 0x38, 0x71, 0x4b, 0x50, 0x47,
		0x41, 0x41, 0x51, 0x49, 0x3d, 0x20, 0x20 };	
	memcpy((&((PEAPHDR) buf)->Type) + 1, data, 2+28+2);

	memcpy((&((PEAPHDR) buf)->Type) + 1 + 2+28+2, usrArgus.username, strlen(
			usrArgus.username));

	debugPacketBytes(buf, packet.length);
}

void generatePapPassword() {
	memset(packet.buf, 0, packet.length); // clear buffer
	packet.length = 60;
	u8* buf = packet.buf;

	setSrcDstTypeVersionCommonHeader(buf);
	((PEAPOLHDR) buf)->Type = EAP_Packet;
	((PEAPOLHDR) buf)->Length = htons(strlen(usrArgus.username) + strlen(
			usrArgus.password) + 0x06);
	((PEAPHDR) buf)->Code = EAP_Response;
	((PEAPHDR) buf)->Id = packetId;
	((PEAPHDR) buf)->Length = ((PEAPOLHDR) buf)->Length;
	((PEAPHDR) buf)->Type = EAP_HuaweiPAP;
	*(((u8*) &((PEAPHDR) buf)->Type) + 1) = (u8) strlen(usrArgus.password);

	memcpy((&((PEAPHDR) buf)->Type) + 2, usrArgus.password, strlen(
			usrArgus.password));
	memcpy((&((PEAPHDR) buf)->Type) + 2 + strlen(usrArgus.password),
			usrArgus.username, strlen(usrArgus.username));

	debugPacketBytes(buf, 60);
}

void generateKeepState() {
	memset(packet.buf, 0, packet.length); // clear buffer
	packet.length = 60;
	u8* buf = packet.buf;

	setSrcDstTypeVersionCommonHeader(buf);
	((PEAPOLHDR) buf)->Type = EAP_Packet;
	((PEAPOLHDR) buf)->Length = htons(strlen(usrArgus.username) + 0x05);
	((PEAPHDR) buf)->Code = EAP_Response;
	((PEAPHDR) buf)->Id = packetId;
	((PEAPHDR) buf)->Length = ((PEAPOLHDR) buf)->Length;
	((PEAPHDR) buf)->Type = EAP_Identity;
	memcpy((&((PEAPHDR) buf)->Type) + 1, usrArgus.username, strlen(
			usrArgus.username));

	debugPacketBytes(buf, 60);
}

void setSrcDstTypeVersionCommonHeader(u8* buf) {
	u8 dstBroadcast[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
	u8 dstMulticast[6] = { 0x01, 0x80, 0xc2, 0x00, 0x00, 0x03 };
	u8* src = interfaceHWAddr;
	u8* dst;
	if (state == 0) {
		dst = usrArgus.triggerType == TRIGGER_TYPE_BROADCAST ? dstBroadcast
				: dstMulticast;
	} else {
		dst = serverHWAddr;
	}

	PDLCHDR pbuf = (PDLCHDR) buf;
	memcpy(pbuf->DestMAC, dst, 6);
	memcpy(pbuf->SourMAC, src, 6);
	pbuf->EthType = htons(EAPOL);
	PEAPOLHDR ebuf = (PEAPOLHDR) buf;
	ebuf->Version = 1;
}

int isEapUserNameRequest(u8* buf) {
	if (!memcmp(((PDLCHDR) buf)->DestMAC, interfaceHWAddr, 6)
			&& ((PEAPOLHDR) buf)->Type == EAP_Packet && ((PEAPHDR) buf)->Code
			== EAP_Request && ((PEAPHDR) buf)->Type == EAP_Identity) {
		return 1;
	}
	return 0;
}

int isEapPapPasswordRequest(u8* buf) {
	if (!memcmp(((PDLCHDR) buf)->DestMAC, interfaceHWAddr, 6)
			&& ((PEAPOLHDR) buf)->Type == EAP_Packet && ((PEAPHDR) buf)->Code
			== EAP_Request && ((PEAPHDR) buf)->Type == EAP_HuaweiPAP) {
		return 1;
	}
	return 0;
}

int isSuccess(u8* buf) {
	if (!memcmp(((PDLCHDR) buf)->DestMAC, interfaceHWAddr, 6)
			&& ((PEAPOLHDR) buf)->Type == EAP_Packet && ((PEAPHDR) buf)->Code
			== EAP_Success) {
		return 1;
	}
	return 0;
}

int isFailure(u8* buf) {
	if (!memcmp(((PDLCHDR) buf)->DestMAC, interfaceHWAddr, 6)
			&& ((PEAPOLHDR) buf)->Type == EAP_Packet && ((PEAPHDR) buf)->Code
			== EAP_Failure && state > 0) {
		return 1;
	}
	return 0;
}

int isKeepState(u8* buf) {
	return isEapUserNameRequest(buf);
}
