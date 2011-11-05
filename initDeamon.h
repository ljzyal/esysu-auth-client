/*
 *  Copyright (c) 2010，XRouter GROUP
 *  All rights reserved.
 *
 *  Name        : initDeamon.h
 *  Description :
 *
 *  Version     : 1.0
 *  Author      : Xu Guanglin
 *  Created on  : Dec 11, 2010
 */

#ifndef INITDEAMON_H_
#define INITDEAMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>

void initDeamon(void);
void writePidFile(char* pidfile);
void removePidFile(char* pidfile);

#endif /* INITDEAMON_H_ */
