/*********************************************************
*
* Module Name: Client/server header file
*
* File Name:    UDPIncludes.h
*
* Summary:
*  This file contains common stuff for the client and server
*
*********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>     /* for memset() */
#include <netinet/in.h> /* for in_addr */
#include <sys/socket.h> /* for socket(), connect(), sendto(), and recvfrom() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <netdb.h>      /* for getHostByName() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <unistd.h>     /* for close() */


#define STRMAX 128      /* Longest string to echo */

void DieWithError(char *errorMessage);  /* External error handling function */

