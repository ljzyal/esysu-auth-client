/*
 *  Copyright (c) 2010，XRouter, GROUP
 *  All rights reserved.
 *
 *  Name        : general.h
 *  Description :
 *
 *  Version     : 1.0
 *  Author      : Xu Guanglin
 *  Created on  : Dec 2, 2010
 */

#ifndef GENERAL_H_
#define GENERAL_H_

#include "../custom_types.h"

// ETHER TYPE (802.1X Standard)
#define		EAPOL				0x888e
//  802.1X EAPOL HEADER PACKET TYPE (802.1X Standard)
#define		EAP_Packet			0x00
#define		EAPOL_Start			0x01
#define		EAPOL_Logoff		0x02
//	EAP HEADER Code (RFC3748)
#define		EAP_Request			0x01
#define		EAP_Response		0x02
#define		EAP_Success			0x03
#define		EAP_Failure			0x04
//	EAP HEADER Type (RFC3748)
#define		EAP_Identity		0x01
#define		EAP_Notification	0x02
#define		EAP_NakResponse		0x03
#define		EAP_MD5Challenge	0x04
#define		EAP_OneTimePassword	0x05
#define		EAP_GenericTokenCard 0x06
#define		EAP_HuaweiPAP		0x07 /*Out of Sandard*/


//Structure bytes align. Need for network packet
#pragma pack(push)
#pragma pack(1)
// DLC header
typedef struct DlcHdr {
	u8 	DestMAC[6];
	u8 	SourMAC[6];
	u16	EthType;
}DLCHDR, *PDLCHDR;

// EAPOL header
typedef struct EapolHdr {
	DLCHDR DHDR;
	u8 	Version;
	u8 	Type;
	u16 Length;
}EAPOLHDR, *PEAPOLHDR;

// EAP header
typedef struct EapHdr
{
	EAPOLHDR EOHDR;
	u8	Code;
	u8	Id;
	u16 Length;
	u8	Type;
}EAPHDR, *PEAPHDR;
#pragma pack(pop) /* restore pack  config */


#endif /* GENERAL_H_ */
